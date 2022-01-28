import csv
import re
import matplotlib.pyplot as plt
import numpy as np

dataFile = open("BatteryTestOutput-1726-time-edited.csv", "r") # BatteryTestOutput-1726-time-edited.csv
dataReader = csv.reader(dataFile, delimiter=";")
data = []
for row in dataReader:
    data.append(row)
dataRowsSize = len(data)
dataColumnsSize = len(data[0])   
print("Rows ", dataRowsSize, " Columns ", dataColumnsSize)
#print(type(data[1][0]))
#print(data[1][0])
#print("Converting to float")
dataFloat = [[None for x in range(dataColumnsSize)] for y in range(dataRowsSize)] 
for row in range(1,dataRowsSize):
    for col in range(0,dataColumnsSize):
        #print(data[row][col])      
        try:
            dataFloat[row][col] = float(data[row][col].replace(',', '.'))
        except ValueError:
            #print("Not a float")
            isPatternTime = re.search(r"\d{2}.\d{2}.\d{2}", data[row][col])
            if isPatternTime == None:
                print("Bad luck, time pattern is invalid, enter time in following pattern as interger value <1> or time <23.59.59>")
                exit()
            else:
                print("Time stamp at: ", isPatternTime.string)
                #print(type(isPatternTime))

#print("After Conversion ", dataFloat)
numpyArrayDataFloat = np.array(dataFloat)
transposeNumpyArrayDataFloat = numpyArrayDataFloat.T
transposeToList = transposeNumpyArrayDataFloat.tolist()

xAxis = transposeToList[0]
yAxis = transposeToList 

plt.scatter(xAxis, yAxis[0])
plt.show()
#exit()

print("************")
print(xAxis)
print("************")
print(yAxis[0])
print("************")

plt.plot(xAxis, yAxis[0], label = "line 1")
 
plt.xlabel('x - axis')

plt.ylabel('y - axis')

plt.title('Two lines on same graph!')
 
plt.legend()
 

plt.show()

# save as picture

# First version with normal time stamp
# Second version with original clock time stamp
# arg parser for csv filename, x, and y data
# headers must be present

