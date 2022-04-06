/*
 * Alarm.h
 *
 *  Created on: Mar 18, 2022
 *      Author: Nickg
 */

#ifndef INC_OUTPUT_AUDIO_H_
#define INC_OUTPUT_AUDIO_H_

void alarminit( void );
void playAudio( int16_t *audio_samples, uint16_t audio_sample_length );

#endif /* INC_OUTPUT_AUDIO_H_ */
