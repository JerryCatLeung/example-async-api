rm -rf build
mkdir build && cd build
cmake ..
make -j
./example-async-api-exe