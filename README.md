# ledman
LEDManager (ledman) is a client-server software for light-emitting diodes management

# Build instructions
```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=(Debug|Release|RelWithDebInfo)
make -j
```

# Test instructions
```cpp
Application has two intuitive-to-use tests (client and server)
./server <ip> <port>
./client <ip> <port>
```
