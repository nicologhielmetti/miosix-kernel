| ![miosixlogo](https://user-images.githubusercontent.com/16907319/74107162-a0d42e80-4b6d-11ea-979e-c17000bc936c.png)  | 
![1024px-STMicroelectronics svg](https://user-images.githubusercontent.com/16907319/74107182-d24cfa00-4b6d-11ea-91e6-96fa5fa9dcee.png) | ![keras](https://user-images.githubusercontent.com/16907319/74107160-9e71d480-4b6d-11ea-94a7-b82f26f1dc05.png) | ![Tensorflow_logo svg](https://user-images.githubusercontent.com/16907319/74107157-9a45b700-4b6d-11ea-9743-648a312c5d97.png) | ![Influxdb_logo svg](https://user-images.githubusercontent.com/16907319/74490550-27b83c80-4ec9-11ea-8f50-662fc363a603.png)|
| ------------- | ------------- | ------------- | ------------- | ------------- |

# Predicting Atmospheric Pressure using LSTM Neural Network on STM32-NucleoF401RE and Miosix RTOS

The goal of this project is to embed an LSTM Neural Network produced by MXCubeAI tool of STMicroelectronics within an RTOS for microcontroller. As a use-case for the project it has been considered the prediction of atmosferic pressure trained with a dataset taken from a certified weather station (http://meteovalmorea.it/). After the deployment, to get the prediction from the neural network, it is considered pressure data taken from LPS22HB pressure sensor installed on IKS01A2 MEMS sensors board. For LSTM neural network implementation details consider [this](https://github.com/francescoalongi/Atm_pressure_predictor) link. For a more precise presentation of the project please refer to [this]() paper.

## Tools
* [NetbeansIDE](https://netbeans.org/): used for modifying and compiling the Miosix Kernel;
* [PyCharm](https://www.jetbrains.com/pycharm/): used for the neural network implementation and training;
* [Miosix RTOS](https://miosix.org/): OS used for thread implementation and synchronization which runs on the considered board;
* [X-CUBE-AI](https://www.st.com/en/embedded-software/x-cube-ai.html): tool used to convert the `.h5` file obtained from Keras to a STM32-optimized library.

## Main solution design
![sequence](https://user-images.githubusercontent.com/16907319/74106821-21912b80-4b6a-11ea-9184-b63f8d23454f.png)

## System test performance
![es_performance](https://user-images.githubusercontent.com/16907319/74106850-864c8600-4b6a-11ea-88d8-310afa679bd6.png)
![valmo](https://user-images.githubusercontent.com/16907319/74490598-4fa7a000-4ec9-11ea-8f94-7998ae710db3.png)
![es-influxdb](https://user-images.githubusercontent.com/16907319/74490626-71a12280-4ec9-11ea-8bdd-1b00e1f2b883.png)

### For debugging using NetBeansIDE
* install gdbserver plugin for NetBeansIDE
* paste the following [configuration file](https://github.com/arduino-org/OpenOCD/blob/master/tcl/board/st_nucleo_f401re.cfg) 
  to the following location: miosix-kernel/miosix/arch/cortexM4_stm32f4/stm32f401re_nucleo/
* run: openocd -f miosix-kernel/miosix/arch/cortexM4_stm32f4/stm32f401re_nucleo/stm32f401re_nucleo.cfg
* within NetBeansIDE: Debug, Attach Debug : select "gdbserver" and as target "ext :3333", OK
