#ifndef MAIN_VIEW_HPP
#define MAIN_VIEW_HPP

#include <gui_generated/main_screen/MainViewBase.hpp>
#include <gui/main_screen/MainPresenter.hpp>

struct systemState {
	//Setting defaults
	int volume = 50;
	bool voiceToggle = true;
	volatile bool anomalyDetected = false;
	bool alert = false;
	bool alert_mute = false;
};


class MainView : public MainViewBase
{
public:
    MainView();
    virtual ~MainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void set_volume_slider(int value);
    virtual void set_volume_encoder();
    virtual void toggle_voice();
    virtual void update_anomaly_alert();
    virtual void alert_mute_pressed();
protected:
};

#endif // MAIN_VIEW_HPP
