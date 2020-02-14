"""
This script is used in order to read the prediction results from the serial port and write them
in a csv file together with the timestamp at which the prediction was issued.

ONLY FOR TESTING PURPOSES
"""

import serial
import re
from datetime import datetime

# Modify these variables according to the serial port in use and to the baudrate needed
serial_port = '/dev/ttyACM1'
baudrate = 19200

ser = serial.Serial(port=serial_port, baudrate=baudrate)  # open serial port
print(ser.name)

data_output_pred = open("Predictions.csv", "w+")
data_output_mean = open("Ground_truth.csv", "w+")

#write the header
data_output_pred.write('datetime,prediction\n')
data_output_mean.write('datetime,mean\n')

# enter here the number of prediction to record
pr_n = 20

print('Predictions recording started...')
# this is the string that is needed to wipe out in order to obtain the prediction
startStringPrediction = 'Prediction result: '
startStringMean = 'Mean: '
i = 0
while i < pr_n:
    reading = ser.readline().decode('utf-8')
    if reading.startswith(startStringPrediction):
        now = datetime.now()
        string_to_write = str(now) + ',' + str(re.sub(startStringPrediction + '|\n', '', reading))
        data_output_pred.write(string_to_write)
        data_output_pred.flush()
        print('Recorded prediction: ' + string_to_write)
        i += 1
    if reading.startswith(startStringMean):
        now = datetime.now()
        string_to_write = str(now) + ',' + str(re.sub(startStringMean + '|\n', '', reading))
        data_output_mean.write(string_to_write)
        data_output_mean.flush()
        print('Recorded mean: ' + string_to_write)

print('Predictions recording ended.')
data_output_pred.close()
data_output_mean.close()
