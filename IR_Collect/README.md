# LLVM Pass example
This LLVM pass inserts calls to loggers, which dynamically print IR of executed instructions.


## Build:
```
clang++ -O2 ir_collect.cpp -fPIC -shared -I$(llvm-config --includedir) -o ir_collect.so
clang -O2 -fpass-plugin=./ir_collect.so loggers.c ../SDL/life_game.c -c
clang -O2 ../SDL/sim.c ../SDL/main.c -c
clang -O2 sim.o main.o loggers.o life_game.o -o run -lSDL2
```

## Run:
```
./run > ir.log
```

## Analyze:
```
python3 analyze.py
```
