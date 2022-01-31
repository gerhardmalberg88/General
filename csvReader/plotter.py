import csv
import re
import matplotlib.pyplot as plt
import numpy as np
import argparse

# TODO Parser implementation

#Instantiate the parser
parser = argparse.ArgumentParser()

parser.add_argument("--inputfilename", type=str, default="", required = True, 
        help="File full name must be entered. For example: exampleinput.csv")

parser.add_argument("--outputfilename", type=str, default="example", #required = True, 
        help="File name must be entered. For example: exampleoutput")

parser.add_argument("--xaxis", type=str, default="Time",  required = True,
        help="Enter x-axis header name")

parser.add_argument("--yaxiscount", type=int, default=0,  required = True,
        help="Enter number of y-axis plotted. Enter 0 if all y-axis need to be plotted")

parser.add_argument("--xlabel", type=str, default="x - axis",
        help="Enter x-axis label")

parser.add_argument("--ylabel", type=str, default="y - axis",
        help="Enter y-axis label")

parser.add_argument("--title", type=str, default="title",
        help="Enter title")

args = parser.parse_args()

dataFile = open("BatteryTestOutput-1726-time-edited.csv", "r") # BatteryTestOutput-1726-time-edited.csv
dataReader = csv.reader(dataFile, delimiter=";")
data = []
for row in dataReader:
    data.append(row)
dataRowsSize = len(data)
dataColumnsSize = len(data[0])   
print("Rows ", dataRowsSize, " Columns ", dataColumnsSize)

# Headers detection
for col in range(0,dataColumnsSize):
    dataHeader = data[0][col]
    dataHeaderStripped = dataHeader.replace(" ", "") # Remove white spaces
    dataHeaderStripped = ''.join([i for i in dataHeaderStripped if not i.isdigit()]) # Remove digits
    #print("alpha ", dataHeaderStripped.isalpha(), " string ", dataHeaderStripped)
    if dataHeaderStripped.isalpha() == False:
        print("Enter headers in correct alphabetic format ", dataHeaderStripped)
        exit()

# TODO Check if xaxis header is valid
headerNameCounter = 0
prevHeaderNameCounter = 0
for i in range(0, 1):
    for col in range(0,dataColumnsSize):
        if args.xaxis == data[0][col]:
            headerNameCounter = headerNameCounter + 1
    if headerNameCounter == prevHeaderNameCounter:
        while prevHeaderNameCounter == headerNameCounter:
            print("Following header is not found in file: ", args.xaxis, end=". ")
            args.xaxis = input("Enter correct header name: ")
            for j in range(0,dataColumnsSize):
                if args.xaxis == data[0][j]:
                    headerNameCounter = headerNameCounter + 1
    prevHeaderNameCounter = headerNameCounter

# Get header names if yaxiscount is more than, else plot all
if args.yaxiscount >= 1:
    yAxisHeaderNames = [None for x in range(args.yaxiscount)]
    for i in range(0,args.yaxiscount):
        print("Enter ",i+1," y-axis header name", end =" ")
        yAxisHeaderNames[i] = input()
    #print(yAxisHeaderNames)

# Headers match and index
headerNameCounter = 0
prevHeaderNameCounter = 0
for i in range(0, len(yAxisHeaderNames)):
    for col in range(0,dataColumnsSize):
        if yAxisHeaderNames[i] == data[0][col]:
            headerNameCounter = headerNameCounter + 1
    if headerNameCounter == prevHeaderNameCounter:
        while prevHeaderNameCounter == headerNameCounter:
            print("Following header is not found in file: ", yAxisHeaderNames[i], end=". ")
            yAxisHeaderNames[i] = input("Enter correct header name: ")
            for j in range(0,dataColumnsSize):
                if yAxisHeaderNames[i] == data[0][j]:
                    headerNameCounter = headerNameCounter + 1
    prevHeaderNameCounter = headerNameCounter 

exit()

# Dataconversion
dataFloat = [[None for x in range(dataColumnsSize)] for y in range(dataRowsSize)] 
for row in range(1,dataRowsSize):
    for col in range(0,dataColumnsSize):
        #print(data[row][col])      
        try:
            dataFloat[row][col] = float(data[row][col].replace(',', '.')) # Convert to float
        except ValueError: #If non convertable data is found, check if it is correct time stamp
            #print("Not a float")
            isPatternTime = re.search(r"\d{2}.\d{2}.\d{2}", data[row][col])
            if isPatternTime == None:
                print("Bad luck, time pattern is invalid, enter time in following pattern as interger value <1> or time <23.59.59>")
                exit()
            else:
                print("Time stamp at: ", isPatternTime.string)
                #print(type(isPatternTime))

# Data transposing for plotting
numpyArrayDataFloat = np.array(dataFloat) # Create numpy list from converted data and transpose column vector to row vector for plotter
transposeNumpyArrayDataFloat = numpyArrayDataFloat.T
transposedToList = transposeNumpyArrayDataFloat.tolist()

xAxis = transposedToList[0]
yAxis = transposedToList

#plt.scatter(xAxis, yAxis[0])
#plt.show()
#exit()

print("************")
print(xAxis)
print("************")
print(yAxis[3])
print("************")

plt.plot(xAxis, yAxis[3], label = "line 1") # TODO Forloop these
plt.plot(xAxis, yAxis[2], label = "line 2") # TODO Forloop these

plt.xlabel('x - axis')
plt.ylabel('y - axis')
plt.title('Two lines on same graph!')
plt.legend()
#plt.show()

# arg parser for csv filename, x from header, y from header, y2 from header, y3 from header, title define
# add plot all headers
# save as picture
