#ifndef BUTTONS_REF_H
#define BUTTONS_REF_H

#include <linux/uinput.h>    //Reference to keycodes

#define BUTTONS_SIZE        53
#define AXES_SIZE        19
#define MAX_ABS_VAL      32767
#define MIN_ABS_VAL      -32767

namespace buttons_ref {
    static const uint16_t BUTTONS[] =
            {
                    BTN_TRIGGER,
                    BTN_THUMB,
                    BTN_THUMB2,
                    BTN_TOP,
                    BTN_TOP2,
                    BTN_PINKIE,
                    BTN_BASE,
                    BTN_BASE2,
                    BTN_BASE3,
                    BTN_BASE4,
                    BTN_BASE5,
                    BTN_BASE6,
                    BTN_DEAD,
                    BTN_TRIGGER_HAPPY1,
                    BTN_TRIGGER_HAPPY2,
                    BTN_TRIGGER_HAPPY3,
                    BTN_TRIGGER_HAPPY4,
                    BTN_TRIGGER_HAPPY5,
                    BTN_TRIGGER_HAPPY6,
                    BTN_TRIGGER_HAPPY7,
                    BTN_TRIGGER_HAPPY8,
                    BTN_TRIGGER_HAPPY9,
                    BTN_TRIGGER_HAPPY10,
                    BTN_TRIGGER_HAPPY11,
                    BTN_TRIGGER_HAPPY12,
                    BTN_TRIGGER_HAPPY13,
                    BTN_TRIGGER_HAPPY14,
                    BTN_TRIGGER_HAPPY15,
                    BTN_TRIGGER_HAPPY16,
                    BTN_TRIGGER_HAPPY17,
                    BTN_TRIGGER_HAPPY18,
                    BTN_TRIGGER_HAPPY19,
                    BTN_TRIGGER_HAPPY20,
                    BTN_TRIGGER_HAPPY21,
                    BTN_TRIGGER_HAPPY22,
                    BTN_TRIGGER_HAPPY23,
                    BTN_TRIGGER_HAPPY24,
                    BTN_TRIGGER_HAPPY25,
                    BTN_TRIGGER_HAPPY26,
                    BTN_TRIGGER_HAPPY27,
                    BTN_TRIGGER_HAPPY28,
                    BTN_TRIGGER_HAPPY29,
                    BTN_TRIGGER_HAPPY30,
                    BTN_TRIGGER_HAPPY31,
                    BTN_TRIGGER_HAPPY32,
                    BTN_TRIGGER_HAPPY33,
                    BTN_TRIGGER_HAPPY34,
                    BTN_TRIGGER_HAPPY35,
                    BTN_TRIGGER_HAPPY36,
                    BTN_TRIGGER_HAPPY37,
                    BTN_TRIGGER_HAPPY38,
                    BTN_TRIGGER_HAPPY39,
                    BTN_TRIGGER_HAPPY40
            };


    static const uint16_t AXES[] =
            {
                    ABS_X,
                    ABS_Y,
                    ABS_Z,
                    ABS_RX,
                    ABS_RY,
                    ABS_RZ,
                    ABS_THROTTLE,
                    ABS_RUDDER,
                    ABS_WHEEL,
                    ABS_GAS,
                    ABS_BRAKE,
                    ABS_HAT0X,
                    ABS_HAT0Y,
                    ABS_HAT1X,
                    ABS_HAT1Y,
                    ABS_HAT2X,
                    ABS_HAT2Y,
                    ABS_HAT3X,
                    ABS_HAT3Y
            };

}//namespace buttons_ref

#endif


