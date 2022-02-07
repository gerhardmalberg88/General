import csv
import re
import matplotlib.pyplot as plt
import numpy as np
import argparse
from statistics import mean
import matplotlib.dates as md

parser = argparse.ArgumentParser()

parser.add_argument("-i", "--inputfilename", type=str, default="", required = True, 
        help="File full name must be entered. For example: exampleinput.csv")

parser.add_argument("-o", "--outputfilename", type=str, default="", 
        help="File name must be entered as full name with extension. For example: exampleoutput.png")

parser.add_argument("-x", "--xaxis", type=str, default="Time",  required = True,
        help="Enter x-axis header name")

parser.add_argument("-y", "--yaxiscount", type=int, default=0,  required = True,
        help="Enter number of y-axis plotted. Enter 0 if all y-axis need to be plotted")

parser.add_argument("--xlabel", type=str, default="x - axis",
        help="Enter x-axis label")

parser.add_argument("--ylabel", type=str, default="y - axis",
        help="Enter y-axis label")

parser.add_argument("--title", type=str, default="title",
        help="Enter title")

parser.add_argument("-t", "--ticklabels", type=int, default=None, 
        help="Quantity of tick labels can be defined here if time, date or name stmaps are used")

args = parser.parse_args()

dataFile = open(args.inputfilename, "r")
dataReader = csv.reader(dataFile, delimiter=";")
data = []
for row in dataReader:
    data.append(row)
dataRowsSize = len(data)
dataColumnsSize = len(data[0])   
print("Size of CSV: Rows ", dataRowsSize, " Columns ", dataColumnsSize)

# Headers detection
for col in range(0,dataColumnsSize):
    dataHeader = data[0][col]
    dataHeaderStripped = dataHeader.replace(" ", "") # Remove white spaces
    dataHeaderStripped = ''.join([i for i in dataHeaderStripped if not i.isdigit()]) # Remove digits
    if dataHeaderStripped.isalpha() == False:
        print("Enter headers in correct alphabetic format ", dataHeaderStripped)
        exit()

# Check if xaxis header is valid
if args.yaxiscount >= 1:
    columnsToPlot = [None for x in range(args.yaxiscount+1)]
if args.yaxiscount == 0:
    columnsToPlot = [None for x in range(dataColumnsSize+1)]

headerNameCounter = 0
prevHeaderNameCounter = 0
for i in range(0, 1):
    for col in range(0,dataColumnsSize):
        if args.xaxis == data[0][col]:
            headerNameCounter = headerNameCounter + 1
            columnsToPlot[0] = col
    if headerNameCounter == prevHeaderNameCounter:
        while prevHeaderNameCounter == headerNameCounter:
            print("Following header is not found in file: ", args.xaxis, end=". ")
            args.xaxis = input("Enter correct header name: ")
            for col in range(0,dataColumnsSize):
                if args.xaxis == data[0][col]:
                    headerNameCounter = headerNameCounter + 1
                    columnsToPlot[0] = col
    prevHeaderNameCounter = headerNameCounter

# Get header names if yaxiscount is more than 0, else plot all
if args.yaxiscount >= 1:
    yAxisHeaderNames = [None for x in range(args.yaxiscount)]
    for i in range(0,args.yaxiscount):
        print("Enter ",i+1," y-axis header name", end =" ")
        yAxisHeaderNames[i] = input()

    # Headers match and index
    headerNameCounter = 0
    prevHeaderNameCounter = 0
    for i in range(0, len(yAxisHeaderNames)):
        for col in range(0,dataColumnsSize):
            if yAxisHeaderNames[i] == data[0][col]:
                headerNameCounter = headerNameCounter + 1
                columnsToPlot[i+1] = col
        if headerNameCounter == prevHeaderNameCounter:
            while prevHeaderNameCounter == headerNameCounter:
                print("Following header is not found in file: ", yAxisHeaderNames[i], end=". ")
                yAxisHeaderNames[i] = input("Enter correct header name: ")
                for col in range(0,dataColumnsSize):
                    if yAxisHeaderNames[i] == data[0][col]:
                        headerNameCounter = headerNameCounter + 1
                        columnsToPlot[i+1] = col
        prevHeaderNameCounter = headerNameCounter 

# If yaxiscount = 0 then plot all columns
if args.yaxiscount == 0:
    for col in range(0, dataColumnsSize):
        if args.xaxis != data[0][col]:
            columnsToPlot[col+1] = col
    #print(columnsToPlot)
    removeNones = [i for i in columnsToPlot if i != None]
    columnsToPlot = removeNones

# Data conversion
timePatternUsed = False # Dedicates if time pattern 23.59.59 is used or not
dataFloat = [[None for x in range(dataColumnsSize)] for y in range(dataRowsSize)] 
for row in range(1,dataRowsSize):
    for col in range(0,dataColumnsSize):    
        try:
            dataFloat[row][col] = float(data[row][col].replace(',', '.')) # Convert to float
        except ValueError: #If non convertable data is found, check if it is correct time stamp
            isPatternTime = re.search(r"\d{2}.\d{2}.\d{2}", data[row][col])
            if isPatternTime == None:
                print("Time pattern is invalid, enter time in following pattern as interger value <1> or time <23.59.59>")
                exit()
            if isPatternTime != None:
                timePatternUsed = True

# Data transposing for plotting
numpyArrayDataFloat = np.array(dataFloat) # Create numpy list from converted data and transpose column vector to row vector for plotter
transposeNumpyArrayDataFloat = numpyArrayDataFloat.T
transposedToList = transposeNumpyArrayDataFloat.tolist()
arrayToStoreTimePattern = [None for x in range(dataRowsSize)] # THISS

if timePatternUsed == False:
    xAxis = transposedToList[0]
    yAxis = transposedToList
if timePatternUsed == True:
    xAxis = list(range(1, dataRowsSize+1))
    yAxis = transposedToList
    yAxis[0] = xAxis

    if args.ticklabels == None:
        tickLabelsCoefficient = -0.95
        tickLablesConstant = dataRowsSize+1
        tickLablesQuantity = tickLabelsCoefficient*dataRowsSize+tickLablesConstant
        tickLableCounter = 0
        for i in range(1, dataRowsSize):
            if tickLableCounter >= tickLablesQuantity:
                tickLableCounter = 0
                arrayToStoreTimePattern[i] = data[i][columnsToPlot[0]]
            if i == 1:
                arrayToStoreTimePattern[i] = data[i][columnsToPlot[0]]
            if i == dataRowsSize:
                arrayToStoreTimePattern[i] = data[i][columnsToPlot[0]]
            tickLableCounter = tickLableCounter + 1

    if args.ticklabels != None and args.ticklabels > 0:
        tickLablesQuantity = dataRowsSize/args.ticklabels
        tickLableCounter = 0
        for i in range(1, dataRowsSize):
            if tickLableCounter >= tickLablesQuantity:
                tickLableCounter = 0
                arrayToStoreTimePattern[i] = data[i][columnsToPlot[0]]
            if i == 1:
                arrayToStoreTimePattern[i] = data[i][columnsToPlot[0]]
            if i == dataRowsSize-1:
                arrayToStoreTimePattern[i] = data[i][columnsToPlot[0]]
            tickLableCounter = tickLableCounter + 1

    plt.xticks(xAxis, arrayToStoreTimePattern)
    plt.subplots_adjust(bottom=0.2)
    plt.xticks(rotation=90)

for i in range(1, len(columnsToPlot)):
    plt.plot(xAxis, yAxis[columnsToPlot[i]], label = data[0][columnsToPlot[i]])

# Calculate min,mean,max
colMaximum = [None for x in range(len(columnsToPlot))]
colMinimum = [None for x in range(len(columnsToPlot))]
colMean = [None for x in range(len(columnsToPlot))]
for col in range(0, len(columnsToPlot)):
    removeNones = [i for i in yAxis[col] if i != None]
    colMaximum[col] = max(removeNones)
    colMinimum[col] = min(removeNones)
    colMean[col] = mean(removeNones)
print(data[0])
print("Max ", colMaximum)
print("Min ", colMinimum)
print("Mean ", colMean)

plt.xlabel(args.xlabel)
plt.ylabel(args.ylabel)
plt.title(args.title)
plt.legend()
if args.outputfilename != "":
    plt.savefig(args.outputfilename)
plt.show()

# TODO Add option for two x axis plots