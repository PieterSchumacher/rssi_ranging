import serial
import re
import sqlite3

DB_PATH = '/home/pi/rssi_ranging/rssi.db'

conn = sqlite3.connect(DB_PATH)
c = conn.cursor()
dist = "dist:"
rssi = "RSSI:"
snr = "SNR:"
d,r,s=-1,-1,-1
#print("Provide the path of serial port: ")
#path = input()
ser = serial.Serial("/dev/ttyACM0", 9600)
while 1:
    if(ser.in_waiting > 0):
        line = ser.readline().decode("utf-8")
        line = line[:len(line)-2]
        m = re.match(dist, line)
        if m is not None: d = 4.0
        m = re.match(rssi, line)
        if m is not None: r = -int(line[m.end()::])
        m = re.match(snr, line)
        if m is not None: s = -int(line[m.end()::])
        if d != -1 and r != -1 and s != -1:
            print("done ", (d,r,s,))
            c.execute('INSERT INTO exp1 (Dist,RSSI,SNR) VALUES (?,?,?)', (d,r,s))
            conn.commit()
            d,r,s=-1,-1,-1