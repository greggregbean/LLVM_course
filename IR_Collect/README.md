# LLVM Pass example
This LLVM pass inserts calls to loggers, which dynamically print IR of executed instructions.


## Usage:
```
clang++ ir_collect.cpp -fPIC -shared -I`llvm-config --includedir` -o ir_collect.so
clang -lSDL2 -Xclang -load -Xclang ./ir_collect.so ../SDL/main.c ../SDL/sim.c ../SDL/life_game.c loggers.c  -flegacy-pass-manager
```

## Ir dump:
```
clang -Xclang -load -Xclang ./ir_collect.so ../SDL/main.c ../SDL/sim.c ../SDL/life_game.c -emit-llvm -S -flegacy-pass-manager
```
