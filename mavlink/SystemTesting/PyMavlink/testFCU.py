"""
FCU Communication
"""

# Import mavutil
from pymavlink import mavutil
import argparse
import sys

# Instantiate the parser
parser = argparse.ArgumentParser()
# Parse Serial path or UDP string
parser.add_argument('pos_arg1', type=str,
        help="For Serial use /dev/ttyACM0 For UDP use udp:localhost:14460")
args = parser.parse_args()
#print("Argument values:")
#print(args.pos_arg1)

print("Program Started")
# Create the connection
# Need to provide the serial port and baudrate
master = mavutil.mavlink_connection(args.pos_arg1)
master.wait_heartbeat()
heartbeatCounter = 0
notAvailableStatus = "Not Available"

# TODO Better implementation to "Not Available"
gps1Msg         = notAvailableStatus
gps2Msg         = notAvailableStatus
distSensor      = notAvailableStatus
rawImu          = notAvailableStatus
scaledPressure1 = notAvailableStatus
scaledPressure2 = notAvailableStatus
pixyCam         = notAvailableStatus

while heartbeatCounter  < 2:
    msg = master.recv_match()
    #messageRecv = master.recv_msg()
    if not msg:
        continue
    if msg.get_type() == "HEARTBEAT":
        #print("Heartbeat Detected")        
        #print("\n\n*****Got message: %s*****" % msg.get_type())
        #print("Message: %s" % msg)
        heartbeatMsg = msg
        heartbeatCounter = heartbeatCounter+1

    if msg.get_type() == "GPS_RAW_INT":
        #print("\n\n*****Got message: %s*****" % msg.get_type())
        #print("Message: %s" % msg)
        gps1Msg = msg
    
    if msg.get_type() == "GPS2_RAW":
        #print("\n\n*****Got message: %s*****" % msg.get_type())
        #print("Message: %s" % msg)
        gps2Msg = msg

    if msg.get_type() == "DISTANCE_SENSOR":
        #print("\n\n*****Got message: %s*****" % msg.get_type())
        #print("Message: %s" % msg)
        distSensor = msg

    if msg.get_type() == "RAW_IMU":
        #print("\n\n*****Got message: %s*****" % msg.get_type())
        #print("Message: %s" % msg)
        rawImu = msg

    if msg.get_type() == "SCALED_PRESSURE":
        #print("\n\n*****Got message: %s*****" % msg.get_type())
        #print("Message: %s" % msg)
        scaledPressure1 = msg

    if msg.get_type() == "SCALED_PRESSURE2":
        #print("\n\n*****Got message: %s*****" % msg.get_type())
        #print("Message: %s" % msg)
        scaledPressure2 = msg

    if msg.get_type() == "LANDING_TARGET":
        #print("\n\n*****Got message: %s*****" % msg.get_type())
        #print("Message: %s" % msg)
        pixyCam = msg

# GPS1 OK/NOK
try:
    getattr(gps1Msg, "fix_type")
except  AttributeError: 
    gps1MsgResult = nok
else:
    gps1MsgResult = ok
if gps1MsgResult == ok:
    if gps1Msg.fix_type == 0:
        gps1MsgResult = nok
    else:
        gps1MsgResult = ok
print ("GPS1            %s" %gps1MsgResult)

# GPS2 OK/NOK
try:
    getattr(gps2Msg, "fix_type")
except AttributeError:
    gps2MsgResult = nok
else:
    gps2MsgResult = ok
if gps2MsgResult == ok:
    if gps2Msg.fix_type == 0:
        gps2MsgResult = nok
    else:
        gps2MsgResult = ok
print ("GPS2            %s" %gps2MsgResult)

# LeddarOne OK/NOK
try:
    getattr(distSensor, "current_distance")
except AttributeError:
    distSensorResult = nok
else:
    distSensorResult = ok
if distSensorResult == ok:
    if distSensor.current_distance == 0:
        distSensorResult = nok
    else:
        distSensorResult = ok
print ("LeddarOne       %s" %distSensorResult)

# Magnetometers OK/NOK TODO Detect one missing magnetometer
try:
    getattr(rawImu, "xmag")
except AttributeError:
    rawImuResult = nok
else:
    rawImuResult = ok
if rawImuResult == ok:
    if rawImu.xmag == 0 and rawImu.ymag == 0 and rawImu.zmag == 0:
        rawImuResult = nok
    else:
        rawImuResult = ok
print ("Magnetometers   %s" %rawImuResult)

print("======================================================================")
print("Heartbeat       %s" % heartbeatMsg)
print("GPS1            %s   %s" % (gps1MsgResult, gps1Msg))
print("GPS2            %s   %s" % (gps2MsgResult, gps2Msg))
print("LeddarOne       %s   %s" % (distSensorResult, distSensor))
print("RawIMU          %s" % rawImu)
print("ScaledPressure1 %s" % scaledPressure1)
print("ScaledPressure2 %s" % scaledPressure2)
print("PixyCam         %s" % pixyCam)
print("======================================================================")

# TODO Magnetometer, Rangefinder(Leddarone), PixyCam, IMU Pressure, IMU Accel, IMU Gyro,
print("Program Finished")
