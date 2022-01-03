from pymavlink import mavutil
import argparse
import sys
from datetime import datetime
import subprocess

print("testFCU.py Started")

#Instantiate the parser
parser = argparse.ArgumentParser()
# Parse Serial path or UDP string
parser.add_argument('--connection', type=str, default="/dev/ttyACM0",
        help="For Serial use /dev/ttyACM0 For UDP use udp:localhost:14460; Default: /dev/ttyACM0")
# Parse Loop requirement
parser.add_argument('--loop', type=int, default=1,
        help="Set the loop iterations if needed; Default: 1")
args = parser.parse_args()
loopIter = 0

while loopIter < args.loop:
    print(datetime.now())
    heartbeatCounter = 0
    notAvailableStatus = "Not Available"
    ok  = "OK"
    nok = "NOK"

    # TODO Better implementation to "Not Available"
    heartbeatMsg        = notAvailableStatus
    gps1Msg             = notAvailableStatus
    gps2Msg             = notAvailableStatus
    distSensorMsg       = notAvailableStatus
    rawImuMsg           = notAvailableStatus
    scaledPressure1Msg  = notAvailableStatus
    scaledPressure2Msg  = notAvailableStatus
    pixyCamMsg          = notAvailableStatus

    lsusbOutput = subprocess.check_output("lsusb")
    splittedUsbOutput = lsusbOutput.split()

    rm500qModemUSBID    = "2c7c:0800"
    fcuUSBID            = "26ac:0011"
    pixyCamUSBID        = "b1ac:f000"

    rm500qModemUSBState = "NOK"
    fcuUSBState         = "NOK"
    pixyCamUSBState     = "NOK"

    for i in range(len(splittedUsbOutput)):
        if rm500qModemUSBID == splittedUsbOutput[i]:
            rm500qModemUSBState = "OK"
        if fcuUSBID == splittedUsbOutput[i]:
            fcuUSBState = "OK"
        if pixyCamUSBID == splittedUsbOutput[i]:
            pixyCamUSBState = "OK"

    print("======================================================================")
    print("Modem USB        %s" %rm500qModemUSBState)
    print("FCU USB          %s" %fcuUSBState)
    print("PixyCam USB      %s" %pixyCamUSBState)
    print("======================================================================")

    #print("Program Started")
    # Create the connection
    # Need to provide the serial port and baudrate
    master = mavutil.mavlink_connection(args.connection)
    master.wait_heartbeat()

    #############
    '''
    # Request all data streams
    master.mav.request_data_stream_send(
            master.target_system, 
            master.target_component,                                
            mavutil.mavlink.MAV_DATA_STREAM_EXTRA1, 
            1, 
            1
            )
    master.mav.request_data_stream_send(
            master.target_system, 
            master.target_component,                                
            mavutil.mavlink.MAV_DATA_STREAM_EXTRA2, 
            1, 
            1
            )
    master.mav.request_data_stream_send(
            master.target_system, 
            master.target_component,                                
            mavutil.mavlink.MAV_DATA_STREAM_ALL, 
            1, 
            1
            )
    ############
    '''


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
            distSensorMsg = msg

        if msg.get_type() == "RAW_IMU":
            #print("\n\n*****Got message: %s*****" % msg.get_type())
            #print("Message: %s" % msg)
            rawImuMsg = msg

        if msg.get_type() == "SCALED_PRESSURE":
            #print("\n\n*****Got message: %s*****" % msg.get_type())
            #print("Message: %s" % msg)
            scaledPressure1Msg = msg

        if msg.get_type() == "SCALED_PRESSURE2":
            #print("\n\n*****Got message: %s*****" % msg.get_type())
            #print("Message: %s" % msg)
            scaledPressure2Msg = msg

        if msg.get_type() == "LANDING_TARGET":
            #print("\n\n*****Got message: %s*****" % msg.get_type())
            #print("Message: %s" % msg)
            pixyCamMsg = msg

        if msg.get_type() == "SYS_STATUS":
            #print("\n\n*****Got message: %s*****" % msg.get_type())
            #print("Message: %s" % msg)
           sysStatusMsg = msg

    # Heartbeat OK/NOK
    try:
        getattr(heartbeatMsg, "type")
    except AttributeError:
        heartbeatResult = nok
    else:
        heartbeatResult = ok
    print ("Heartbeat       %s" %heartbeatResult)

    # SystemStatus OK/NOK
    try:
        getattr(sysStatusMsg, "voltage_battery")
    except AttributeError:
        sysStatusResult = nok
    else:
        sysStatusResult = ok
    print ("SystemStatus    %s" %sysStatusResult)

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
        getattr(distSensorMsg, "current_distance")
    except AttributeError:
        distSensorResult = nok
    else:
        distSensorResult = ok
    if distSensorResult == ok:
        if distSensorMsg.current_distance == 0:
            distSensorResult = nok
        else:
            distSensorResult = ok
    print ("LeddarOne       %s" %distSensorResult)

    # Magnetometers OK/NOK TODO Detect one missing magnetometer
    try:
        getattr(rawImuMsg, "xmag")
    except AttributeError:
        rawImuResult = nok
    else:
        rawImuResult = ok
    if rawImuResult == ok:
        if rawImuMsg.xmag == 0 and rawImuMsg.ymag == 0 and rawImuMsg.zmag == 0:
            rawImuResult = nok
        else:
            rawImuResult = ok
    print ("Magnetometers   %s" %rawImuResult)

    print("======================================================================")
    print("Heartbeat        %s      %s" % (heartbeatResult, heartbeatMsg))
    print("SystemStatus     %s      %s" % (sysStatusResult, sysStatusMsg))
    print("GPS1             %s      %s" % (gps1MsgResult, gps1Msg))
    print("GPS2             %s      %s" % (gps2MsgResult, gps2Msg))
    print("LeddarOne        %s      %s" % (distSensorResult, distSensorMsg))
    print("RawIMU           %s" % rawImuMsg)
    print("ScaledPressure1  %s" % scaledPressure1Msg)
    print("ScaledPressure2  %s" % scaledPressure2Msg)
    #print("PixyCam          %s" % pixyCamMsg)
    print("======================================================================")

    # TODO PixyCam, Modem

    loopIter = loopIter + 1

print("Program Finished")
