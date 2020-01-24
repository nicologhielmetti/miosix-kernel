"""
This script is used in order to read the prediction results from the serial port and write them
in a csv file together with the timestamp at which the prediction was issued.

ONLY FOR TESTING PURPOSES
"""

import serial
import re
from datetime import datetime

# Modify these variables according to the serial port in use and to the baudrate needed
serial_port = 'COM6'
baudrate = 19200

ser = serial.Serial(port=serial_port, baudrate=baudrate)  # open serial port
print(ser.name)

data_output = open("Predictions.csv", "w+")

#write the header
data_output.write('datetime,prediction\n')

# enter here the number of prediction to record
pr_n = 20

print('Predictions recording started...')
# this is the string that is needed to wipe out in order to obtain the prediction
startString = 'Prediction result: '
i = 0
while i < pr_n:
    reading = ser.readline().decode('utf-8')
    if reading.startswith(startString):
        now = datetime.now()
        string_to_write = str(now) + ',' + str(re.sub(startString, '', reading))
        data_output.write(string_to_write)
        print('Recorded: ' + string_to_write)
        i += 1

print('Predictions recording ended.')
data_output.close()