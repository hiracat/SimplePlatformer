#!/etc/profiles/per-user/forest/bin/zsh

set -e

source compileshaders.zsh

if [ "$1" = "release" ]; then
    cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
elif [ "$1" = "data" ]; then
    cmake -B build -S . -DCMAKE_BUILD_TYPE=Data
else
    cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
fi

pushd build
bear --append --output ../compile_commands.json -- make -j 10
popd

./build/SimplePlatformer
