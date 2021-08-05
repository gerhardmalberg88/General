#include <mavsdk/mavsdk.h>
#include <future>
#include <iostream>
                                                                        
#include <mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.h>     // Added library for Test Automation
#include <mavsdk/plugins/telemetry/telemetry.h>                         // Added library for Test Automation
#include <chrono>                                                       // Added library for Test Automation
#include <cstdint>                                                      // Added library for Test Automation
#include <memory>                                                       // Added library for Test Automation
#include <thread>                                                       // Added library for Test Automation
#include <unistd.h>                                                     // Added library for Test Automation

#include <iomanip>                                                      // Added library used for std::setpercision()
#include <iostream>                                                     // Added library used for std::setpercision()
#include <cmath>                                                        // Added library used for std::setpercision()
#include <limits>                                                       // Added library used for std::setpercision()

#include "plugin_base.h"


//############################################################################
// System initalization calls
void usage(const std::string& bin_name);
std::shared_ptr<mavsdk::System> get_system(mavsdk::Mavsdk& mavsdk);
//############################################################################