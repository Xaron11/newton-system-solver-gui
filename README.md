# Newton System Solver GUI – C++/Qt Application for Solving Nonlinear Equation Systems
## Download
```sh
git clone https://github.com/Xaron11/newton-system-solver-gui.git
cd newton-system-solver-gui
```

## Build
```sh
mkdir -p build
cd build
cmake ..
make -j$(nproc)
```
 
## Run 
Inside `build/`
```sh
./EAN 
```

## Create Library
```sh
cd lib
g++ -shared -fPIC -o ExampleLibrary.so ExampleLibrary.cpp
```