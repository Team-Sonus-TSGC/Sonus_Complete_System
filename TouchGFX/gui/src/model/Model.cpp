#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <gui/common/FrontendApplication.hpp>
#include "Definitions.hpp"
#include "ipc_interface.h"

//#include "main.h"
boolean_t led1 = TRUE;
extern int currentDelta;
Model::Model() : modelListener(0)
{

}

bool swStatus = false;
bool swFilter = false;

bool muteStatus = false;
bool muteFilter = false;

bool anomaly_detect_state_previous;
bool anomaly_detect_state_current = false;

uint16_t encoderCount = 0;
void Model::tick()
{
  // update anomaly detect state from IPC
  anomaly_detect_state_previous = anomaly_detect_state_current;
  // a direct cast to bool doesnt work here, need a conditional
  anomaly_detect_state_current = ((int)IPCGetAnomalyDetectState( ) != 0 ? true : false);

  // only update GUI on change of state
  if ( anomaly_detect_state_current != anomaly_detect_state_previous )
  {
    static_cast<FrontendApplication*>(Application::getInstance())->handleKeyEvent(87);
  }

  // voice mute
	bool currentSWStatus = HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_3);
    if(currentSWStatus != swStatus){
	   swStatus = currentSWStatus;
	   if(swFilter) {
		   static_cast<FrontendApplication*>(Application::getInstance())->handleKeyEvent(89);
	   }
	   swFilter = !swFilter;
	}

  // alarm mute button
	bool currentMuteStatus = HAL_GPIO_ReadPin(GPIOK, GPIO_PIN_1);
    if(currentMuteStatus != muteStatus){
	   muteStatus = currentMuteStatus;
	   if(muteFilter) {
		   static_cast<FrontendApplication*>(Application::getInstance())->handleKeyEvent(90);
	   }
	   muteFilter = !muteFilter;
	}

  // TIM4 serves as the encoder counter
  if (TIM4->CNT != encoderCount)
  {
    int16_t encoderDelta = TIM4->CNT - encoderCount;
    encoderCount = TIM4->CNT;
    currentDelta = encoderDelta * 2;

    static_cast<FrontendApplication*>(Application::getInstance())->handleKeyEvent(88);
  }
}
