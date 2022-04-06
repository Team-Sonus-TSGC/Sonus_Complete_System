/*
 * ipc_interface.h
 *
 *  Created on: Apr 6, 2022
 *      Author: Mason
 */

#ifndef INC_IPC_INTERFACE_H_
#define INC_IPC_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

// initializes a few of the shared variables
void IPCInitialize( void );

// vanilla getters/setters for global states
float IPCGetVoiceVolumeNormalized( void );
void IPCSetVoiceVolumeNormalized( float );
boolean_t IPCGetVoiceMuteState( void );
void IPCSetVoiceMuteState( boolean_t );
boolean_t IPCGetAnomalyDetectState( void );
void IPCSetAnomalyDetectState( boolean_t );
float IPCGetAnomalyFrequency( void );
void IPCSetAnomalyFrequency( float );
float IPCGetAnomalyMagnitudedB( void );
void IPCSetAnomalyMagnitudedB( float );

boolean_t IPCGetAlarmMuteState( void );
void IPCSetAlarmMuteState( boolean_t );

// TODO - full array getters/setters
// TODO - vector getter/setter

#ifdef __cplusplus
}
#endif

#endif /* INC_IPC_INTERFACE_H_ */
