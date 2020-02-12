"""
This script is used in order to read the prediction results from the serial port, write them
in a csv file together with the timestamp at which the prediction was issued and send them to
an influxdb instance via line protocol.

ONLY FOR TESTING PURPOSES
"""

import serial
import re
from datetime import datetime, timedelta

# Modify these variables according to the serial port in use and to the baudrate needed
serial_port = 'COM6'
baudrate = 19200

ser = serial.Serial(port=serial_port, baudrate=baudrate)  # open serial port
print(ser.name)

# this is the string that is needed to wipe out in order to obtain the prediction
startStringPrediction = 'Prediction result: '
startStringMean = 'Mean: '
startStringTemp = 'Temperature reading: '
startStringPres = 'Pressure reading: '
temperature = ''
measurement = 'es-valmorea'

print('Predictions recording started...')

while 1:
    reading = ser.readline().decode('utf-8')
    now = str(int(datetime.now().timestamp()))
    if reading.startswith(startStringTemp):
        temperature = str('%.2f' % (float(re.sub(startStringTemp + '|\n', '', reading))))
    if reading.startswith(startStringPres):
        # first, the board print the temperature, then it prints the pressure.
        pressure = str('%.2f' % (float(re.sub(startStringPres + '|\n', '', reading))))
        str_to_send = measurement + ' ' + 'temp=' + temperature + ' pres=' + pressure + ' ' + now
        print(str_to_send)

    if reading.startswith(startStringMean):
        now = datetime.now()
        pressureMean = str('%.2f' % (float(re.sub(startStringMean + '|\n', '', reading))))
        str_to_send = measurement + ' ' + 'pres-mean=' + pressureMean + ' ' + str(int(now.timestamp()))
        print(str_to_send)

    if reading.startswith(startStringPrediction):
        pred_time = now + timedelta(hours=8)
        pressurePred = str('%.2f' % (float(re.sub(startStringPrediction + '|\n', '', reading))))
        str_to_send = measurement + ' ' + 'pres-pred=' + pressurePred + ' ' + str(int(pred_time.timestamp()))
        print(str_to_send)

