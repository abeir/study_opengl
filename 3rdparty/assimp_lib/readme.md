# build

安装以下步骤执行编译，需要注意，已经命令仅假设当前为类linux系统
```shell
cd assimp
mkdir build
cd build
cmake ..  -DASSIMP_BUILD_ZLIB=ON -DBUILD_SHARED_LIBS=OFF
cmake --build .

cp -rf lib/libassimp.a ../../assimp_lib
cp -rf include ../../assimp_lib
```
