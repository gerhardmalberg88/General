import matplotlib.pyplot as plt
import numpy as np
from statistics import mean
import matplotlib.dates as md

L = 1
C = 1
R = 2

#L = 10
#C = 0.1
#R = 1

#L = 100
#C = 0.01
#R = 0.5

V = 1
BWStart = 0
BWEnd = 0
BWSwitchState = 0

sizeOfAngFreq = ((5-0.0)/0.01)+1 #((1.5-0.5)/0.01)+1
sizeOfAngFreq = int(sizeOfAngFreq)
angularFreq = [0 for x in range(0, sizeOfAngFreq)]
current = [0 for x in range(0, sizeOfAngFreq)]
Z = [0 for x in range(0, sizeOfAngFreq)]
bandwidthDetector = [None for x in range(0, sizeOfAngFreq)]

for i in range(0, sizeOfAngFreq):
    angularFreq[i] = 0.01+(i*0.01)
    XL = angularFreq[i]*L
    XC = 1/(angularFreq[i]*C)
    R = R
    Z[i] = np.sqrt((R**2)+(XL-XC)**2)
    current[i] = V/Z[i]
    print("angularFreq ", angularFreq[i], " current ", current[i])

minimaR = min(Z)
print("MinimaR ", minimaR)

for i in range(0, sizeOfAngFreq):
    if Z[i] <= minimaR/0.707:
        bandwidthDetector[i] = current[i]
        print("BW Region")
        if BWSwitchState == 0:
            BWStart = i
            BWSwitchState = 1
    if BWSwitchState == 1 and Z[i] <= minimaR/0.707:
        BWEnd = i

measuredBW = angularFreq[BWEnd] - angularFreq[BWStart]
resonantFreq = 1/(np.sqrt(L*C))
unloadedQ = 1/(resonantFreq*R*C)
fractionalBW = resonantFreq/unloadedQ

print("Resonant Frequency ", resonantFreq)
print("Unloaded Q ", unloadedQ)
print("Fractional BW ", fractionalBW)
print("BWStart ", angularFreq[BWStart])
print("BWEnd ", angularFreq[BWEnd])
print("measuredBW ", measuredBW)

#plt.plot(angularFreq, current, label = "Current")
#plt.plot(angularFreq, bandwidthDetector, label = "BW")
plt.plot(angularFreq, Z, label = "Z")
plt.xlabel("Angular Frequency")
plt.ylabel("Current")
s1 = "Series RLC Plotter L="
s2 = " C="
s3 = " R="
title = "L={} C={} R={} w0={} Q={} fBW={} mBW={}".format(L, C, R, resonantFreq, unloadedQ, fractionalBW, measuredBW)
plt.title(title)
plt.legend()
plt.show()