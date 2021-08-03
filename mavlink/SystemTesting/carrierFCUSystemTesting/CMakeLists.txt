cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(carrierFCUSystemTesting)		# Changed for Test Automation

add_executable(carrierFCUSystemTesting		# Changed for Test Automation
	carrierFCUSystemTesting.cpp		# Changed for Test Automation, This is MAIN file
    customSystemTestingLibrary.cpp		# Changed for Test Automation, This is Library file
    customSystemTestingLibrary.h		# Changed for Test Automation, This is Library file
)

find_package(MAVSDK REQUIRED)

target_link_libraries(carrierFCUSystemTesting
	MAVSDK::mavsdk_calibration		# Added for Test Automation
    MAVSDK::mavsdk_telemetry                    # Added for Test Automation
    MAVSDK::mavsdk_mavlink_passthrough          # Added for Test Automation
    MAVSDK::mavsdk
)

if(NOT MSVC)
    add_compile_options(calibrate PRIVATE -Wall -Wextra)
else()
    add_compile_options(calibrate PRIVATE -WX -W2)
endif()
