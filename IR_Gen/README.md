# LLVM IR Generator
This is IR genaration of my SDL Life Game app (which simply duplicates life_game.ll).

## Usage:
```
clang++ $(llvm-config --cppflags --ldflags --libs) ../SDL/sim.c ir_gen.cpp -o run -lSDL2
```

## Run:
```
./run > ir_gen.log
```