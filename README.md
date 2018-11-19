# solitare_solver
C++ practice project to create a solver for some solitare games

## Build and test
```
cmake .
make
make test
```
> If built on cygwin after the downloading of `googletest` modify its `CMakeFiles.txt` to enable compiler extension and run `cmake` again.

## Run
```
./solitare_solver -h

Solve various solitare games
Usage:
  Solitare Solver [OPTION...]

  -g, --game arg   solitare game type (scorpion|klondike) (default: scorpion)
  -i, --input arg  input game state
  -v, --verbose    info messages
  -d, --debug      debug messages
  -h, --help       display this help
```

## Using
- [googletest](https://github.com/google/googletest)
- [cxxopts](https://github.com/jarro2783/cxxopts)
- [DownloadProject CMake extension](https://github.com/Crascit/DownloadProject)
