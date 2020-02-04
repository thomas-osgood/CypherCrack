#!/usr/bin/python3

from datetime import datetime
import matplotlib.pyplot as plt

letterA = []
letterC = []
filename = "histogram.txt"

with open(filename,'r') as f:
    for line in f:
        a = line.split(':')
        letterA.append(a[0])
        letterC.append(int(a[1]))

figname = "saved_figs/cryptograph_{0}.jpg".format(datetime.now().strftime("%Y%m%d_%H%M%S"))
fig = plt.figure()
plt.bar(letterA,letterC)
plt.xlabel("Letters")
plt.ylabel("Count")
try:
    plt.savefig(figname)
    print("[+] Histogram Saved To {0}".format(figname))
except:
    print("[!] Unable To Save Figure")
plt.show()

