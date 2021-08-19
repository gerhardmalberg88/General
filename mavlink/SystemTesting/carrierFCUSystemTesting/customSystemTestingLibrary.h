#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.h>     // Added library for Test Automation
#include <mavsdk/plugins/telemetry/telemetry.h>                         // Added library for Test Automation
#include "plugins/info/info.h"                                          // Added library for Test Automation
#include <mavsdk/plugins/tune/tune.h>                                   // Added library for Test Automation
#include <mavsdk/plugins/action/action.h>                               // Added library for Test Automation
#include <mavsdk/plugins/offboard/offboard.h>                           // Added library for Test Automation

#include <cstdint>                                                      // Added library for Test Automation
#include <chrono>                                                       // Added library for Test Automation
#include <cstdint>                                                      // Added library for Test Automation
#include <memory>                                                       // Added library for Test Automation
#include <thread>                                                       // Added library for Test Automation
#include <unistd.h>                                                     // Added library for Test Automation
#include <future>                                                       // Added library for Test Automation
#include <iomanip>                                                      // Added library used for std::setpercision()
#include <iostream>                                                     // Added library used for std::setpercision()
#include <cmath>                                                        // Added library used for std::setpercision()
#include <limits>                                                       // Added library used for std::setpercision()


//############################################################################
// System initalization calls
void usage(const std::string& bin_name);
std::shared_ptr<mavsdk::System> get_system(mavsdk::Mavsdk& mavsdk);
//############################################################################