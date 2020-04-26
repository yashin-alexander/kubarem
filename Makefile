ASSIMP_SOURCES=thirdparty/assimp

all:
	mkdir -p build
	cmake -S . -B build
	cmake --build build

init:
	git submodule update --init --recursive
	git -C $(ASSIMP_SOURCES) checkout v5.0.1
	cmake -S $(ASSIMP_SOURCES) -B $(ASSIMP_SOURCES)
	make -C $(ASSIMP_SOURCES) -j4

clean:
	rm -rf build
	git clean -e thirdparty/* build/* CMakeFiles CMakeCache.txt -f
