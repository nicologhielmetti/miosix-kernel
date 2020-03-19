# import serial
# import re
from datetime import datetime, timedelta
import mem_profiling.MemoryUsage as mu

# Modify these variables according to the serial port in use and to the baudrate needed
# serial_port = 'COM3'
# baudrate = 19200
#
#
# ser = serial.Serial(port=serial_port, baudrate=baudrate)  # open serial port
# print(ser.name)

methods = {}
populate = False
method = ''
cnt = 0
with open("profiling_main.log", "r") as file:
    for line in file:
        try:
            if 'FINISH!' in line:
                break

            if 'BEFORE' in line:
                populate = True
                method = line.split(',')[1].replace('\n', '')
                if method in methods:
                    method = method + str(cnt)
                    cnt = cnt + 1
                methods[method] = mu.MemoryUsage()

            if 'Stack' in line:
                file.readline() # skip Stack size
                line = file.readline() # Used stack
                values = line.split(':')[1].split('/')
                if populate:
                    methods[method].stackUsedCurrent = int(values[0])
                    methods[method].stackUsedMax = int(values[1])
                else:
                    stackUsedCurrent = methods[method].stackUsedCurrent
                    stackUsedMax = methods[method].stackUsedMax
                    methods[method].stackUsedCurrent = int(values[0]) - stackUsedCurrent
                    methods[method].stackUsedMax = int(values[1]) - stackUsedMax

                line = file.readline() # Free stack
                values = line.split(':')[1].split('/')
                if populate:
                    methods[method].stackFreeCurrent = int(values[0])
                    methods[method].stackFreeMin = int(values[1])
                else:
                    stackFreeCurrent = methods[method].stackFreeCurrent
                    stackFreeMax = methods[method].stackFreeMin
                    methods[method].stackFreeCurrent = int(values[0]) - stackFreeCurrent
                    methods[method].stackFreeMin = int(values[1]) - stackFreeMax

            if 'Heap' in line:
                file.readline()  # skip Heap size
                line = file.readline()  # Used heap
                values = line.split(':')[1].split('/')
                if populate:
                    methods[method].heapUsedCurrent = int(values[0])
                    methods[method].heapUsedMax = int(values[1])
                else:
                    heapUsedCurrent = methods[method].heapUsedCurrent
                    heapUsedMax = methods[method].heapUsedMax
                    methods[method].heapUsedCurrent = int(values[0]) - heapUsedCurrent
                    methods[method].heapUsedMax = int(values[1]) - heapUsedMax

                line = file.readline()  # Free heap
                values = line.split(':')[1].split('/')
                if populate:
                    methods[method].heapFreeCurrent = int(values[0])
                    methods[method].heapFreeMin = int(values[1])
                    populate = False
                else:
                    heapFreeCurrent = methods[method].heapFreeCurrent
                    heapFreeMin = methods[method].heapFreeMin
                    methods[method].heapFreeCurrent = int(values[0]) - heapFreeCurrent
                    methods[method].heapFreeMin = int(values[1]) - heapFreeMin

        except Exception as e:
            print(e)
            continue


out = open("deltas_main.csv", "w")
out.write("methodCalled,stackUsedCurrentDelta,stackUsedMaxDelta,stackFreeCurrentDelta,stackFreeMinDelta," +
          "heapUsedCurrentDelta,heapUsedMaxDelta,heapFreeCurrentDelta,heapFreeMinDelta\n")
for method in methods:
    memoryUsage = methods[method]
    out.write(method + ',' + memoryUsage.toCSVLine() + '\n')
    out.flush()
