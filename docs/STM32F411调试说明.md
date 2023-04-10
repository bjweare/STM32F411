## 

```bash
openocd -f interface/cmsis-dap.cfg -f target/stm32f4x.cfg

openocd -f interface/cmsis-dap.cfg -f target/stm32f4x.cfg -c "program /Users/chenzw/PersonalApp/stm32f4/build/DevEBox.bin 0x08000000 verify exit"

```

```bash
telnet localhost 4444
program /Users/chenzw/PersonalApp/dev_ebox/build/DevEBox.bin 0x08000000 verify
read_memory 0x00000058 32 1
read_memory 0x40020800 32 1
write_memory 0x40020800 32 {0x4000000}
```

## GDB Debugging
`.gdbinit`
```bash
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

# Run to main (first breakpoint)
continue

print/x *((uint32_t *)0x00000058)
(gdb) print (*((uint32_t *)0xE000E100) >> 26) & 1
monitor reset init
```
