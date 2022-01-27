'''
import csv
datafile = open('BatteryTestOutput-1726-time-edited.csv', 'r')
#datareader = csv.reader(datafile, delimiter=';')
data = list(csv.reader(open(datafile)))
print(data)
'''
import csv
dataFile = open('BatteryTestOutput-1726-time-edited.csv', 'r')
dataReader = csv.reader(dataFile, delimiter=';')
data = []
for row in dataReader:
    data.append(row)
dataRows = len(data)
dataColumns = len(data[0])   
print("Rows ", dataRows, " Columns ", dataColumns)
print(type(data[1][0]))
print(data[1][0])
print("Converting to float")
for row in range(1,6):
    for col in range(0,6):
        print(data[row][col])
        ''' #Uncomment this section if unconvertability must be detected
        try:
            float(element[row][col])
        except ValueError:
            print "Not a float"
        '''
        dataFloat = float(data[row][col].replace(',', '.'))
        print(type(dataFloat))
        print(dataFloat)


