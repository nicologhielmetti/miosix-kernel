For debugging using NetBeansIDE

- install gdbserver plugin for NetBeansIDE
- paste the following configuration file (https://github.com/arduino-org/OpenOCD/blob/master/tcl/board/st_nucleo_f401re.cfg) 
  at the following location -> miosix-kernel/miosix/arch/cortexM4_stm32f4/stm32f401re_nucleo/
- run -> openocd -f miosix-kernel/miosix/arch/cortexM4_stm32f4/stm32f401re_nucleo/stm32f401re_nucleo.cfg
- within NetBeansIDE -> Debug -> Attach Debug : select "gdbserver" and as target "ext :3333" -> OK
