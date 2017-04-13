#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import re
import sys

if len(sys.argv) == 1:
    cmd = "sudo ip link show"

else:
    cmd = "sudo ip link show " + sys.argv[1]

vid = os.popen(cmd)

for line in vid.readlines():
    print line
    
