mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ..
mingw32-make
mkdir release
move /y cs471-proj5.out.exe release\cs471-proj5.out.exe
cd ..\
cmd /k
