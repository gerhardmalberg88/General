// TODO Finish Discription of program
// 

#include "customSystemTestingLibrary.h"                                 // Added custom library for Test Automation

using namespace mavsdk;
using std::chrono::seconds;

int main(int argc, char** argv)
{
    //############################################################################
    // System & MAVLINK initialization

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

    auto telemetry = Telemetry(system);                                                 // Telemetry class instance

    Telemetry::Quaternion telemetryQuaternion;                                          // Instance for Quaternion struct
    Telemetry::Imu telemetryImuAll;                                                     // Instance for Imu struct
    Telemetry::GpsInfo telemetryGpsInfo;                                                // Instance for GpsInfo struct
    Telemetry::ScaledPressure telemetryScaledPressure;                                  // Instance for ScaledPressure struct
    Telemetry::Health telemetryHealth;                                                  // Instance for Health struct
    Telemetry::RcStatus telemetryRcStatus;                                              // Instance for RcStatus struct
    Telemetry::Battery telemetryBattery;                                                // Instance for Battery struct                                 

    //############################################################################
    // Plugins use in continous subscription method (Blocking)
    // using AttitudeQuaternionCallback = std::function<void(Quaternion)>;                                  DON'T uncomment this, used is for reference
    // void TelemetryImpl::subscribe_attitude_quaternion(Telemetry::AttitudeQuaternionCallback& callback)   DON'T uncomment this, used is for reference
    //telemetry.subscribe_attitude_quaternion([](Telemetry::Quaternion telemetryQuaternionBySubscribe) { std::cout << "QuaternionBySubscribing.x " << telemetryQuaternionBySubscribe.x << std::endl; });

    // Plugins use in polling method (Suitable for System Testing)
    sleep(1);                                                                           // Wait for system to get ready
    int printPrecision = 2;                                                             // Set decimal precision (Change this parameter to change the precision)
    for(int i = 0; i < 2; i++)
    {
        telemetryQuaternion = telemetry.attitude_quaternion();                          // Get Quaternion data and store it to struct
        std::cout /*<< // TODO std::setprecision(printPrecision)*/
        << "| Quaternion x: "           << telemetryQuaternion.x 
        << "| Quaternion y: "           << telemetryQuaternion.y
        << "| Quaternion z: "           << telemetryQuaternion.z
        << "| Quaternion w: "           << telemetryQuaternion.w
        << "| Quaternion timestamp: "   << telemetryQuaternion.timestamp_us
        << std::endl;
        sleep(1);
    }

    for(int i = 0; i < 2; i++)
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
        sleep(1);
    }

    for(int i = 0; i < 2; i++)
    {
        telemetryGpsInfo = telemetry.gps_info();                                        // Get GPS data and store it to struct
        std::cout 
        << "| GpsInfo num of sat: "     << telemetryGpsInfo.num_satellites
        << "| GpsInfo fix type: "       << telemetryGpsInfo.fix_type
        << std::endl;
        sleep(1);
    }

    for(int i = 0; i < 2; i++)
    {
        telemetryScaledPressure = telemetry.scaled_pressure();                          // Get Pressure data and store it to struct
        std::cout 
        << "| Pressure temp: "              << telemetryScaledPressure.temperature_deg
        << "| Pressure abs pressure: "      << telemetryScaledPressure.absolute_pressure_hpa
        << "| Pressure diff pressure: "     << telemetryScaledPressure.differential_pressure_hpa
        << std::endl;
        sleep(1);
    }
 
        for(int i = 0; i < 2; i++)
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
        sleep(1);
    }

        for(int i = 0; i < 2; i++)
    {
        telemetryRcStatus = telemetry.rc_status();                                      // Get RcStatus data and store it to struct
        std::cout 
        << "| RC is available: "            << telemetryRcStatus.is_available
        << "| RC signal strength: "         << telemetryRcStatus.signal_strength_percent
        << "| RC was one available: "       << telemetryRcStatus.was_available_once
        << std::endl;
        sleep(1);
    }

        for(int i = 0; i < 2; i++)
    {
        telemetryBattery = telemetry.battery();                                         // Get Battery data and store it to struct
        std::cout 
        << "| Battery id: "                 << telemetryBattery.id
        << "| Battery remaining percent: "  << telemetryBattery.remaining_percent
        << "| Battery voltage: "            << telemetryBattery.voltage_v
        << std::endl;
        sleep(1);
    }
    return 0;
}





