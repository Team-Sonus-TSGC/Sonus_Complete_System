/*
 * dsp.h
 *
 *  Created on: Jan 26, 2022
 *      Author: Mason
 */

#ifndef INC_DSP_H_
#define INC_DSP_H_

#include "common.h"

// initializes and runs the dsp subsystem
// contains infinite loop
void dspEntry( void );

// returns anomaly detection state, TRUE if active detection on any channel
boolean_t dspGetIsAnomalyDetected( void );

// returns single frequency of array from channels with active anomaly,
//   or of all channels if no anomaly active state
// **magnitude determined relative to detection threshold
// Frequency in Hertz
float dspGetAnomalyFrequency( void );

// returns single frequency of highest magnitude on array from channels with active anomaly,
//   or of all channels if no anomaly active state
// **magnitude determined relative to detection threshold
// Frequency normalized 0.0 to 1.0
float dspGetAnomalyFrequencyNormalized( void );

// returns largest ultrasonic frequency magnitude of any channel relative to
//   the detection threshold
// **magnitude in dBV
float dspGetAnomalyMagnitude( void );

// get individual anomaly detectino states for each microphone
void dspGetMicrophoneAnomalyDetectStates( boolean_t *mic_1, boolean_t *mic_2, boolean_t *mic_3,
                                       boolean_t *mic_4, boolean_t *mic_5, boolean_t *mic_6 );

// get individual dominating frequency for each microphone
void dspGetMicrophoneAnomalyDetectFrequencies( float *mic_1_freq, float *mic_2_freq, float *mic_3_freq,
                                            float *mic_4_freq, float *mic_5_freq, float *mic_6_freq );

// get individual ultrasonic magnitudes for each microphone
// **relative to detection threshold
// **magnitude in dBV
void dspGetMicrophoneAnomalyMagnitudes( float *mic_1_db, float *mic_2_db, float *mic_3_db,
                                     float *mic_4_db, float *mic_5_db, float *mic_6_db );

#endif /* INC_DSP_H_ */
