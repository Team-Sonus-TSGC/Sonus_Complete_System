#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "main.h"
#include "dsp.h"
#include "ipc_interface.h"

// alarm buffer length required for n integer cycles of 750 Hz
#define ALARM_BUFFER_LENGTH 64

static const float FREQ_MIN = 200;
static const float FREQ_MAX = 8000;

// maximum multiplier for voice volume
static const float VOICE_VOLUME_MAX = 16.0f;

// normalized voice volume variable
static float voice_volume_normalized = 0.5f;
static float voice_volume_log_approx;

// buffer for alarm tone, holds an integer number of cycles to avoid discontinuities
static int16_t alarm_tone_data_buffer[ALARM_BUFFER_LENGTH];

// decoupled index for alarm tone buffer so alarm is continuous across iterations
static uint16_t alarm_tone_buffer_index = 0;

// separate sonification index so sonification is not coupled to
//   the passed sample chunks, sonification is continuous throughout time
static uint32_t sonification_index_n = 0;

// boolean flag used to gracefully end sonification in event that the anomaly ends
//   and the sonification wave is not yet zero -> avoid pop on output
static boolean_t sonification_active = FALSE;

// normalized sonification frequency from DSP
static volatile float sonification_frequency_normalized = 0.5f;

// Hz sonification frequency determined from normalized frequency and
//    supported output frequency range
static float sonification_frequency_current = 1000.0f;
static float sonification_frequency_new = 1000.0f;
static float sonification_magnitude = -100.0f;

// current/previous used only to avoid discontinuities when changing frequencies
//  if a frequency change happens, it must occur when the speaker has positive inertia, i.e. it is
//  traveling in the same direction as it should for the next sample at the new frequency,
//  a discontinuity is heard otherwise
static int16_t sonification_sample_current;
static int16_t sonification_sample_previous;

// boolean flags to detect change in alarm state
static boolean_t alarm_state_current = FALSE;
static boolean_t alarm_state_previous = FALSE;

// counter used for alarm on/off cycle
static uint32_t alarm_counter = 0;
static boolean_t alarm_mute = FALSE;

// HAL defined variable
extern UART_HandleTypeDef huart3;

// one-time generation of alarm tone,
//   contains integer number of cycles of alarm frequency to avoid discontinuities
void alarminit( )
{
  int n;

  for (n = 0; n < ALARM_BUFFER_LENGTH; ++n)
	{
		alarm_tone_data_buffer[n] = (65535/64)* sinf((750.0f/16000.0f)*2*M_PI*n);
	}
}

// passed array is voice audio from DSP
void playAudio( int16_t *audio_samples, uint16_t audio_sample_length )
{
	alarm_state_previous = alarm_state_current;
	alarm_state_current = dspGetIsAnomalyDetected( );

	if ( alarm_state_current && !alarm_state_previous )
	{
	  // alarm just went active, set mute false and set counter used for on/off cycle to zero
    alarm_mute = FALSE;
    alarm_counter = 0;

    // set global mute state in IPC
    IPCSetAlarmMuteState( FALSE );
	}
	else
	{
	  // update mute state from IPC
	  alarm_mute = IPCGetAlarmMuteState( );
	}

	// update voice volume from IPC
	voice_volume_normalized = IPCGetVoiceVolumeNormalized( );
	voice_volume_log_approx = powf(voice_volume_normalized, 4.0f) * VOICE_VOLUME_MAX;
	// scale raw voice audio data depending on volume variable, the other sources have their own volume standards
	// TODO - adjust this scaling so the bounds are reasonable
	if ( IPCGetVoiceMuteState( ) )
	{
	  // voice is muted, zero each sample
    for ( int index = 0; index < audio_sample_length; index++ )
    {
      audio_samples[index] = 0;
    }
	}
	else
  {
	  // voice is active, adjust volume
    for ( int index = 0; index < audio_sample_length; index++ )
    {
      audio_samples[index] = (int16_t)((float)audio_samples[index] * voice_volume_log_approx);
    }
	}

	// Mix in alarm audio, if alarm active or needs to end
  if ( alarm_state_current && !alarm_mute )
  {
    for ( int index = 0; index < audio_sample_length; index++ )
    {
	    if (( alarm_counter % 16000 < 4000 ) || ( alarm_tone_buffer_index != 0))
	    {
	      // if in first 1/4 of a second, play alarm tone
	      // OR if alarm tone buffer index not zero -> to avoid pop
	      // Alarm tone should be active
        audio_samples[index] += alarm_tone_data_buffer[alarm_tone_buffer_index];

        alarm_tone_buffer_index = (alarm_tone_buffer_index == (ALARM_BUFFER_LENGTH - 1)
                                      ? 0 : alarm_tone_buffer_index + 1);
	    }
	    alarm_counter++;
	  }
	}
  else if ( alarm_tone_buffer_index != 0 )
  {
    // TODO - same end condition as sonification? sample size 0?
    for ( int index = 0; index < audio_sample_length; index++ )
    {
      // alarm is not active but index is not zero, keep adding alarm samples until zero to avoid pop

      audio_samples[index] += alarm_tone_data_buffer[alarm_tone_buffer_index];

      alarm_tone_buffer_index = (alarm_tone_buffer_index == (ALARM_BUFFER_LENGTH - 1)
                                    ? 0 : alarm_tone_buffer_index + 1);
    }
  }

  // TODO - scale depending on magnitude from DSP *** test this
	// if alarm active
  if ( alarm_state_current )
  {
    sonification_active = TRUE;

    // get normalized sonification frequency from DSP
    // if frequency changes, continue previous frequency until sonification value
    //   is at or close to zero to avoid output pop -> 400 Hz from discontinuity

    sonification_frequency_normalized = dspGetAnomalyFrequencyNormalized( );

    // ** current and desired sonification_frequency variables? -> if current != desired,
    //      calculate current until |sample| < 50, then current = desired
    if ( alarm_state_previous == FALSE )
    {
      sonification_frequency_new = FREQ_MIN + (FREQ_MAX - FREQ_MIN) * sonification_frequency_normalized;
      sonification_magnitude = dspGetAnomalyMagnitude( );
    }
    else
    {
      // kind of rolling average to smooth out frequency and magnitude changes
      sonification_frequency_new = (sonification_frequency_new * 399.0f / 400.0f)
                                     + (FREQ_MIN + (FREQ_MAX - FREQ_MIN) * sonification_frequency_normalized)
                                       / 400.0f;
      sonification_magnitude = (sonification_magnitude * 399.0f / 400.0f) + dspGetAnomalyMagnitude( ) / 400.0f;
    }

    // for each sample in given array
    for ( int index = 0; index < audio_sample_length; index++ )
    {
      // calculate sample, volume dependent on standard maximum and linear with dsp reported magnitude
      // sample magnitude has bounds from max of standard magnitude (65535/64) to min of (65535/184)
      // TODO - test this volume setup to see if it and the bounds work well
      sonification_sample_previous = sonification_sample_current;
      sonification_sample_current = (65535/(64 + limitFloat(2 * ( 60 - sonification_magnitude) / 6, 0.0f, 120.0f)))
                                      *sinf((sonification_frequency_current / 16000.0f)*2*M_PI*sonification_index_n);

      audio_samples[index] += sonification_sample_current;
      sonification_index_n++;

      // if sonification frequency needs to change
      if ( sonification_frequency_new != sonification_frequency_current )
      {
        // if sound sample is small in magnitude and speaker has positive inertia
        // *** zero because this works well, no noticable delay, and even 1 seems to be audible
        if ((( sonification_sample_current == 0))
            && (sonification_sample_previous < sonification_sample_current))
        {
          // change frequency and reset sonification index to 1 (current index is already a zero value, next sample will use index 1)
          sonification_frequency_current = sonification_frequency_new;
          sonification_index_n = 1;
        }
      }
    }
  }
  else if ( sonification_active )
  {
    // alarm is not active, but sonification needs to end cleanly
    // for each sample in array, unless sonification has reached end condition
    for ( int index = 0; (index < audio_sample_length) && sonification_active; index++ )
    {
      // calculate sample, volume dependent on standard maximum and linear with dsp reported magnitude
      // sample magnitude has bounds from max of standard magnitude (65535/64) to min of (65535/184)
      // TODO - test this volume setup to see if it and the bounds work well
      int16_t sonification_sample = (65535/(64 + limitFloat(2 * ( 60 - sonification_magnitude) / 6, 0.0f, 120.0f)))
                                      *sinf((sonification_frequency_current / 16000.0f)*2*M_PI*sonification_index_n);

      // end condition for sonification, no active anomaly and current sample small enough in amplitude to avoid output pop
      // *** zero because this works well, no noticable delay, and even 1 seems to be audible
      if ( sonification_sample == 0 )
      {
        sonification_active = FALSE;
      }
      audio_samples[index] += sonification_sample;
      sonification_index_n++;
    }
  }

	// write mixed output audio to output -> stlink uart -> usb com port
  HAL_UART_Transmit_DMA( &huart3, (uint8_t*)audio_samples, audio_sample_length*2 );
}
