"""
This script is used in order to read the prediction results from the serial port and send them
to InfluxDB.

ONLY FOR TESTING PURPOSES
"""

import serial
import re
import json
from datetime import datetime, timedelta

# Modify these variables according to the serial port in use and to the baudrate needed
serial_port = 'COM6'
baudrate = 19200

ser = serial.Serial(port=serial_port, baudrate=baudrate)  # open serial port
print(ser.name)

# enter here the number of prediction to record
pr_n = 3

print('Predictions recording started...')
# this is the string that is needed to wipe out in order to obtain the prediction
startStringPrediction = 'Prediction result: '
startStringMean = 'Mean: '
startStringTemp = 'Temperature reading: '
startStringPres = 'Pressure reading: '
temperature = ''
i = 0
now = datetime.now()
while i < pr_n:
    reading = ser.readline().decode('utf-8')
    if reading.startswith(startStringTemp):
        temperature = str('%.2f'%(float(re.sub(startStringTemp + '|\n', '', reading))))
    if reading.startswith(startStringPres):
        # first the board print the temperature, then it prints the pressure.
        pressure = str('%.2f'%(float(re.sub(startStringPres + '|\n', '', reading))))
        now = str(int(datetime.now().timestamp()))
        str_to_send = 'es-milano ' + 'temp=' + temperature + ' pres=' + pressure + ' ' + now
        print(str_to_send)
    if reading.startswith(startStringMean):
        now = datetime.now()
        pressureMean = str('%.2f'%(float(re.sub(startStringMean + '|\n', '', reading))))
        str_to_send = 'es-milano ' + 'pres-mean=' + pressureMean + ' ' + str(int(now.timestamp()))
        print(str_to_send)
    if reading.startswith(startStringPrediction):
        pred_time = now + timedelta(hours=8)
        pressurePred = str('%.2f'%(float(re.sub(startStringPrediction + '|\n', '', reading))))
        str_to_send = 'es-milano ' + 'pres-pred=' + pressurePred + ' ' + str(int(pred_time.timestamp()))
        print(str_to_send)
        i += 1

print('Predictions recording ended.')
