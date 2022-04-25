# solitare_solver
C++ practice project to create a solver for some solitare games

## Build and test
```
mkdir build
cmake -Bbuild .
make -Cbuild
make -Cbuild test
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
  -p, --parallel   use parallel solver
  -f, --filter     filter valid steps
  -s, --steps arg  steps to run
  -h, --help       display this help
```

## Documentation

Doxygen documentation is available here.

It can also be generated (`build/docs/html/index.html`):
```
make -Cbuild docs
```

## Coverage

The project uses CTest and CDash from CMake, but doesn't have a public profile, so the CDash server can be run with the help of docker.
```
cp .env.example .env
docker-compose run --rm cdash install configure
docker-compose up -d
```

Then create a project *SolitareSolverUnitTests* in CDash (http://localhost:8080).

To test and submit coverage run:
```
make -Cbuild Experimental
```

## Using
- [googletest](https://github.com/google/googletest)
- [cxxopts](https://github.com/jarro2783/cxxopts)
- [DownloadProject CMake extension](https://github.com/Crascit/DownloadProject)
- [CTPL](https://github.com/vit-vit/ctpl)
- [C++11 shared_mutex](https://github.com/HowardHinnant/HowardHinnant.github.io/blob/master/shared_mutex)
