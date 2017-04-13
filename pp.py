#!/usr/bin/python

import os
import re
import sys

gg= open("/home/user/proj/hh1", "w")
gg.write("00\n")
gg.close()

can = ""
cmd = ""

#if len(sys.argv) > 1:
#    print sys.argv[1]
#    
#    cmd = sys.argv[1]

   
#    print cmd

# can0 bitrate
cmd = "sudo ip link set can0 type can bitrate 125000"

gg= open("/home/user/proj/hh1", "a")
gg.write("11\n")
gg.close()

vid = os.popen(cmd)

gg= open("/home/user/proj/hh1", "a")
gg.write("22\n")


for line in vid.readlines():
    gg.write(line)
    print line

gg.close()


# can0 up
cmd = "sudo ip link set can0 up"
vid1 = os.popen(cmd)

gg= open("/home/user/proj/hh1", "a")
gg.write("33\n")


for line in vid1.readlines():
    gg.write(line)
    print line
    
gg.close()


cmd = "ip link show"
vid2 = os.popen(cmd)

gg= open("/home/user/proj/hh1", "a")
gg.write("44\n")


for line in vid2.readlines():
    gg.write(line)
    print line
    
gg.close()
    
### can1 bitrate
##cmd = "ip link set can1 type can bitrate 125000"
##vid = os.popen(cmd)
##gg.write("22")
##for line in vid.readlines():
##    print line
##
### can0 up
##cmd = "ip link set can0 up"
##vid = os.popen(cmd)
##for line in vid.readlines():
##    print line
##
### can1 up
##cmd = "ip link set can1 up"
##vid = os.popen(cmd)
##for line in vid.readlines():
##    print line
