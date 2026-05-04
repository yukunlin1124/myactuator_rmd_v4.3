import sys
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.abspath(os.path.join(current_dir, '../build'))
sys.path.insert(0, build_dir)

import myactuator_rmd_py as rmd

print(rmd.actuator_constants.X6_60.rated_speed)
