import serial
import re
from datetime import datetime, timedelta
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS
from data_switcher.secret import Secret

client = InfluxDBClient(url="https://eu-central-1-1.aws.cloud2.influxdata.com", token=Secret.token)

# Modify these variables according to the serial port in use and to the baudrate needed
serial_port = 'COM3'
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

f = open("exception-logger.log", "a")

print('Predictions recording started...')

while 1:
    try:
        reading = ser.readline().decode('utf-8')
        if reading.startswith(startStringTemp):
            temperature = str('%.2f' % (float(re.sub(startStringTemp + '|\n', '', reading))))
        if reading.startswith(startStringPres):
            now = datetime.now().timestamp()
            # first, the board print the temperature, then it prints the pressure.
            pressure = str('%.2f' % (float(re.sub(startStringPres + '|\n', '', reading))))
            point = Point(measurement).field("temp", float(temperature)).field("pres", float(pressure)).time(int(now*1_000_000_000), WritePrecision.NS)
            print(point.to_line_protocol())
            client.write_api(write_options=SYNCHRONOUS).write("es-presentation", "10528091@polimi.it", record=point)

        if reading.startswith(startStringMean):
            now = datetime.now()
            pressureMean = str('%.2f' % (float(re.sub(startStringMean + '|\n', '', reading))))
            point = Point(measurement).field("pres-mean", float(pressureMean)).time(int(now.timestamp()*1_000_000_000), WritePrecision.NS)
            print(point.to_line_protocol())
            client.write_api(write_options=SYNCHRONOUS).write("es-presentation", "10528091@polimi.it", record=point)

        if reading.startswith(startStringPrediction):
            now = datetime.now()
            pred_time = now + timedelta(hours=7, minutes=47, seconds=36)
            pressurePred = str('%.2f' % (float(re.sub(startStringPrediction + '|\n', '', reading))))
            point = Point(measurement).field("pres-pred", float(pressurePred)).time(int(pred_time.timestamp()*1_000_000_000), WritePrecision.NS)
            print(point.to_line_protocol())
            client.write_api(write_options=SYNCHRONOUS).write("es-presentation", "10528091@polimi.it", record=point)
    except Exception as e:
        print(e)
        f.write("Time and Date : " + str(datetime.now()) + "\n")
        f.write("Exception : " + str(e) + "\n")
        f.flush()
        continue