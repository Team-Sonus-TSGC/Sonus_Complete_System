#ifndef LOCALIZE_ANOMALYPRESENTER_HPP
#define LOCALIZE_ANOMALYPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Localize_AnomalyView;

class Localize_AnomalyPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Localize_AnomalyPresenter(Localize_AnomalyView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~Localize_AnomalyPresenter() {};

private:
    Localize_AnomalyPresenter();

    Localize_AnomalyView& view;
};

#endif // LOCALIZE_ANOMALYPRESENTER_HPP
