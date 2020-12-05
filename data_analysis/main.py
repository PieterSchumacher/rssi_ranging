import sqlite3
import matplotlib.pyplot as plt
import random
import numpy

NB_SAMPLES = 10


conn = sqlite3.connect('../rssi.db')
conn.row_factory = lambda cursor, row: row[0]
c = conn.cursor()

dist = c.execute('SELECT DISTINCT dist FROM exp1').fetchall()
d = numpy.zeros(len(dist)*NB_SAMPLES)
rssi = numpy.zeros(len(dist)*NB_SAMPLES)

# for i in range(len(dist)):
#     print(i)
#     print((i)*NB_SAMPLES, ':', (i+1)*NB_SAMPLES)
#     print(rssi[(i)*NB_SAMPLES:(i+1)*NB_SAMPLES])
for i in range(len(dist)):
    d[i*NB_SAMPLES:(i+1)*NB_SAMPLES] = dist[i]
    rssi[i*NB_SAMPLES:(i+1)*NB_SAMPLES] = numpy.array(random.sample(c.execute('SELECT RSSI FROM exp1 WHERE dist= ?', (dist[i],)).fetchall(), NB_SAMPLES))
print(d)
print(rssi)

plt.scatter(d, rssi)
plt.show()

# MIN-MAX
d = numpy.zeros(len(dist)*2)
rssi = numpy.zeros(len(dist)*2)
for i in range(len(dist)):
    d[i*2:(i+1)*2] = dist[i]
    arr = numpy.array(c.execute('SELECT RSSI FROM exp1 WHERE dist= ?', (dist[i],)).fetchall())
    rssi[i*2:(i+1)*2] = numpy.array([numpy.amin(arr), numpy.amax(arr)])
plt.scatter(d, rssi)
plt.show()

# AVERAGE
d = numpy.zeros(len(dist))
rssi = numpy.zeros(len(dist))
for i in range(len(dist)):
    d[i] = dist[i]
    rssi[i] = numpy.average(numpy.array(c.execute('SELECT RSSI FROM exp1 WHERE dist= ?', (dist[i],)).fetchall()))
plt.scatter(d, rssi)
plt.show()
