## [c++ GRPC Installation](https://github.com/grpc/grpc/blob/v1.14.1/src/cpp/README.md)
Prerequisites: (build and install the grpc components)  
  ```
  sudo apt-get install build-essential autoconf libtool pkg-config curl golang libssl-dev
  ```
Clone grpc repo:  
  ```
  git clone https://github.com/grpc/grpc
  ```
Build third_party dependencies first as package (it's the only way grpc will work w/ the cmake find_package mechanism (see [relevant ticket](https://github.com/grpc/grpc/issues/16741))):  
  ```
  git submodule update --init
  ```  
**Note:** If `boringssl` does not compile, you need to checkout master of the `boringssl` lib  
#### Build and Install ZLIB
  ```
  mkdir build; cd build
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
  make && sudo make install
  ```
#### Install Protoc
Read more about [Protoc](https://github.com/protocolbuffers/protobuf/blob/master/cmake/README.md)
  ```
  cd grpc/third_party/protobuf/cmake
  mkdir -p  build/release; cd build/release
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_TESTS=OFF ../..
  make && sudo make install
  ```

#### Install OPENSSL 
This should already be installed, if not install with your package manager  

#### Install C-Ares  
  ```
  cmake files in "third_party/cares/cares"
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
  ```  
#### Build and Install GRPC
    ```
    mkdir build
    mkdir build/release
    cd build/release
    cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DgRPC_ZLIB_PROVIDER=package -DgRPC_CARES_PROVIDER=package -DgRPC_PROTOBUF_PROVIDER=package -DgRPC_SSL_PROVIDER=package ../..
    make
    sudo make install
    ```
    
