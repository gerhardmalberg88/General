# Follow commands to Build, Compile & Run program

### Build MAVSDK from source
Clone MAVSDK
``` 
git clone https://github.com/mavlink/MAVSDK.git
``` 
Get all submodules
``` 
git submodule init
``` 
Update all submodules
``` 
git submodule update
``` 
Generate CMake project
``` 
cmake -DCMAKE_BUILD_TYPE=Debug -Bbuild/default -H.
``` 
Build project
``` 
cmake --build build	
``` 
Install project(library) files in your system
``` 
sudo cmake --build build --target install
``` 

### Build and Compile carrierFCUSystemTesting program at carrierFCUSystemTesting directory
At the carrierFCUSystemTesting directory generate CMake project of carrierFCUSystemTesting
``` 
cmake -Bbuild -H.
``` 
Build carrierFCUSystemTesting project
``` 
cmake --build
``` 

### Run example using UART connection
At carrierFCUSystemTesting/build directory run following command to execute program
``` 
sudo ./carrierFCUSystemTesting serial:///dev/ttyACM0:57600
``` 
