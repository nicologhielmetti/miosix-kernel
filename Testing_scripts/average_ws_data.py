"""
This script is used in order to perform the average on the dataset obtained from the weather station.
The dataset comes in a .txt file with the following format:

<n_i>   201903312310  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  1016.9
<n_i>   201903312320  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  1016.8
<n_i>   201903312330  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  <n_i>  1017.0
...

Where the <n_i> tag stands for "not interested" in the analysis that is needed to perform.
"""

import re
from datetime import datetime

predictions = open("Predictions.csv", "r")

# Skip header
predictions.readline()
first_pr = predictions.readline()
# This timestamp represent the first one in which we have a prediction
first_timestamp = int(datetime.strptime(re.split(',', first_pr)[0], "%Y-%m-%d %H:%M:%S.%f").timestamp() * 1000)

second_pr = predictions.readline()
second_timestamp = int(datetime.strptime(re.split(',', second_pr)[0], "%Y-%m-%d %H:%M:%S.%f").timestamp() * 1000)

# The FIFO is not filled each 32 seconds, it is slightly less. In 40 minutes, this would lead to a big numerical
# error if not considered. So here the true gap (in milliseconds) between two predictions is calculated.
pr_gap = second_timestamp - first_timestamp

predictions.close()

true_data_input = open("../raw_test_data.txt", "r")
data_output = open("data_average_40min.csv", "w+")
# Write header
data_output.write("datetime,pressure\n")

datetime_index = 1
pressure_measurement_index = 11

first_line = true_data_input.readline()
first_line_splitted = re.split('  +', first_line)
avg_pressure_value = float(first_line_splitted[pressure_measurement_index])
n_values = 1
avg_timestamp = first_timestamp
for line in true_data_input:
    line_splitted = re.split('  +', line)
    timestamp = datetime.strptime(line_splitted[datetime_index],"%Y%m%d%H%M").timestamp() * 1000
    # If the row we are reading is related to a pressure measurement acquired prior to our prediction, discard it
    if timestamp < first_timestamp:
        continue

    # Sometimes, pressure measurements are corrupted and they are not number. In those cases the measurement is simply skipped
    try:
        pressure_measurement = float(line_splitted[pressure_measurement_index])
    except ValueError:
        print("The value measured at time " + str(timestamp) + " cannot be parsed.")
        continue

    # Check whether the timestamp of the pressure measurement taken into account belongs to the same time band,
    # in that case, keep incrementing the avg
    if avg_timestamp + pr_gap > timestamp:
        avg_pressure_value = avg_pressure_value + (pressure_measurement - avg_pressure_value) / n_values
        n_values += 1
    else:
        data_output.write(str(datetime.fromtimestamp(avg_timestamp)) + ',' + str(avg_pressure_value) + '\n')
        avg_pressure_value = pressure_measurement
        avg_timestamp = avg_timestamp + pr_gap
        n_values = 1

true_data_input.close()
data_output.close()




