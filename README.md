# ledman
LEDManager (ledman) is a client-server software for light-emitting diodes management

# Build instructions
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=(Debug|Release|RelWithDebInfo)
make -j

# Test instructions
Application has two intuitive-to-use tests (client and server)
./server <ip> <port>
./client <ip> <port>
