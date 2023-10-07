#!/bin/bash
sudo echo ""
sudo openocd -f ti_ek-tm4c123gxl.cfg &> /dev/null &
openocd_pid=$!

gdb-multiarch -ex "file objs/lab2binary.axf" -ex "target extended-remote localhost:3333" -ex "b main" -ex "layout split" -ex "mon reset halt" -ex "ni" -tui

kill $openocd_pid
