#!/usr/bin/python3

import matplotlib.pyplot as plt

letterA = []
letterC = []
filename = "histogram.txt"

with open(filename,'r') as f:
    for line in f:
        a = line.split(':')
        letterA.append(a[0])
        letterC.append(int(a[1]))

fig = plt.figure()
plt.bar(letterA,letterC)
plt.xlabel("Letters")
plt.ylabel("Count")
plt.show()
