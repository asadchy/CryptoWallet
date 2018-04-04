#ifndef CUSTOMTRANSITION_HPP_
#define CUSTOMTRANSITION_HPP_
#include <touchgfx/transitions/Transition.hpp>
#include <touchgfx/widgets/SnapshotWidget.hpp>
#include <touchgfx/widgets/Image.hpp>

using namespace touchgfx;

class CustomTransition : public Transition
{
public:
    CustomTransition();
    virtual ~CustomTransition();
    virtual void init();
    virtual void tearDown();
    virtual void handleTickEvent();

private:

    /**
     * An enum defining the different states the transition can have.
     */
    typedef enum
    {
        OPENING,
        CLOSING,
        CLOSED,
        PAUSE
    } DoorState;

    /**
     * The snapshot widget is used to take a snapshot of the screen transitioning from.
     */
    SnapshotWidget snapshot;

    /**
     * Image for the left door.
     */
    Image leftDoorImage;

    /**
     * Image for the right door.
     */
    Image rightDoorImage;

    /**
     * The counter defining the number of ticks to wait before opening the doors.
     */
    uint8_t pauseCounter;

    /**
     * The current state of the transition.
     */
    DoorState doorState;

    /**
     * The number of pixels to move the doors on the x-axis at each tick.
     */
    static const uint8_t DELTA_X = 6;

    static const uint8_t PAUSE_TICKS = 30;
};

#endif /* CUSTOMTRANSITION_HPP_ */
