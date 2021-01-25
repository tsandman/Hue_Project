if [ -d build ]; then
    cd build
else
    mkdir build
    cd build
fi
export HUE_PROJECT_BUILD_DIR=$(pwd)

cmake -DCMAKE_BUILD_TYPE=Debug ../
cmake --build .
cd ..
