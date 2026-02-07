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
    #  Setup
    driver = rmd.CanDriver(interface_name)
    actuator = rmd.ActuatorInterface(driver, motor_id)
    print(f"Connected to Motor {motor_id} on {interface_name}")
    print(f"Starting Sine Wave: Amp={amplitude_deg} deg, Freq={frequency_hz} Hz")
    print("Press Ctrl+C to stop.")

    start_time = time.time()

    # Continuous Loop
    while True:
        # Calculate Time
        t_now = time.time() - start_time
        omega = 2 * math.pi * frequency_hz  # Angular frequency (rad/s)

        # Calculate Target State
        # Position: p(t) = A * sin(omega * t)
        p_des = amplitude_deg * math.sin(omega * t_now)

        # Send Command
        status = actuator.sendPositionAbsoluteSetpoint(p_des, X6_60.rated_speed * 6)

        # Print Status
        # Comparison: Target vs Actual
        target_deg = p_des
        actual_deg = status.shaft_angle
        actual_vel = status.shaft_speed
        actual_current = status.current

        print(
            f"Target: {target_deg:6.2f} deg | Actual: {actual_deg:6.2f} deg | Actual Vel: {actual_vel:6.2f} deg/s | Current: {actual_current:6.2f} A"
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
