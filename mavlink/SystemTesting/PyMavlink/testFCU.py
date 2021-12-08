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
gps1Msg = notAvailableStatus
gps2Msg = notAvailableStatus


while heartbeatCounter  < 2:
    msg = master.recv_match()
    #messageRecv = master.recv_msg()
    if not msg:
        continue
    if msg.get_type() == "HEARTBEAT":
        #print("Heartbeat Detected")        
        #print("\n\n*****Got message: %s*****" % msg.get_type())
        print("Message: %s" % msg)
        heartbeatMsg = msg
        heartbeatCounter = heartbeatCounter+1

    if msg.get_type() == "GPS_RAW_INT":
        #print("\n\n*****Got message: %s*****" % msg.get_type())
        #print("Message: %s" % msg)
        gps1Msg = msg
    
    if msg.get_type() == "LANDING_TARGET":
        #print("\n\n*****Got message: %s*****" % msg.get_type())
        print("Message: %s" % msg)
        gps2Msg = msg


print("Heartbeat ", heartbeatMsg)
print("GPS1 ", gps1Msg)
print("GPS2 ", gps2Msg)

# GPS2_RAW
print("Program Finished")
