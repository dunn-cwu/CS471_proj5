set -e
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
make
mkdir -p release
mv ./cs471-proj5.out release/cs471-proj5.out
echo Program binary moved to build/release
