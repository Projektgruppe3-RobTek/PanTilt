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
  
def makeavg(liste):
  total=0
  for i in liste:
    total+=i
  return total/len(liste)
  
  
tty = sys.argv[1]
baud = int(sys.argv[2])
serialport = Serial(port = tty, baudrate=baud)
#Flush input queue
sleep(.1)
serialport.flushInput()

count = 0
last = 0
Runningavg=[0,0,0,0]
i=0
print("Encoderpuls,Tid(ns),Forskel(ns),Running avg(ns)\n")
NS_PER_CLOCK = 20
First = 0
while True:
  count += 1
  this = GetTimeStamp(serialport)*20
  if(count == 1):
    First = this
  if not ((this-last) > 10000000 or (this-last) < 0):
    Runningavg[i%4] = this - last
    i+=1
    print(str(count) +"," + str(this-First) + "," + str(this-last) + "," + str(makeavg(Runningavg)))
  last = this 
  if count > 200:
    exit(0)

