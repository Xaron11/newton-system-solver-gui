# Download
```sh
git clone ...
cd ...
```

# Build
```sh
mkdir -p build && cd build && cmake ..
make -j$(nproc)
```
 
# Run 
Inside `build/`
```sh
./EAN 
```

# Create Library
```sh
cd lib
g++ -shared -fPIC -o ExampleLibrary.so ExampleLibrary.cpp
```