mkdir results
mkdir results\fss-times
mkdir results\fsb-times
mkdir results\fsnw-times

build\release\cs471-proj5.out.exe params\fss.ini
build\release\cs471-proj5.out.exe params\fsb.ini
build\release\cs471-proj5.out.exe params\fsnw.ini

cmd /k
