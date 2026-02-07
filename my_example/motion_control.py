import myactuator_rmd_py as rmd
from myactuator_rmd_py.actuator_constants import X6_60
import math
import time

dtr = math.pi / 180.0  # Degrees to Radians

# Configuration
interface_name = "can2"
motor_id = 1

# Sine Wave Settings
amplitude_deg = 45.0
frequency_hz = 1


try:
    # 1. Setup
    driver = rmd.CanDriver(interface_name)
    actuator = rmd.ActuatorInterface(driver, motor_id)
    print(f"Connected to Motor {motor_id} on {interface_name}")

    # 2. Control Gains
    kp = 15  # Stiffness (Increase slightly for tracking, but start low)
    kd = 1  # Damping (To prevent oscillation)
    t_ff = 0.0  # Feedforward Torque

    print(f"Starting Sine Wave: Amp={amplitude_deg} deg, Freq={frequency_hz} Hz")
    print("Press Ctrl+C to stop.")

    start_time = time.time()

    # 3. Continuous Loop
    while True:
        # A. Calculate Time
        t_now = time.time() - start_time
        omega = 2 * math.pi * frequency_hz  # Angular frequency (rad/s)

        # B. Calculate Target State
        # Position: p(t) = A * sin(omega * t)
        p_des = (amplitude_deg * dtr) * math.sin(omega * t_now)

        # Velocity: v(t) = A * omega * cos(omega * t)
        v_des = (amplitude_deg * dtr) * omega * math.cos(omega * t_now)

        # C. Send Command
        status = actuator.motionControl(p_des, v_des, kp, kd, t_ff)

        # D. Print Status
        # Comparison: Target vs Actual
        target_deg = p_des / dtr
        target_vel = v_des / dtr
        actual_deg = status.shaft_angle / dtr
        actual_vel = status.shaft_speed / dtr
        actual_torque = status.torque

        print(
            f"Target: {target_deg:6.2f} deg | Target Vel: {target_vel:6.2f} deg/s | Actual: {actual_deg:6.2f} deg | Actual Vel: {actual_vel:6.2f} deg/s | Torque: {actual_torque:6.2f} Nm"
        )

        # Rate Limiting (100Hz)
        time.sleep(0.01)

except KeyboardInterrupt:
    print("\n[!] Ctrl+C Detected. Stopping Motor...")

    # 4. Safe Shutdown
    try:
        actuator.shutdownMotor()
        print("Motor Stopped Safely.")
    except Exception as e:
        print(f"Failed to shutdown motor: {e}")

except Exception as e:
    print(f"Error: {e}")
