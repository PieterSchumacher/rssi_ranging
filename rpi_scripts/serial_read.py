import serial
print("Provide the path of serial port: ")
path = input()
ser = serial.Serial(path, 9600)
while 1:
    if(ser.in_waiting >0):
        line = ser.readline()
        print(line)