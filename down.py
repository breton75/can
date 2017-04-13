#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import re
import sys

if len(sys.argv) > 1:
    cmd = "sudo ip link set " + sys.argv[1] + " down"
    vid = os.popen(cmd)
    for line in vid.readlines():
        print line
        
else:  #если нет параметров, то останавливаем все порты
    
    # can0 down
    cmd = "sudo ip link set can0 down"
    vid = os.popen(cmd)
    for line in vid.readlines():
        print line

    # can1 down
    cmd = "sudo ip link set can1 down"
    vid = os.popen(cmd)
    for line in vid.readlines():
        print line
