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

  // Actuate and toggle alarm LEDS
  // * Originally went for series LEDs with center tap by driving output, but they remain lit even without output active,
  //     so added another output to conditionally provide 3v3 to the series
  // TODO - Might as well use one output for each LED, instead of the current kind of fixed arrangement
  // TODO - This level of hardware access should NOT be here, but it's 2 days to FDR and I'm long out of patience
  if ( anomaly_detect_state_current )
  {
    // enable the driving output
    GPIOH->BSRR |= 1 << 15;

    // half second on, half second off for each
    // wiring so that one or the other is on
    if ( HAL_GetTick( ) % 1000 < 500 )
    {
      GPIOE->BSRR |= 1 << 3;
    }
    else
    {
      GPIOE->BSRR |= 1 << (3 + 16);
    }
  }
  else
  {
    // disable outputs driving LEDs
    GPIOH->BSRR |= 1 << (15 + 16);
    GPIOE->BSRR |= 1 << (3 + 16);
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
