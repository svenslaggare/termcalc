# termcalc
A terminal based calculator written in C++.

## Usage
Works like a normal calculator. The calculator can be in the following modes:
* float: Arithemtic done in 64 bits floating point.
* int: Arithemtic done in 64 bits integer.
* complex: For each component, the arithemtic is done in 64 bits floating point.

Each mode has different predefined functions and constants.

### Defining variables and functions
Variables can be defined as: `x = 4`. Functions are defined as: `f(x)=x^2`.

### Commands
The following commands are available:
* `:exit|:q|:quit`: Exits the program.
* `:mode`:          Sets the evaluation mode: float (default), int or complex.
* `:display`:       Sets to display the result in the given base.
* `:vars`:          Prints the defined variables.
* `:funcs`:         Prints the defined functions.

### Predefined
Variables and functions can be predefined by adding a file `default.termcalc` in your home folder.

## Build

### Linux
To build:
```
make release
```
To run tests (requires [CXXTest](http://cxxtest.com/)):
```
make test
```

### Windows
Requirements:
* Visual Studio 2015.
* CMake.
* [CXXTest](http://cxxtest.com/) for tests.

The following command installs CxxTest:
```
git clone -q --branch=master https://github.com/svenslaggare/CxxTest.git C:\CXXTest
set PATH=%PATH%;C:\CXXTest\bin
```

To create the solution file run:
```
mkdir Windows
cd Windows
cmake -G "Visual Studio 14 2015 Win64" ..\
```
