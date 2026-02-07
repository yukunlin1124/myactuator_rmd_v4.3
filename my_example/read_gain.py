# -*- coding: gbk -*-
import myactuator_rmd_py as rmd

# Initialization
driver = rmd.CanDriver("can2")
actuator = rmd.ActuatorInterface(driver, 1)

gain = actuator.getSingleGain(rmd.actuator_state.GainType.POSITION_LOOP_KD)

print(f"Gain = {gain}")
