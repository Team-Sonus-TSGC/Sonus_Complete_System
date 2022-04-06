/*
 * ipc_interface.c
 *
 *  Created on: Apr 6, 2022
 *      Author: Mason
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "ipc_interface.h"

// This is shared memory at a defined RAM section to enable IPC
//  with located variables
static volatile struct
{
  // normalized voice volume
  float voice_volume_normalized;

  // voice mute
  boolean_t voice_mute_active;

  // TRUE if dsp detects a current anomaly
  boolean_t anomaly_detection_active;

  // TRUE if alarm tone is muted
  boolean_t alarm_mute;

  // one anomaly frequency calculated from microphones with active detection
  float anomaly_frequency;

  // one anomaly magnitude calculated from microphones with active detection
  float anomaly_magnitude_db;

  // individual anomaly detect states for each microphone
  boolean_t microphone_detect_states[6];

  // individual anomaly magnitudes for each microphone, not tied to detection state
  float microphone_anomaly_magnitudes_db[6];

  // individual anomaly frequencies for each microphone
  float microphone_anomaly_frequencies[6];

  // x, y, z vector indicating anomaly direction
  int8_t anomaly_vector[3];
} shared_memory __attribute__((section(".ipc_data")));

void IPCInitialize( void )
{
  // only initialize a few, the others are constantly updated by DSP or others
  shared_memory.voice_volume_normalized = 0.5f;
  shared_memory.voice_mute_active = FALSE;
  shared_memory.anomaly_detection_active = FALSE;
  shared_memory.alarm_mute = FALSE;
  shared_memory.anomaly_vector[0] = 0;
  shared_memory.anomaly_vector[1] = 0;
  shared_memory.anomaly_vector[2] = 0;
}

float IPCGetVoiceVolumeNormalized( void )
{
  return shared_memory.voice_volume_normalized;
}

void IPCSetVoiceVolumeNormalized( float new_vol )
{
  shared_memory.voice_volume_normalized = new_vol;
}

boolean_t IPCGetVoiceMuteState( void )
{
  return shared_memory.voice_mute_active;
}

void IPCSetVoiceMuteState( boolean_t new_state )
{
  shared_memory.voice_mute_active = new_state;
}

boolean_t IPCGetAnomalyDetectState( void )
{
  return shared_memory.anomaly_detection_active;
}

void IPCSetAnomalyDetectState( boolean_t new_state )
{
  shared_memory.anomaly_detection_active = new_state;
}

float IPCGetAnomalyFrequency( void )
{
  return shared_memory.anomaly_frequency;
}

void IPCSetAnomalyFrequency( float new_freq )
{
  shared_memory.anomaly_frequency = new_freq;
}

float IPCGetAnomalyMagnitudedB( void )
{
  return shared_memory.anomaly_magnitude_db;
}

void IPCSetAnomalyMagnitudedB( float new_mag )
{
  shared_memory.anomaly_magnitude_db = new_mag;
}

boolean_t IPCGetAlarmMuteState( void )
{
  return shared_memory.alarm_mute;
}

void IPCSetAlarmMuteState( boolean_t new_state )
{
  shared_memory.alarm_mute = new_state;
}

#ifdef __cplusplus
}
#endif
