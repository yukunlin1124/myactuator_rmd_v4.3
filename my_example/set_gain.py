import sys
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.abspath(os.path.join(current_dir, '../build'))
sys.path.insert(0, build_dir)

import myactuator_rmd_py as rmd

# Initialization
driver = rmd.CanDriver("can2")
actuator = rmd.ActuatorInterface(driver, 1)

gain_type = rmd.actuator_state.GainType.POSITION_LOOP_KP

original_gain = actuator.getSingleGain(gain_type)
new_gain = actuator.setSingleGainPersistently(gain_type, 0.3)

print(f"Original gain = {original_gain}")
print(f"New gain = {new_gain}")
