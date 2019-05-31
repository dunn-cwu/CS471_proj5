Course: CS471
Assignment: Project 5
Student: Andrew Dunn
Instructor: Dr. Donald Davendra

---------------------------------
Project Description
---------------------------------

This project tests the NEH algorithm on three different
flow shop problems: Flows shop scheduling, flow shop with blocking,
and flow shop with no wait. Each of the three problems is tested over
120 different data sets, with all results recorded to various .csv files.

---------------------------------
Project Requirements
---------------------------------

1. A computer running Linux, MacOS, or Windows.
2. Some version of g++ that supports C++11 (g++, MinGW, or clang)
3. Cmake 3.1+

----

This program was succesfully built, ran, and tested on the following
operating systems and tool chains:

Ubuntu 19.10
g++ version 8.2 and cmake 3.10

MacOS 10.13
clang 10.0.0 and cmake 3.14

Windows 10
g++ via MinGW-W64 8.1 and cmake 3.14

Your C++ compiler must support the C++11 standard or greater.

---------------------------------
Build Instructions - Unix Based Machines
---------------------------------

For Ubuntu and MacOS:

Within the [project]/source/ directory you will find a few different
shell scripts. To build this project you can run either
'unix-build-debug.sh' or 'unix-build-release.sh' from within the
[project]/source/ directory. To build the release binary, open a terminal
and execute the following commands:

```
cd [Path-to-/source-dir]
./build-release.sh
```

Where [Path-to-/source-dir] is the path to the [project]/source/ directory
within this project. This script will run cmake and make, building the 
project with your default C++ compiler which should be auto-detected. 
The produced binary will be moved to the build/release directory.

---------------------------------
Build Instructions - Windows Based Machines
---------------------------------

For Windows 10 and earlier:

Within the [project]\source\ directory you will find a few different
batch scripts. To build this project in release mode you can run win-build-release.bat 
by simply double clicking it. This script will run MinGW cmake and make, building the 
project with your default MinGW C++ compiler which should be auto-detected if your
PATH system variable is set up correctly. The produced binary will be moved to the 
build/release directory.

---------------------------------
Run Instructions - Unix Based Machines
---------------------------------

For Ubuntu and MacOS:

To run this project, a few shell scripts are provided that will
run the release binary using different test parameter files included 
in the [project]/source/params/ directory. To run NEH on the flow shop
scheduling problem for all 120 data sets, execute the following commands in a terminal.

```
cd [Path-to-/source-dir]
./unix-run-fss.sh
```

Where [Path-to-/source-dir] is the file system path to the [project]/source/ directory
within this project. You should see a message that says all tests were ran after they complete.
Results files will be placed in the [project]/source/results directory.

--------

To run the flow shop with blocking or flow shop with no wait problems, open a terminal and 
execute the following commands:

```
cd [Path-to-/source-dir]
./unix-run-fsb.sh
./unix-run-fsnw.sh
```
Where [Path-to-/source-dir] is the file system path to the [project]/source/ directory
within this project. You should see a message that says all tests were ran after they complete.
Results files will be placed in the [project]/source/results directory.

--------

To run all three flow shop problems for all 120 data sets,
open a terminal and execute the following command:

```
cd [Path-to-/source-dir]
./unix-run-all.sh
```
Where [Path-to-/source-dir] is the file system path to the [project]/source/ directory
within this project. You should see a message that says all tests were ran after they complete.
Results files will be placed in the [project]/source/results directory.

--------

If you wish to run this project manually, within the terminal run the
following command:

```
./build/release/cs471-proj5.out [Input-parameter-file]
```

Where [Input-parameter-file] is the path to the input parameter ini file.
There several already provided within the [project]/source/params/ directory 
which will run the NEH algorithm with different flow shop problems. For example:

```
cd [Path-to-/source-dir]
./build/release/cs471-proj4.out ./params/fss.ini
```

Available files:

Used for debugging:
./params/debug.ini

Flow shop scheduling problem, all data sets:
./params/fss.ini

Flow shop with blocking problem, all data sets:
./params/fsb.ini

Flow shop with no wait problem, all data sets:
./params/fsnw.ini

---------------------------------
Optional Run Command Line Argument - Specify Job Sequence
---------------------------------

When running the project manually, you can specify an additional command line
argument that allows you verify the resulting start and departure time matrices
for a given job sequence.

```
./build/release/cs471-proj4.out [Input-parameter-file] "[job-sequence]"
```

Where "[job-sequence]" is a list of integers surrounded by quotation marks and
separated by spaces. For example, open a terminal and run the following command:

```
cd [Path-to-/source-dir]
./build/release/cs471-proj4.out ./params/debug.ini "1 2 3 4 5"
```

This will run data set 0 (the data set used in Dr. Davendra's powerpoint slides) with the
given job sequence of 1, 2, 3, 4, 5 and will output the resulting calculations. If you wish to 
select a different data set or flow shop problem, please edit the debug.ini file.

---------------------------------
Run Instructions - Windows based machines
---------------------------------

A few batch scripts are provided in the [project]\source\ directory.

To run this project in Windows, simply double click the the win-run-all.bat
file which will run both the NEH algorithm for all three flow shop problems.
Results files will be placed in the [project]/source/results directory.

---------------------------------
Input parameter file format
---------------------------------

The input parameter file is a configuration file in the *.ini format.
It contains one section, 'test'. The 'test' section contains various 
settings to control how the experiment is ran and which files are produced.

--

Within the 'test' section:

The 'minTestFile' entry sets the starting number of the input data sets to be computed.
All input data sets must be in a text file inside the ./source/[inputFilesDir]/directory
and have a file name with the following patern: [NUM].txt.

The 'maxTestFile' entry sets the last number of the input data sets to be computed.
All files within minTestFile and maxTestFile will be opened and solved with the NEH algorithm.
All input data sets must be in a text file inside the ./source/[inputFilesDir]/directory
and have a file name with the following patern: [NUM].txt.

The 'numThreads' entry sets the number of worker threads you want to use
to run the experiment. Note that you want to set this value to be equal or
close to the number of CPU's/CPU cores available in your system.

The 'algorithm' entry allows you to select which flowshop problem to use.
0 = Flow shop scheduling, 1 = Flow shop with blocking, and 2 = flow shop with no wait.

The 'inputFilesDir' entry is the directory path (without spaces) containing all input data
set files.

The 'resultsFile' entry is the file path (without spaces) where you wish to output the results
.csc file to.

The 'timesFile' entry is the file path prefix (without spaces) where you wish to output all start time
and departure time matrices for the resulting job sequence to.

---------------------------------
