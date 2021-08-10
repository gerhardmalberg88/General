# Follow commands to Build, Compile & Run program

### Build MAVSDK from source
``` 
git clone https://github.com/mavlink/MAVSDK.git
``` 

``` 
git submodule init
``` 

``` 
git submodule update
``` 

``` 
cmake -DCMAKE_BUILD_TYPE=Debug -Bbuild/default -H.
``` 

``` 
cmake --build build	
``` 

``` 
sudo cmake --build build --target install
``` 

### Build and Compile carrierFCUSystemTesting program at carrierFCUSystemTesting directory
``` 
cmake -Bbuild -H.
``` 

``` 
cmake --build
``` 

### Run example using UART connection
``` 
sudo ./carrierFCUSystemTesting serial:///dev/ttyACM0:57600
``` 
