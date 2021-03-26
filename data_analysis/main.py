import sqlite3
import matplotlib.pyplot as plt
import random
import numpy as np
import haversine as hs
import pandas as pd

NB_SAMPLES = 10

avg_rssi = "SELECT AVG(RSSI), T_FLAT, T_FLONG, A_FLAT, A_FLONG FROM exp4 GROUP BY T_FLAT, T_FLONG, A_FLAT, A_FLONG"

db_label = "C:\\School\\rssi_ranging\\data_analysis\\rssi.db"
conn = sqlite3.connect(db_label)
conn.row_factory = lambda cursor, row: row
c = conn.cursor()
# dist = np.array([hs.haversine((t_flat, t_flong), (a_flat, a_flong), unit=hs.Unit.METERS) for t_flat, t_flong, a_flat, a_flong in c.execute('SELECT DISTINCT T_FLAT, T_FLONG, A_FLAT, A_FLONG FROM exp4').fetchall()])
# dist.sort()
# print(len(dist))
# print(pd.read_sql_query(avg_rssi, conn))


rssi = sorted([(rssi, hs.haversine((t_flat, t_flong), (a_flat, a_flong), unit=hs.Unit.METERS)) for rssi, t_flat, t_flong, a_flat, a_flong in c.execute(avg_rssi).fetchall()], key=lambda row: row[1])
# print(np.sort(rssi, axis=0))
print(rssi)
# val = 0.  # this is the value where you want the data to appear on the y-axis.
# plt.plot(dist, np.zeros_like(dist) + val, ',')
# plt.show()

# for i in range(len(dist)):
#     print(i)
#     print((i)*NB_SAMPLES, ':', (i+1)*NB_SAMPLES)
#     print(rssi[(i)*NB_SAMPLES:(i+1)*NB_SAMPLES])
# for i in range(len(dist)):
#     d[i*NB_SAMPLES:(i+1)*NB_SAMPLES] = dist[i]
#     rssi[i*NB_SAMPLES:(i+1)*NB_SAMPLES] = np.array(random.sample(c.execute('SELECT RSSI FROM exp4 WHERE dist= ?', (dist[i],)).fetchall(), NB_SAMPLES))
# print(d)
# print(rssi)
#
# plt.scatter(d, rssi)
# plt.title("Raw RSSI values")
# plt.ylabel("RSSI (dBm)")
# plt.xlabel("Distance (m)")
# plt.show()
#
# # MIN-MAX
# d = np.zeros(len(dist)*2)
# rssi = np.zeros(len(dist)*2)
# for i in range(len(dist)):
#     d[i*2:(i+1)*2] = dist[i]
#     arr = np.array(c.execute('SELECT RSSI FROM exp4 WHERE dist= ?', (dist[i],)).fetchall())
#     rssi[i*2:(i+1)*2] = np.array([np.amin(arr), np.amax(arr)])
# plt.scatter(d, rssi)
# plt.title("Min-max RSSI values")
# plt.ylabel("RSSI (dBm)")
# plt.xlabel("Distance (m)")
# plt.show()

# AVERAGE
# d = np.zeros(len(dist))
# rssi = np.zeros(len(dist))
# for i in range(len(dist)):
#     rssi[i] = np.average(np.array(c.execute('SELECT RSSI FROM exp4 WHERE dist= ?', (dist[i],)).fetchall()))
plt.title("Average RSSI values")
plt.ylabel("RSSI (dBm)")
plt.xlabel("Distance (m)")
rssi, d = zip(*rssi)
plt.scatter(d, rssi, marker=".")
popt = np.polyfit(d, rssi, 2)
xn = np.linspace(min(d), max(d), len(d))
yn = np.polyval(popt, xn)
plt.plot(xn, yn)
# plt.semilogx()
plt.show()

# # SNR AVERAGE
# d = np.zeros(len(dist))
# rssi = np.zeros(len(dist))
# for i in range(len(dist)):
#     d[i] = dist[i]
#     rssi[i] = np.average(np.array(c.execute('SELECT SNR FROM exp4 WHERE dist= ?', (dist[i],)).fetchall()))
# plt.scatter(d, rssi)
# plt.title("Average SNR values")
# plt.ylabel("SNR (dBm)")
# plt.xlabel("Distance (m)")
# plt.show()
#
# # SNR MINMAX
#
# d = np.zeros(len(dist)*2)
# rssi = np.zeros(len(dist)*2)
# for i in range(len(dist)):
#     d[i*2:(i+1)*2] = dist[i]
#     arr = np.array(c.execute('SELECT SNR FROM exp4 WHERE dist= ?', (dist[i],)).fetchall())
#     rssi[i*2:(i+1)*2] = np.array([np.amin(arr), np.amax(arr)])
# plt.scatter(d, rssi)
# plt.title("Raw RSSI values")
# plt.ylabel("SNR")
# plt.xlabel("Distance (m)")
# plt.show()
