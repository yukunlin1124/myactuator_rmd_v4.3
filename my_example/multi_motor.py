import sys
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.abspath(os.path.join(current_dir, '../build'))
sys.path.insert(0, build_dir)

import myactuator_rmd_py as rmd
import math
import time

# Initialization
driver = rmd.CanDriver("can0")
motor1_id = 1
motor2_id = 2

# Sine Wave Settings
amplitude_deg = 90.0
frequency_hz = 1


try:
    # 1. Setup
    driver = rmd.CanDriver(interface_name)
    actuator1 = rmd.ActuatorInterface(driver, motor1_id)
    actuator2 = rmd.ActuatorInterface(driver, motor2_id)

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
        status1 = actuator1.sendMotionModeSetpoint(p_des, v_des, kp, kd, t_ff)
        status2 = actuator2.sendMotionModeSetpoint(p_des, v_des, kp, kd, t_ff)


        # Rate Limiting (100Hz)
        time.sleep(0.01)

except KeyboardInterrupt:
    print("\n[!] Ctrl+C Detected. Stopping Motor...")

    # 4. Safe Shutdown
    try:
        # actuator1.stopAllMotors()
        actuator1.shutdownAllMotors()
        print("Motor Stopped Safely.")
    except Exception as e:
        print(f"Failed to shutdown motor: {e}")

except Exception as e:
    print(f"Error: {e}")