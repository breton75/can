#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import re
import sys

if len(sys.argv) > 1:
    # bitrate
    cmd = "sudo ip link set " + sys.argv[1] + " type can bitrate 125000"
    vid = os.popen(cmd)
    for line in vid.readlines():
        print line

    # up
    cmd = "sudo ip link set " + sys.argv[1] + " up"
    vid = os.popen(cmd)
    for line in vid.readlines():
        print line

else:
    print "device name not defined"
    

