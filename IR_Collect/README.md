# LLVM Pass example
This LLVM pass inserts calls to loggers, which dynamically print IR of executed instructions.


## Usage:
```
clang++ ir_collect.cpp -fPIC -shared -I`llvm-config --includedir` -o ir_collect.so
clang -O2 -lSDL2 -Xclang -load -Xclang ./ir_collect.so ../SDL/main.c ../SDL/sim.c ../SDL/life_game.c loggers.c  -flegacy-pass-manager
./a.out > ir.log
```

## Ir dump:
```
clang -Xclang -load -Xclang ./ir_collect.so ../SDL/main.c ../SDL/sim.c ../SDL/life_game.c -emit-llvm -S -flegacy-pass-manager
```

## Analyze:
```
python3 analyze.py
```
