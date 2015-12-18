# termcalc
A terminal based calculator written in C++.

## Usage
Works like a normal calculator. The calculator can be in two modes, float and integer.

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
* `:mode`:          Sets the evaluation mode: float (default) or int.

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
