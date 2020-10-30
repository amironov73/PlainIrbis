@echo off

set DOS4G=quiet
if not exist obj mkdir obj
wmake -h -f ow_16bit.mak