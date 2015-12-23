# termcalc
A terminal based calculator written in C++.

## Usage
Works like a normal calculator. The calculator can be in the following modes:
* float: Arithemtic done in 64 bits floating point.
* int: Arithemtic done in 64 bits integer.
* complex: For each component, the arithemtic is done in 64 bits floating point.

Each mode has different function and constants.

### Defining variables and functions
Variables can be defined as: `x = 4`. Functions are defined as: `f(x)=x^2`.

### Commands
The following commands are available:
* `:exit|:q|:quit`: Exits the program.
* `:bin`:           Sets to display the result in base 2.
* `:dec`:           Sets to display the result in base 10. (default)
* `:hex`:           Sets to display the result in base 16.
* `:vars`:          Prints the defined variables.
* `:funcs`:         Prints the defined functions.
* `:mode`:          Sets the evaluation mode: float (default), int or complex.

### Predefined
Variables and functions can be predefined by adding a file `default.termcalc` in your home folder.

## Build
To build:
```
make all
```
To run tests (requires [CXXTest](http://cxxtest.com/)):
```
make test
```

## Platforms
* Linux
* Windows (not tested with MSVC++)
