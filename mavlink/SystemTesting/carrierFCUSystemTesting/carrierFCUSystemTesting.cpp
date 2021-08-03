// TODO Finish Discription of program
// 

#include "customSystemTestingLibrary.h"                                 // Added custom library for Test Automation

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
    // Plugins initialization

    auto telemetry = Telemetry{system};                                         // Telemetry class instance
    Telemetry::Quaternion telemetryQuaternion;                                  // Instance for Quaternion struct
    sleep(2);
    for(int i = 0; i < 20000; i++)
    {
        telemetryQuaternion = telemetry.attitude_quaternion();
        std::cout << "telemetryQuaternion.x " << telemetryQuaternion.y << std::endl;
        sleep(1);
    }
    return 0;
}
