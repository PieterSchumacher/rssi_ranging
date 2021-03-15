import sqlite3
import serial
import struct
import time
DB_PATH = '/home/pi/rssi_ranging/rssi.db'

conn = sqlite3.connect(DB_PATH)
c = conn.cursor()
ser = serial.Serial("/dev/ttyACM0", 9600, timeout=.1)
ser.setDTR(1)
size = struct.calcsize('fffB');
float_size = struct.calcsize('ff');
while 1:
    try:
        if ser.in_waiting > 0:
            a_loc = struct.unpack('ff', ser.read(float_size))
            config = struct.unpack('fffB', ser.read(size))
            time.sleep(.1)
            current_millis = time.time()
            while time.time() - current_millis < 0.6:
                try:
                    if ser.in_waiting > 0:
                        data = struct.unpack('hh', ser.read(4))
                        current_millis = time.time()
                        c.execute('INSERT INTO exp4 (RSSI,SNR,T_FLAT,T_FLONG,A_FLAT,A_FLONG,FREQ,SF) VALUES (?,?,?,?,?,?,?,?)',
                                                    (data[0], data[1], config[1], config[2], a_loc[0], a_loc[1], config[0], config[3]))
                except:
                    print("error in data packets")
            conn.commit()
    except:
        print("error in config packets")