set -e
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ../
make
mkdir -p debug
mv ./cs471-proj5.out debug/cs471-proj5.out
echo Program binary moved to build/debug
