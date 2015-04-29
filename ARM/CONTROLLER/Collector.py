#!/usr/bin/python3

import sys
from serial import Serial
from time import sleep

def GetTimeStamp(serialport): #Wait for 4 charaters from serial, add them and return the value
  recieved = []
  while len(recieved) < 4:
    while serialport.inWaiting() > 0 and len(recieved) < 4:
      recieved.append(ord(serialport.read(1)))

  return (recieved[0] << 24) + (recieved[1] << 16) + (recieved[2] << 8) + (recieved[3])


tty = sys.argv[1]
baud = int(sys.argv[2])
serialport = Serial(port = tty, baudrate=baud)
#Flush input queue
sleep(.1)
serialport.flushInput()


while True:
  this = GetTimeStamp(serialport)
  binstr = str(bin(this))[2:]
  while len(binstr) < 32:
      binstr = '0' + binstr
  print (binstr[0:8] + " " + binstr[8:16] + " " + binstr[16:24] + " " + binstr[24:32])
