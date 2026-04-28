#pragma once

namespace core {
    namespace math {
        struct SRotator {
            float pitch = 0.0f;
            float yaw = 0.0f;
            float roll = 0.0f;

            SRotator() = default;

            explicit SRotator(float value)
                : pitch(value), yaw(value), roll(value)
            {
            }

            SRotator(float inPitch, float inYaw, float inRoll)
                : pitch(inPitch), yaw(inYaw), roll(inRoll)
            {
            }

            SRotator operator+(float value) const {
                return SRotator(
                    pitch + value,
                    yaw + value,
                    roll + value
                );
            }

            SRotator operator*(float value) const {
                return SRotator(
                    pitch * value,
                    yaw * value,
                    roll * value
                );
            }

            SRotator operator+(const SRotator& other) const {
                return SRotator(
                    pitch + other.pitch,
                    yaw + other.yaw,
                    roll + other.roll
                );
            }

            SRotator operator*(const SRotator& other) const {
                return SRotator(
                    pitch * other.pitch,
                    yaw * other.yaw,
                    roll * other.roll
                );
            }

            SRotator& operator+=(float value) {
                pitch += value;
                yaw += value;
                roll += value;

                return *this;
            }

            SRotator& operator*=(float value) {
                pitch *= value;
                yaw *= value;
                roll *= value;

                return *this;
            }

            SRotator& operator+=(const SRotator& other) {
                pitch += other.pitch;
                yaw += other.yaw;
                roll += other.roll;

                return *this;
            }

            SRotator& operator*=(const SRotator& other) {
                pitch *= other.pitch;
                yaw *= other.yaw;
                roll *= other.roll;

                return *this;
            }
        };
    }
}