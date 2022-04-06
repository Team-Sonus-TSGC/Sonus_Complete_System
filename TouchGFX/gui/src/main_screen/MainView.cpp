#include <gui/main_screen/MainView.hpp>
#include "ipc_interface.h"

systemState state;
int currentDelta = 0;

MainView::MainView()
{
}

void MainView::setupScreen()
{
	//setup voiceToggle
	if(!state.voiceToggle) {
		VoiceCommMess_Text_On.setVisible(false);
		VoiceCommMess_Text_Off.setVisible(true);

		VoiceCommMess_Text_On.invalidate();
		VoiceCommMess_Text_Off.invalidate();

		Voice_Toggle_Button.forceState(true);
		Voice_Toggle_Button.invalidate();
	}

	//setup volume
	textProgress1.setValue(state.volume);
	Volume_Slider.setValue(state.volume);
	Volume_Slider.invalidate();

	//setup anomaly message
	AnomalyWarning_Textbox_1_1.setVisible(state.anomalyDetected);
	AnomalyWarning_Textbox_1_1.invalidate();

	MessageBoard_Message_1.setVisible(state.alert);
	MessageBoard_Message_1.invalidate();
	MessageBoard_Message_Muted.setVisible(!state.alert);
	MessageBoard_Message_Muted.invalidate();

	//Set buttons
	Localize_Button.setVisible(state.anomalyDetected);
	Localize_Button.invalidate();
	Alert_Mute_Button.setVisible(state.anomalyDetected);
	Alert_Mute_Button.invalidate();

	//Set disabled button images
	Disabled_Button1.setVisible(!state.anomalyDetected);
	Disabled_Button1.invalidate();
	Disabled_Button2.setVisible(!state.anomalyDetected);
	Disabled_Button2.invalidate();
}

void MainView::tearDownScreen()
{
}

void MainView::set_volume_slider(int value) {
	state.volume = value;
	textProgress1.setValue(value);

  // update IPC voice volume, normalized 0.0f to 1.0f
  IPCSetVoiceVolumeNormalized( (float)state.volume/100.0f );
}

void MainView::set_volume_encoder() {
	int tempVolume = state.volume + currentDelta;
	if(tempVolume > 100) {
		tempVolume = 100;
	}
	else if(tempVolume < 0) {
		tempVolume = 0;
	}
	state.volume = tempVolume;

	// update IPC voice volume, normalized 0.0f to 1.0f
	IPCSetVoiceVolumeNormalized( (float)state.volume/100.0f );

	//setup volume
	textProgress1.setValue(state.volume);
	Volume_Slider.setValue(state.volume);
	Volume_Slider.invalidate();

}

/*
 * Virtual function called when either toggle button is pressed
 */
void MainView::toggle_voice() {
	//Toggle voiceToggle variable
	state.voiceToggle = !state.voiceToggle;

	// update IPC, mute state is !voiceToggle
	IPCSetVoiceMuteState( (boolean_t)!state.voiceToggle );

	//Update GUI accordingly
	if(state.voiceToggle) {
		VoiceCommMess_Text_On.setVisible(true);
		VoiceCommMess_Text_Off.setVisible(false);

		VoiceCommMess_Text_On.invalidate();
		VoiceCommMess_Text_Off.invalidate();

	} else {
		VoiceCommMess_Text_On.setVisible(false);
		VoiceCommMess_Text_Off.setVisible(true);

		VoiceCommMess_Text_On.invalidate();
		VoiceCommMess_Text_Off.invalidate();
	}

	if(Voice_Toggle_Button.getState() == state.voiceToggle) {
		Voice_Toggle_Button.forceState(!state.voiceToggle);
		Voice_Toggle_Button.invalidate();
	}
}

void MainView::update_anomaly_alert() {
  // a direct cast to bool doesnt work here, need a conditional
	state.anomalyDetected = (IPCGetAnomalyDetectState( ) != 0 ? true : false);
	state.alert = state.anomalyDetected;

	//Set alert message
	AnomalyWarning_Textbox_1_1.setVisible(state.anomalyDetected);
	AnomalyWarning_Textbox_1_1.invalidate();

	//Set text of alert message
	MessageBoard_Message_1.setVisible(state.alert);
	MessageBoard_Message_1.invalidate();
	MessageBoard_Message_Muted.setVisible(!state.alert);
	MessageBoard_Message_Muted.invalidate();

	//Set buttons
	Localize_Button.setVisible(state.anomalyDetected);
	Localize_Button.invalidate();
	Alert_Mute_Button.setVisible(state.anomalyDetected);
	Alert_Mute_Button.invalidate();

	//Set disabled button images
	Disabled_Button1.setVisible(!state.anomalyDetected);
	Disabled_Button1.invalidate();
	Disabled_Button2.setVisible(!state.anomalyDetected);
	Disabled_Button2.invalidate();
}

void MainView::alert_mute_pressed() {
  // toggle IPC mute, doesnt matter if alarm is not active, output audio sets it active on new event
  IPCSetAlarmMuteState( (boolean_t)!IPCGetAlarmMuteState( ) );

	if(state.alert) {
		state.alert = false;

		MessageBoard_Message_1.setVisible(state.alert);
		MessageBoard_Message_1.invalidate();
		MessageBoard_Message_Muted.setVisible(!state.alert);
		MessageBoard_Message_Muted.invalidate();
	}
}

