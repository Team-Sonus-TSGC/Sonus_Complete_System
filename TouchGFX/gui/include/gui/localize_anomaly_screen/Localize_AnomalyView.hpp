#ifndef LOCALIZE_ANOMALYVIEW_HPP
#define LOCALIZE_ANOMALYVIEW_HPP

#include <gui_generated/localize_anomaly_screen/Localize_AnomalyViewBase.hpp>
#include <gui/localize_anomaly_screen/Localize_AnomalyPresenter.hpp>

class Localize_AnomalyView : public Localize_AnomalyViewBase
{
public:
    Localize_AnomalyView();
    virtual ~Localize_AnomalyView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // LOCALIZE_ANOMALYVIEW_HPP
