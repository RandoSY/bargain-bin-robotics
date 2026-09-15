#ifndef BALA2_TELEMETRY_H
#define BALA2_TELEMETRY_H

#include <stdint.h>

// Starts a local Wi-Fi access point and UDP telemetry stream.
void telemetryBegin();

// Sends one CSV telemetry record. The implementation rate-limits nothing;
// callers should choose a reasonable sample rate for the control loop.
void telemetrySend(uint32_t timestamp_ms,
                   float angle,
                   float angle_rate,
                   float target_angle,
                   float motor_speed,
                   int16_t pwm_angle,
                   int16_t pwm_speed,
                   int16_t pwm_output,
                   int32_t encoder_left,
                   int32_t encoder_right);

#endif
