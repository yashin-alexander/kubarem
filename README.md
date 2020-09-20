[![Actions Status](https://github.com/yashin-alexander/kubarem/workflows/Linux%20build/badge.svg)](https://github.com/yashin-alexander/kubarem/actions)


## Dependencies installation
```
apt-get install libsoil-dev libglm-dev libassimp-dev \
        libglew-dev libglfw3-dev libxinerama-dev \ 
        libxcursor-dev libxi-dev libasound2-dev
make init
```

### Examples
Several examples are available. To buld and playback, use the following:
```
git checkout ${EXAMPLE_HASH}
make
./build/src/kubarem
```

## Katamari Damacy clone

To build and playback:
```
git checkout v0.3
```

## Solar system simulation

To build and playback:
```
git checkout v0.2
```

## Pong game

To build and play:
```
git checkout v0.1
```

Controls:
```
Space - start game
W/S - first player movement
Up/Down arrows - second player movement
```
