// !!! DO NOT ADD #pragma once !!! //

// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(stefan)
RGB_MATRIX_EFFECT(stefan_nexus)
RGB_MATRIX_EFFECT(stefan_wide)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

    static uint8_t some_global_state;

    static void stefan_complex_init(effect_params_t* params) {
        some_global_state = 1;
    }

    static HSV stefan_math(HSV hsv, uint16_t offset) {
        hsv  = rgb_matrix_config.hsv;
#       ifdef RGB_MATRIX_SOLID_REACTIVE_GRADIENT_MODE
            hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 8) >> 4);
#       endif
        hsv.v = scale8(255 - offset, hsv.v);

        if (hsv.v <40) {
            hsv.v = 40;
            hsv.h = 0;
            hsv.s =0;
        }
        return hsv;
    }

    static HSV stefan_nexus_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
        uint16_t effect = tick - dist;
        hsv.h = rgb_matrix_config.hsv.h;
        if (effect > 255) effect = 255;
        if (dist > 72) effect = 255;
        if ((dx > 8 || dx < -8) && (dy > 8 || dy < -8)) effect = 255;
#       ifdef RGB_MATRIX_SOLID_REACTIVE_GRADIENT_MODE
          hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 8) >> 4) + dy / 4;
#       else
           hsv.h = rgb_matrix_config.hsv.h + dy / 4;
#       endif
        hsv.v = qadd8(hsv.v, 255 - effect);

        if (hsv.v <= 40) {
            hsv.v = 40;
            hsv.h = 0;
            hsv.s = 0;
        }

        return hsv;
    }

    static HSV stefan_wide_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
        uint16_t effect = tick + dist * 5;
        if (effect > 255) effect = 255;
#       ifdef RGB_MATRIX_SOLID_REACTIVE_GRADIENT_MODE
           hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 8) >> 4);
#       endif
        hsv.v = qadd8(hsv.v, 255 - effect);
        yaif (hsv.v <= 40) {
            hsv.v = 40;
            hsv.h = 0;
            hsv.s = 0;
        }
        return hsv;
    }

    static bool stefan(effect_params_t* params) {
        if (params->init) stefan_complex_init(params);
        return effect_runner_reactive(params, &stefan_math);
    }

    static bool stefan_nexus(effect_params_t* params) {
        if (params->init) stefan_complex_init(params);
       // return effect_runner_reactive_splash(0, params, &stefan_nexus_math);
        return effect_runner_reactive_splash(qsub8(g_last_hit_tracker.count, 1), params, &stefan_nexus_math);
    }

    static bool stefan_wide(effect_params_t* params) {
        //return effect_runner_reactive_splash(qsub8(g_last_hit_tracker.count, 1), params, &SOLID_REACTIVE_WIDE_math);
        return effect_runner_reactive_splash(0, params, &stefan_wide_math);
    }

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
