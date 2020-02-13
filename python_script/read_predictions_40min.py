"""
This script is used in order to read the prediction results from the serial port, write them
in a csv file together with the timestamp at which the prediction was issued and send them to
an influxdb instance via line protocol.

ONLY FOR TESTING PURPOSES
"""

import serial
import re
from influxdb_client import InfluxDBClient, Point, WritePrecision
from datetime import datetime, timedelta

from secret import Secret

client = InfluxDBClient(url="https://eu-central-1-1.aws.cloud2.influxdata.com", token=Secret.token, debug=True)

# Modify these variables according to the serial port in use and to the baudrate needed
serial_port = '/dev/ttyACM1'
baudrate = 19200

ser = serial.Serial(port=serial_port, baudrate=baudrate)  # open serial port
print(ser.name)

# this is the string that is needed to wipe out in order to obtain the prediction
startStringPrediction = 'Prediction result: '
startStringMean = 'Mean: '
startStringTemp = 'Temperature reading: '
startStringPres = 'Pressure reading: '
temperature = ''
measurement = 'es-project'

print('Predictions recording started...')

while 1:
    reading = ser.readline().decode('utf-8')
    if reading.startswith(startStringTemp):
        temperature = str('%.2f' % (float(re.sub(startStringTemp + '|\n', '', reading))))
    if reading.startswith(startStringPres):
        now = datetime.now().timestamp()
        # first, the board print the temperature, then it prints the pressure.
        pressure = str('%.2f' % (float(re.sub(startStringPres + '|\n', '', reading))))
        str_to_send = measurement + ' ' + 'temp=' + temperature + ' pres=' + pressure + ' ' + str(int(now))
        print(str_to_send)
        point = Point(measurement).field("temp", temperature).field("pres", pressure).time(int(now), WritePrecision.S)
        client.write_api().write("es-presentation", "10528091@polimi.it", record=point.to_line_protocol())

    if reading.startswith(startStringMean):
        now = datetime.now()
        pressureMean = str('%.2f' % (float(re.sub(startStringMean + '|\n', '', reading))))
        str_to_send = measurement + ' ' + 'pres-mean=' + pressureMean + ' ' + str(int(now.timestamp()))
        print(str_to_send)
        point = Point(measurement).field("pres-mean", pressureMean).time(int(now.timestamp()), WritePrecision.S)
        client.write_api().write("es-presentation", "10528091@polimi.it", record=point.to_line_protocol())

    if reading.startswith(startStringPrediction):
        now = datetime.now()
        pred_time = now + timedelta(minutes=40)
        pressurePred = str('%.2f' % (float(re.sub(startStringPrediction + '|\n', '', reading))))
        str_to_send = measurement + ' ' + 'pres-pred=' + pressurePred + ' ' + str(int(pred_time.timestamp()))
        print(str_to_send)
        point = Point(measurement).field("pres-mean", pressurePred).time(int(pred_time.timestamp()), WritePrecision.S)
        client.write_api().write("es-presentation", "10528091@polimi.it", record=point.to_line_protocol())
