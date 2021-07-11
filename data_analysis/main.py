import sqlite3
import matplotlib.pyplot as plt
import haversine as hs
import pandas as pd
import numpy as np
from math import sqrt, pow
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter
from mpl_toolkits.mplot3d import axes3d

def f(x):
    t_flat, t_flong, a_flat, a_flong = x[0], x[1], x[2], x[3]
    return sqrt(pow(12.36,2) + pow(hs.haversine((t_flat, t_flong), (a_flat, a_flong), unit=hs.Unit.METERS),2))

def g(df):
    df['distance'] = df.iloc[:, 2:6].apply(f, axis=1)
    return df.drop(columns=['T_FLAT', 'T_FLONG', 'A_FLAT', 'A_FLONG'])


conn = sqlite3.connect("C:\\School\\rssi_ranging\\data_analysis\\rssi_1.db")
rssi_1_exp4 = g(pd.read_sql("SELECT * from exp4", conn))
rssi_1_exp5 = g(pd.read_sql("SELECT * FROM exp5", conn))
rssi_1_exp6 = g(pd.read_sql("SELECT * FROM exp6", conn))


conn = sqlite3.connect("C:\\School\\rssi_ranging\\data_analysis\\rssi_2.db")
rssi_2_exp4 = g(pd.read_sql("SELECT * from exp4", conn))
rssi_2_exp6 = g(pd.read_sql("SELECT * FROM exp6", conn))


conn = sqlite3.connect("C:\\School\\rssi_ranging\\data_analysis\\rssi_3.db")
rssi_3_exp4 = g(pd.read_sql("SELECT * from exp4", conn))
rssi_3_exp6 = g(pd.read_sql("SELECT * FROM exp6", conn))

conn = sqlite3.connect("C:\\School\\rssi_ranging\\data_analysis\\other_rssi.db")
rssi_1_exp3 = pd.read_sql("SELECT * from exp3", conn)
rssi_1_exp3['distance'] = rssi_1_exp3.iloc[:, 1]
rssi_1_exp3 = rssi_1_exp3.drop(columns=['FLAT', 'FLONG', 'Dist', 'ID'])

df = pd.concat([
    rssi_1_exp4,
    rssi_1_exp5,
    rssi_1_exp6,
    rssi_2_exp4,
    rssi_2_exp6,
    rssi_3_exp4,
    rssi_3_exp6]).sort_values(by=['distance'], ignore_index=True)

d = df.where(df['SF'] != 12).groupby(['FREQ', 'SF'])['RSSI'].std()
d.columns = ['FREQ', 'SF', 'RSSI']
d = d.reset_index()

x = d['SF']
y = d['FREQ']
z = d['RSSI']
#X, Y, Z = axes3d.get_test_data(0.1)
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(x, y, z, c='b', marker='.')
ax.set_xlabel("SF")
ax.set_ylabel("FREQ")
ax.set_zlabel("Standard deviation RSSI (dBm)")
# ax.plot_wireframe(x, y, z, rstride=5, cstride=5)
# for angle in range(0, 360, 60):
ax.view_init(30, 360)
plt.draw()
plt.pause(.001)
plt.show()


