[![Actions Status](https://github.com/yashin-alexander/kubarem/workflows/Linux%20build/badge.svg)](https://github.com/yashin-alexander/kubarem/actions)

## Kubarem

Things implemented:
 - OpenGL rendering;
 - Editor;
 - Python blueprint scripting;
 - Goal-Oriented Action Planning (GOAP) AI;
 - Audio playback;
 - Objects (de)serialization, ability to store game world to a file;
 - Particle system;
 - entt entity component system is used;


## Dependencies installation
```
apt-get install libsoil-dev libglm-dev libassimp-dev \
        libglew-dev libglfw3-dev libxinerama-dev \ 
        libxcursor-dev libxi-dev libasound2-dev
make init
```

## Run

To build and run *kubarem*'s world editor in its current state, use:
```
make && ./build/tools/editor
```

Also, a game target is available:
```
make && ./build/app/dummy
```

Controls:
In both editor and game binaries use `F5-F6` to switch the mouse cursor mode.

## Old demos
During the development there several **raw** demo examples were implemented. 
To build and play, use the following:
```
git checkout ${EXAMPLE_TAG}
make
./build/src/kubarem
```

-  `v0.3.1` - Katamari Damacy clone tag
- `v0.2` - Solar system simulation tag
- `v0.1` - Pong game tag

Pong controls:
```
Space - start game
W/S - first player movement
Up/Down arrows - second player movement
```
