import afl_renode
import ctypes
import time

from afl_renode import INFD, sysbus_name, log_basic_block, read, monitor, visited, STATUS_ABORT, STATUS_SUCCESS


IDLE_COUNT = 10
COMMAND_BEFORE_REBOOT = 1
DATA_SIZE = 50
CYCLES_TO_BOOT = 150

data = ctypes.create_string_buffer(DATA_SIZE)
end = ctypes.create_string_buffer(1)
end.value = '\r'.encode()
eof = False

booted = False
sent_commands = 0
FIRST_RUN = True

def quantum_hook(mach):
    global eof, cntr, skip, sent_commands, booted, FIRST_RUN
   
#    if len(visited) < IDLE_COUNT:
    WriteChar = mach["sysbus.usart2"].WriteChar
    n = read(INFD, data, DATA_SIZE)
    for byte in bytearray(data.raw[:n]):
        WriteChar(byte)
    for byte in bytearray(end.raw[:n]):
        WriteChar(byte)
    sent_commands += 1
    if n == 0:
        if not eof:
            WriteChar(0x0a)
            eof = True
            return
        eof = False
        if len(afl_renode.visited) <= 1:
            afl_renode.status = STATUS_ABORT
            sent_commands = 0
        else:
            afl_renode.status = STATUS_SUCCESS
#            if (sent_commands > 10):
#                sent_commands = 0
#                monitor.Machine.Reset()
#                time.sleep(0.4)

afl_renode.quantum_hook = quantum_hook

