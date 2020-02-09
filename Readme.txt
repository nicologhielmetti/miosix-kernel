# Predicting Atmospheric Pressure using LSTM Neural Network on STM32-NucleoF401RE and Miosix RTOS

The goal of this project is to embed an LSTM Neural Network produced by MXCubeAI tool of STMicroelectronics within an RTOS for microcontroller. As a use-case for the project it has been considered the prediction of atmosferic pressure trained with a dataset taken from a certified weather station (http://meteovalmorea.it/). After the deployment, to get the prediction from the neural network, it is considered pressure data taken from LPS22HB pressure sensor installed on IKS01A2 MEMS sensors board.

## Tools
* [NetbeansIDE](https://netbeans.org/)
* [PyCharm](https://www.jetbrains.com/pycharm/)
* [Miosix RTOS](https://miosix.org/)
* [X-CUBE-AI](https://www.st.com/en/embedded-software/x-cube-ai.html)

## Main solution design
![sequence](https://user-images.githubusercontent.com/16907319/74106821-21912b80-4b6a-11ea-9184-b63f8d23454f.png)

## System test performance
![es_performance](https://user-images.githubusercontent.com/16907319/74106850-864c8600-4b6a-11ea-88d8-310afa679bd6.png)

### For debugging using NetBeansIDE

* install gdbserver plugin for NetBeansIDE
* paste the following configuration file (https://github.com/arduino-org/OpenOCD/blob/master/tcl/board/st_nucleo_f401re.cfg) 
  at the following location -> miosix-kernel/miosix/arch/cortexM4_stm32f4/stm32f401re_nucleo/
* run -> openocd -f miosix-kernel/miosix/arch/cortexM4_stm32f4/stm32f401re_nucleo/stm32f401re_nucleo.cfg
* within NetBeansIDE -> Debug -> Attach Debug : select "gdbserver" and as target "ext :3333" -> OK
