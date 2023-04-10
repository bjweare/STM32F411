file "./build/DevEBox.elf"

# Connect to texane stlink gdb server
target extended-remote :3333
# Or, connect to openOCD instead
# target remote localhost:3333

# monitor reset init
# monitor halt

# Uncomment to enable semihosting
# monitor arm semihosting enable

# Flash program and load symbols
load
break main
#break StartDefaultTask
#break HAL_UART_Transmit
break dev_ebox.c:handleKeyInterrupt
break timer.c:initTimer11
break timer.c:handleTimerUpdate

# Run to main (first breakpoint)
continue
