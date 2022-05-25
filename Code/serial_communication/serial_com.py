import serial
import sys

if len(sys.argv) == 3:
	comport  = sys.argv[1]
	baudRate = sys.argv[2]
else:
	comport  = sys.argv[1]
	# Setup the default baudrate.
	baudRate = 115200

ser = serial.Serial()
ser.baudrate = baudRate
ser.port = comport
ser.timeout = 0.5
ser.open()
#ser.flushInput()

for i in range(0,50):
    ser_bytes = ser.readline()

    print(str(ser_bytes))

ser.close()
    