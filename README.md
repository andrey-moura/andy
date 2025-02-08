# Andy
Base of all Andy libraries + some facilities to include them.

## Building
```bash
    cd /usr/local/src/
    sudo su
    git clone https://github.com/andrey-moura/andy --recursive
    cd andy
    cmake -B build .
    cmake --build build --config Release --parallel
    cmake --install build
```