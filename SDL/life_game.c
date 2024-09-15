#include "sim.h"

int field     [SIM_X_SIZE][SIM_Y_SIZE] = {0};
int new_field [SIM_X_SIZE][SIM_Y_SIZE] = {0};

void start_config () {
    int x, y;

    for (int i = 0; i < 52000; i++) {
        x = simRand() % SIM_X_SIZE;
        y = simRand() % SIM_Y_SIZE;
        field     [x][y] = 1;
        new_field [x][y] = 1;
    }
}

int count_alive_neighbors (int x, int y) {
    int alive_neighbors = 0;

    for (int x_n = x-1; x_n <= x+1; x_n++) {
        if (x_n < 0 || x_n >= SIM_X_SIZE) 
            continue;

        for (int y_n = y-1; y_n <= y+1; y_n++) {
            if (y_n < 0 || y_n >= SIM_Y_SIZE)
                continue;
            
            if (field[x_n][y_n])
                alive_neighbors++;
        }
    }

    return alive_neighbors;
}

void change_dead_cell (int x, int y) {
    int alive_neighbors = count_alive_neighbors (x, y);

    if (alive_neighbors == 3)
        new_field [x][y] = 1;
}

void change_alive_cell (int x, int y) {
    int alive_neighbors = count_alive_neighbors (x, y);

    if (alive_neighbors > 3 || alive_neighbors < 2)
        new_field [x][y] = 0;
}

void run_game () {
    while (1) {
        for (int x = 0; x < SIM_X_SIZE; x++) {
            for (int y = 0; y < SIM_Y_SIZE; y++) {
                if (field[x][y] == 1) {
                    simPutPixel(x, y, 0x800080);
                    change_alive_cell (x, y);
                }

                else {
                    simPutPixel(x, y, 0xFFFFFF);
                    change_dead_cell (x, y);
                }    
            }
        }

        for (int x = 0; x < SIM_X_SIZE; x++) {
            for (int y = 0; y < SIM_Y_SIZE; y++) {
                field [x][y] = new_field [x][y];
            }
        }

        simFlush();
    } 
}

void app () {
    start_config ();
    run_game();
}