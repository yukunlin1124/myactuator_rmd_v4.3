# -*- coding: gbk -*-
import myactuator_rmd_py as rmd
import time

# Initialization
original_id = 2
new_id = 1
driver = rmd.CanDriver("can2")
actuator = rmd.ActuatorInterface(driver, original_id)

function_type = rmd.actuator_state.FunctionControlType.SET_CANID

actuator.functionControl(function_type, new_id)

time.sleep(0.1)

actuator = actuator.reset()  # reset to update the actuator ID
