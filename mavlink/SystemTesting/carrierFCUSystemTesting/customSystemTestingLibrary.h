#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/calibration/calibration.h>
#include <future>
#include <iostream>
                                                                        
#include <mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.h>     // Added library for Test Automation
#include <mavsdk/plugins/telemetry/telemetry.h>                         // Added library for Test Automation
#include <chrono>                                                       // Added library for Test Automation
#include <cstdint>                                                      // Added library for Test Automation
#include <iostream>                                                     // Added library for Test Automation
#include <future>                                                       // Added library for Test Automation
#include <memory>                                                       // Added library for Test Automation
#include <thread>                                                       // Added library for Test Automation
#include <unistd.h>                                                     // Added library for Test Automation


//############################################################################
// System initalization calls
void usage(const std::string& bin_name);
std::shared_ptr<mavsdk::System> get_system(mavsdk::Mavsdk& mavsdk);
//############################################################################