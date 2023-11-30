rm -rf build
mkdir build && cd build
cmake ..
make -j
./async-api-exe