import sqlite3
import serial
import struct
import time
DB_PATH = '/home/pi/rssi_ranging/rssi_2.db'

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
            print("a_loc: ", a_loc)
            config = struct.unpack('fffB', ser.read(size))
            print("CONFIG: ", config)
            time.sleep(.1)
            current_millis = time.time()
            while time.time() - current_millis < 4.8:
                try:
                    if ser.in_waiting > 0:
                        data = struct.unpack('hh', ser.read(4))
                        print("DATA: ", data)
                        current_millis = time.time()
                        c.execute('INSERT INTO exp_july (RSSI,SNR,T_FLAT,T_FLONG,A_FLAT,A_FLONG,FREQ,SF) VALUES (?,?,?,?,?,?,?,?)',
                                                    (data[0], data[1], round(config[1],6), round(config[2],6), round(a_loc[0],6), round(a_loc[1],6), round(config[0],1), config[3]))
                except:
                    print("error in data packets")
            conn.commit()
    except:
        print("error in config packets")