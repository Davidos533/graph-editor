 #!/bin/bash
# build folder
rm -rf build
mkdir build

# build
cmake -G "Unix Makefiles" -S . -B build
make -C build

# putting compilation result
mkdir -p bin
cp -f build/graph-editor bin/graph-editor
cp res/font/MysteryQuest-Regular.ttf bin/MysteryQuest-Regular.ttf
cp res/logo/logo.png bin/logo.png
