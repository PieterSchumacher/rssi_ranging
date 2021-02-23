import serial
import re
import sqlite3

DB_PATH = '/home/pi/rssi_ranging/rssi.db'

conn = sqlite3.connect(DB_PATH)
c = conn.cursor()
dist = "message content:"
rssi = "RSSI:"
snr = "SNR:"
d,r,s=-1,-1,-1
#print("Provide the path of serial port: ")
#path = input()
ser = serial.Serial("/dev/ttyACM0", 9600)
while 1:
    try:
        if(ser.in_waiting > 0):
            line = ser.readline().decode("utf-8")
            #print(line)
            line = line[:len(line)-2]
            print(line)
            m = re.match(dist, line)
            if m is not None:
                d = (line[m.end()::]).split(";")
                print(d)
            m = re.match(rssi, line)
            if m is not None: r = float(line[m.end()::])
            m = re.match(snr, line)
            if m is not None: s = float(line[m.end()::])
            if r != -1:
                print("done ", (d[0],r,d[1],d[2],d[3],))
                c.execute('INSERT INTO exp3 (Dist,RSSI,FLAT,FLONG,FREQ) VALUES (?,?,?,?,?)', (d[0],r,d[1],d[2],d[3]))
                conn.commit()
                d,r,s=-1,-1,-1
    except:
        pass