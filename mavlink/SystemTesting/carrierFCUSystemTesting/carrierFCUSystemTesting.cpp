// TODO Finish Discription of program
// 

#include "customSystemTestingLibrary.h"                                 // Added custom library for Test Automation
#include <iomanip>                                                      // Added library used for std::setpercision()
#include <iostream>                                                     // Added library used for std::setpercision()
#include <cmath>                                                        // Added library used for std::setpercision()
#include <limits>                                                       // Added library used for std::setpercision()

using namespace mavsdk;
using std::chrono::seconds;

int main(int argc, char** argv)
{
    //############################################################################
    // System initialization

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
    // Plugins initialization & Struct instatiation

    auto telemetry = Telemetry{system};                                                 // Telemetry class instance

    Telemetry::Quaternion telemetryQuaternion;                                          // Instance for Quaternion struct
    Telemetry::Imu telemetryImuAll;                                                     // Instance for Imu struct
    Telemetry::GpsInfo telemetryGpsInfo;                                                // Instance of GpsInfo struct
    Telemetry::ScaledPressure telemetryScaledPressure;                                  // Instance of ScaledPressure struct

    //############################################################################
    // Plugins use
    sleep(1);                                                                           // Wait for system to get ready
    int printPrecision = 2;                                                             // Set decimal precision (Changes this parameter to change the precision)
    for(int i = 0; i < 3; i++)
    {
        telemetryQuaternion = telemetry.attitude_quaternion();                          // Get Quaternion data and store it to struct
        std::cout /*<< TODO std::setprecision(printPrecision)*/
        << "| Quaternion x: "               << telemetryQuaternion.x 
        << "| Quaternion y: "               << telemetryQuaternion.y
        << "| Quaternion z: "               << telemetryQuaternion.z
        << "| Quaternion w: "               << telemetryQuaternion.w
        << "| Quaternion timestamp: "       << telemetryQuaternion.timestamp_us
        << std::endl;
        sleep(1);
    }

    for(int i = 0; i < 3; i++)
    {
        telemetryImuAll = telemetry.raw_imu();                                              // Get Imu data and store it to struct
        std::cout 
        << "| Imu temp: "                       << telemetryImuAll.temperature_degc
        << "| Imu acceleration fwd: "           << telemetryImuAll.acceleration_frd.forward_m_s2
        << "| Imu acceleration dwn: "           << telemetryImuAll.acceleration_frd.down_m_s2
        << "| Imu acceleration rgt: "           << telemetryImuAll.acceleration_frd.right_m_s2
        << "| Imu mag field: "                  << telemetryImuAll.magnetic_field_frd.forward_gauss
        << "| Imu timestamp: "                  << telemetryImuAll.timestamp_us
        << std::endl;
        sleep(1);
    }

    for(int i = 0; i < 3; i++)
    {
        telemetryGpsInfo = telemetry.gps_info();                                              // Get GPS data and store it to struct
        std::cout 
        << "| GpsInfo num of sat: "                     << telemetryGpsInfo.num_satellites
        << "| GpsInfo fix type: "                       << telemetryGpsInfo.fix_type
        << std::endl;
        sleep(1);
    }

    for(int i = 0; i < 10; i++)
    {
        telemetryScaledPressure = telemetry.scaled_pressure();                                  // Get Pressure data and store it to struct
        std::cout 
        << "| Pressure data temp: "                     << telemetryScaledPressure.temperature_deg
        << "| Pressure abs pressure: "                  << telemetryScaledPressure.absolute_pressure_hpa
        << "| Pressure diff pressure: "                 << telemetryScaledPressure.differential_pressure_hpa
        << std::endl;
        sleep(1);
    }
 
    return 0;
}
