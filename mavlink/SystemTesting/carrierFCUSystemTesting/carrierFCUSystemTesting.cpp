/*
@brief Intended to be used for FCU System Testing

@test Have been tested and verifed with following FCU hardware & software
Hardware UID: 000100000000333634313338510400310028| Flight software : 1.12.1| OS software : 10.0.0
*/
#include "customSystemTestingLibrary.h"                                 // Added custom library for Test Automation

using namespace mavsdk;
using std::chrono::seconds;

int main(int argc, char** argv)
{
    //############################################################################
    // System & MAVLINK initialization
    // TODO Use UDP connection

    if (argc != 2) 
    {
        usage(argv[0]);
        return 1;
    }
    Mavsdk mavsdk;
    ConnectionResult connection_result = mavsdk.add_any_connection(argv[1]);
    if (connection_result != ConnectionResult::Success) 
    {
        std::cerr << "Connection failed: " << connection_result << '\n';
        return 1;
    }
    auto system = get_system(mavsdk);
    if (!system) 
    {
        return 1;
    }
    //############################################################################
    // Plugins initialization & Data holders instatiation

    Telemetry telemetry(system);                                                        // Telemetry class instance
    Tune tune(system);                                                                  // Tune class instance
    Info info(system);                                                                  // Info class instance
    MavlinkPassthrough mavlink_passthrough(system);                                     // Passthrough class instance
    Action action(system);                                                              // Action class instance
    Offboard offboard(system);                                                          // Offboard class instance

    Telemetry::Quaternion telemetryQuaternion;                                          // Instance for Quaternion struct
    Telemetry::Imu telemetryImuAll;                                                     // Instance for Imu struct
    Telemetry::GpsInfo telemetryGpsInfo;                                                // Instance for GpsInfo struct
    Telemetry::ScaledPressure telemetryScaledPressure;                                  // Instance for ScaledPressure struct
    Telemetry::Health telemetryHealth;                                                  // Instance for Health struct
    Telemetry::RcStatus telemetryRcStatus;                                              // Instance for RcStatus struct
    Telemetry::Battery telemetryBattery;                                                // Instance for Battery struct
    Telemetry::ActuatorControlTarget telemetryActuatorControlTarget;                    // Instance for Actuator struct
    Telemetry::DistanceSensor telemetryDistanceSensor;                                  // Instance for DistanceSensor struct 

    Offboard::ActuatorControl offboardActuatorControl;                                  // Instance for offboard actuator group control struct
    Offboard::ActuatorControlGroup offboardActuatorControlGroup;                        // Instance for offboard actuator group select struct
    Offboard::Result offboardResult;                                                    // Instance for offboard results enum

    std::pair<Info::Result, Info::Identification> hardwareIDPairResult;                 // Hardware and result pair
    std::pair<Info::Result, Info::Version> versionPairResult;                           // Version and result pair
    std::pair<Info::Result, Info::Product> productVendorIDPairResult;                   // Product and vendor ID and result pair

    Action::Result armingResult;                                                        // Arming result enum

    //############################################################################
    // Raw MAVLINK message handling 
    // Commands == used for commanding or setting, ACK is returned by the FCU
    // Messages == can be used for data stream, data stream is returned by the FCU

    // System ID AKA Network ID is stored at FCU EEPROM (Set to 1 by default) 
    // Component ID is system-unique component ID and used for addressing and routing

    /* 
    // TODO Perform RAW MAVLINK message handling
    // Result send_command_int(const CommandInt& command);                                                          DON'T uncomment this, used for reference
    // void subscribe_message_async(uint16_t message_id, std::function<void(const mavlink_message_t&)> callback);   DON'T uncomment this, used for reference
    std::cout << "MAVLINK passthrought started, handling raw MAVLINK messages" << std::endl;
    MavlinkPassthrough::Result passthroughtResult;
    mavlink_message_t messageToBeSent;
    messageToBeSent.seq     = 255;
    messageToBeSent.len     = 255;
    messageToBeSent.sysid   = mavlink_passthrough.get_our_sysid();
    messageToBeSent.compid  = mavlink_passthrough.get_our_compid();
    messageToBeSent.msgid   = 31;                                                   // To request target sysID & compID
    passthroughtResult = mavlink_passthrough.send_message(messageToBeSent);
    std::cout << "Message ACK status: " << passthroughtResult << std::endl;
    mavlink_passthrough.subscribe_message_async(31, [](mavlink_message_t messageProto){std::cout << "###### Callback function for subscribe " << std::endl;});
    uint8_t ourSysID = mavlink_passthrough.get_our_sysid();
    uint8_t ourCompID = mavlink_passthrough.get_our_compid();
    std::cout << "Stopped handling raw MAVLINK messages" << std::endl;
    */
    //############################################################################
    // Plugins use in continous subscription method
    //using AttitudeQuaternionCallback = std::function<void(Quaternion)>;                                  DON'T uncomment this, used for reference
    //void TelemetryImpl::subscribe_attitude_quaternion(Telemetry::AttitudeQuaternionCallback& callback)   DON'T uncomment this, used for reference
    //telemetry.subscribe_attitude_quaternion([](Telemetry::Quaternion telemetryQuaternionBySubscribe) { std::cout << "QuaternionBySubscribing.x " << telemetryQuaternionBySubscribe.y << std::endl; });

    // Plugins use in polling method (Suitable for System Testing)
    int printPrecision                  = 1;                                            // Set decimal precision (Change this parameter to change the precision)
    int iterationOfDataAquire           = 1;                                            // Set loop count for all data
    long timeWaitBetweenDataAquire      = 500000;                                       // Set time that is required to be waited before next data acquisition
    uint8_t our_sysid                   = 1;                                            // Stores our system ID fot parachute check
    sleep(1);                                                                           // Wait for system to get ready


    // TODO Arm vehicle
    // 1. Configure arm check off to be able to arm without checking
    // 2. Arm vehicle
    // PX4 Configuration:
    // COM_ARM_MAG_STR = 0
    // COM_OBL_ACT = Disabled
    // COM_OBL_RC_ACT = Disabled


    // TODO Check if following check can be used
    // while (!telemetry.health_all_ok()) {
    //     std::cout << "Waiting for system to be ready\n";
    //     sleep(1);
    // }

    hardwareIDPairResult        = info.get_identification();                            // Get hardware and software version and product id                                   
    versionPairResult           = info.get_version();
    productVendorIDPairResult   = info.get_product();
    std::cout
    << "| Hardware UID: "           << hardwareIDPairResult.second.hardware_uid
    << "| Flight software : "       << versionPairResult.second.flight_sw_major << "." << versionPairResult.second.flight_sw_minor << "." << versionPairResult.second.flight_sw_patch
    << "| OS software : "           << versionPairResult.second.os_sw_major << "." << versionPairResult.second.os_sw_minor << "." << versionPairResult.second.os_sw_patch
    << std::endl;
    usleep(timeWaitBetweenDataAquire);

    for(int i = 0; i < iterationOfDataAquire; i++)
    {
        telemetryGpsInfo = telemetry.gps_info();                                        // Get GPS data and store it to struct
        std::cout 
        << "| GpsInfo num of sat: "     << telemetryGpsInfo.num_satellites
        << "| GpsInfo fix type: "       << telemetryGpsInfo.fix_type
        << std::endl;
        usleep(timeWaitBetweenDataAquire);
    }

    for(int i = 0; i < iterationOfDataAquire; i++)
    {
        telemetryScaledPressure = telemetry.scaled_pressure();                          // Get Pressure data and store it to struct
        std::cout 
        << "| Pressure temp: "              << telemetryScaledPressure.temperature_deg
        << "| Pressure abs pressure: "      << telemetryScaledPressure.absolute_pressure_hpa
        << "| Pressure diff pressure: "     << telemetryScaledPressure.differential_pressure_hpa
        << std::endl;
       usleep(timeWaitBetweenDataAquire);
    }
 
    for(int i = 0; i < iterationOfDataAquire; i++)
    {
        telemetryHealth = telemetry.health();                                           // Get Health data and store it to struct
        std::cout 
        << "| Is accelerometer cal.: "      << telemetryHealth.is_accelerometer_calibration_ok
        << "| Is gyrometer cal. OK: "       << telemetryHealth.is_gyrometer_calibration_ok
        << "| Is magnetometer cal. OK: "    << telemetryHealth.is_magnetometer_calibration_ok
        << "| Is armable: "                 << telemetryHealth.is_armable
        << "| Is global position OK: "      << telemetryHealth.is_global_position_ok
        << "| Is home position OK: "        << telemetryHealth.is_home_position_ok
        << "| Is local position OK: "       << telemetryHealth.is_local_position_ok
        << std::endl;
        usleep(timeWaitBetweenDataAquire);
    }

    for(int i = 0; i < iterationOfDataAquire; i++)
    {
        telemetryRcStatus = telemetry.rc_status();                                      // Get RC status data and store it to struct
        std::cout 
        << "| RC is available: "            << telemetryRcStatus.is_available
        << "| RC signal strength: "         << telemetryRcStatus.signal_strength_percent
        << "| RC was one available: "       << telemetryRcStatus.was_available_once
        << std::endl;
        usleep(timeWaitBetweenDataAquire);
    }

    for(int i = 0; i < iterationOfDataAquire; i++)
    {
        telemetryBattery = telemetry.battery();                                         // Get Battery data and store it to struct
        std::cout 
        << "| Battery id: "                 << telemetryBattery.id
        << "| Battery remaining percent: "  << telemetryBattery.remaining_percent
        << "| Battery voltage: "            << telemetryBattery.voltage_v
        << std::endl;
        usleep(timeWaitBetweenDataAquire);
    }
    
    for(int i = 0; i < iterationOfDataAquire; i++)
    {
        telemetryDistanceSensor = telemetry.distance_sensor();                          // Get data from Distance sensor (FCU firmaware must be configured for LeddarOne)
        if(isnan((float)telemetryDistanceSensor.current_distance_m))
        {
            std::cerr << "LeddarOne ranger distance returns NAN, check connection of LeddarOne" << std::endl;
            return 1;
        }
        std::cout
        << "| Distance sensor LeddarOne current distance: "     << telemetryDistanceSensor.current_distance_m
        << "| Distance sensor LeddarOne max distance: "         << telemetryDistanceSensor.maximum_distance_m
        << "| Distance sensor LeddarOne min distance: "         << telemetryDistanceSensor.minimum_distance_m
        << std::endl;
        usleep(timeWaitBetweenDataAquire);
    }

    // TODO Camera add more checks or delete
    std::cout << "Camera status: ";
    if (system->has_camera(/*Camera ID as parameter, else checks for any camera*/))     // Check for camera connection (FCU Firmware must be configured PixyCam for camera)
    {
        std::cout << "Camera connected and detected\n";
    }
    else
    {
        std::cout << "No camera connected or detected\n";
    }

    /*
    // ################### Use only if offboard control mode is used
    bool setpointStatus{0};
    setpointStatus = offboard.is_active();
    std::cout << "setpointStatus " << setpointStatus << std::endl;
    action.arm();
    for(int i = 0; i <= 16; i++)
    {
        offboardActuatorControlGroup.controls.push_back(1000);
        std::cout << i << std::endl;
    }
    offboardActuatorControl.groups.push_back(offboardActuatorControlGroup);
    offboardActuatorControl.groups.push_back(offboardActuatorControlGroup);
    std::cout << "offboardActuatorContorl size " << offboardActuatorControl.groups.size() << std::endl;
    std::cout << "offboardActuatorContorl size " << offboardActuatorControlGroup.controls.size() << std::endl;
    offboardResult = offboard.set_actuator_control(offboardActuatorControl);
    std::cout << "offboardResult " << offboardResult << std::endl;
    setpointStatus = offboard.is_active();
    std::cout << "setpointStatus " << setpointStatus << std::endl;
    sleep(2);
    //offboardResult = offboard.start();
    std::cout << "offboardResult " << offboardResult << std::endl;
    setpointStatus = offboard.is_active();
    std::cout << "setpointStatus " << setpointStatus << std::endl;
    //Offboard::Result set_actuator_control(Offboard::ActuatorControl actuator_control);
    */

    /*
    // TODO or fix this and add Parachute (Actuator/Servo) check
    int indexActuator = 1;            // Actuator index                                                                             
    float valueActuator = 0.8;        // Actuator value (-1.0 to 1.0 goes from 1000 to 2000)
    std::cout << "Setting PWM to parachute pin\n";
    Action::Result set_actuator_result = action.set_actuator(indexActuator, valueActuator);       // Command PWM to parachute pin
    if (set_actuator_result != Action::Result::Success) 
    {
        std::cerr << "Setting actuator failed:" << set_actuator_result << '\n';
        return 1;
    }
    if (set_actuator_result == Action::Result::Success) 
    {
        std::cout << "Setting succeeded\n";
    }
    sleep(1);
    */

    // indexActuator = 2;              // Actuator index                                                                             
    // valueActuator = 1100;            // Actuator value (-1.0 to 1.0 goes from 1000 to 2000)
    // std::cout << "Setting PWM to parachute pin\n";
    // set_actuator_result = action.set_actuator(indexActuator, valueActuator);                    // Command PWM to parachute pin
    // if (set_actuator_result != Action::Result::Success) 
    // {
    //     std::cerr << "Setting actuator failed:" << set_actuator_result << '\n';
    //     return 1;
    // }
    // if (set_actuator_result == Action::Result::Success) 
    // {
    //     std::cout << "Setting succeeded\n";
    // }
    // sleep(1);

    // TODO Check the actuators
    // telemetryActuatorOutputStatus.active = 5;
    // telemetryActuatorOutputStatus.actuator.push_back(5.0);
    telemetryActuatorControlTarget = telemetry.actuator_control_target();
    std::cout << "Actuator value: "
    // << "| AUX1 " << telemetryActuatorControlTarget.controls.front()
    // << "| AUX2 " << telemetryActuatorControlTarget.controls.at(2)
    // << "| AUX3 " << telemetryActuatorControlTarget.controls.at(3)
    // << "| AUX4 " << telemetryActuatorControlTarget.controls.at(4)
    // << "| AUX5 " << telemetryActuatorControlTarget.controls.at(5)
    // << "| AUX6 " << telemetryActuatorControlTarget.controls.at(6)
    << std::endl;


        // int32_t group{0}; /**< @brief An actuator control group is e.g. 'attitude' for the core
        //                      flight controls, or 'gimbal' for a payload. */
        // std::vector<float>
        //     controls{}; /**< @brief Controls normed from -1 to 1, where 0 is neutral position. */


    for(int i = 0; i < iterationOfDataAquire; i++)
    {
        telemetryQuaternion = telemetry.attitude_quaternion();                          // Get Quaternion data and store it to struct
        std::cout
        << "| Quaternion x: "           << telemetryQuaternion.x 
        << "| Quaternion y: "           << telemetryQuaternion.y
        << "| Quaternion z: "           << telemetryQuaternion.z
        << "| Quaternion w: "           << telemetryQuaternion.w
        << "| Quaternion timestamp: "   << telemetryQuaternion.timestamp_us
        << std::endl;
        usleep(timeWaitBetweenDataAquire);
    }

    for(int i = 0; i < iterationOfDataAquire; i++)
    {
        telemetryImuAll = telemetry.raw_imu();                                          // Get Imu data and store it to struct
        std::cout 
        << "| Imu temp: "               << telemetryImuAll.temperature_degc
        << "| Imu acceleration fwd: "   << telemetryImuAll.acceleration_frd.forward_m_s2
        << "| Imu acceleration dwn: "   << telemetryImuAll.acceleration_frd.down_m_s2
        << "| Imu acceleration rgt: "   << telemetryImuAll.acceleration_frd.right_m_s2
        << "| Imu mag field: "          << telemetryImuAll.magnetic_field_frd.forward_gauss
        << "| Imu timestamp: "          << telemetryImuAll.timestamp_us
        << std::endl;
        usleep(timeWaitBetweenDataAquire);
    }

    std::vector<Tune::SongElement> song_elements;                                       // Check the Piezo by playing low-high sounds
    song_elements.push_back(Tune::SongElement::Duration4);
    song_elements.push_back(Tune::SongElement::NoteG);
    song_elements.push_back(Tune::SongElement::Flat);
    song_elements.push_back(Tune::SongElement::NotePause);
    song_elements.push_back(Tune::SongElement::NoteA);
    song_elements.push_back(Tune::SongElement::Flat);
    song_elements.push_back(Tune::SongElement::NotePause);
    song_elements.push_back(Tune::SongElement::OctaveDown);
    const int tempo = 255;
    Tune::TuneDescription tune_description;
    Tune::ResultCallback callbackResult;
    tune_description.song_elements = song_elements;
    tune_description.tempo = tempo;
    const auto result = tune.play_tune(tune_description);
    if (result != Tune::Result::Success) 
    {
        std::cerr << "Tune result: " << result << '\n'; 
        return 1;
    }
    /*
    std::cout << "Trying to arm the vehicle: ";
    sleep(1);
    action.arm();
    if(armingResult != Action::Result::Success)
    {
    std::cout << "Arming Failed" << std::endl;
    }
    if(armingResult == Action::Result::Success)
    {
    std::cout << "Arming Succeeded" << std::endl;
    }
    sleep(1);
    */
    return 0;
}





