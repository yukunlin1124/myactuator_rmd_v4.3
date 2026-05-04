import sys
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.abspath(os.path.join(current_dir, '../build'))
sys.path.insert(0, build_dir)

import myactuator_rmd_py as rmd

# Initialization
driver = rmd.CanDriver("can2")
actuator = rmd.ActuatorInterface(driver, 1)

gain = actuator.getSingleGain(rmd.actuator_state.GainType.POSITION_LOOP_KD)

print(f"Gain = {gain}")
