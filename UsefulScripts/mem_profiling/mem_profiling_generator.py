import serial
import re
from datetime import datetime, timedelta

# Modify these variables according to the serial port in use and to the baudrate needed
serial_port = 'COM3'
baudrate = 19200

ser = serial.Serial(port=serial_port, baudrate=baudrate)  # open serial port
print(ser.name)

f = open("mem_profile.log", "a")

print('--- PROFILING STARTED ---')

while 1:
    try:
        print('--- WAITING FOR *Starting Kernel... Ok* STRING ---')
        while ser.readline().decode('utf-8') != "Starting Kernel... Ok":
            continue
        #if ser.readline().decode('utf-8') == "":
        #if ser.readline().decode('utf-8') == "":
        #if ser.readline().decode('utf-8') == "":

    except Exception as e:
        print(e)
        f.write("Time and Date : " + str(datetime.now()) + "\n")
        f.write("Exception : " + str(e) + "\n")
        f.flush()
        continue