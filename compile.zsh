#!/etc/profiles/per-user/forest/bin/zsh

set -e

source compileshaders.zsh

if [ "$1" = "release" ]; then
    cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
else
    cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
fi

pushd build
make -j 10
popd
./build/SimplePlatformer
