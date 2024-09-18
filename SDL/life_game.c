#include "sim.h"

void start_config (int* field, int* new_field) {
    int x, y;

    for (int i = 0; i < 52000; i++) {
        x = simRand() % SIM_X_SIZE;
        y = simRand() % SIM_Y_SIZE;
        field     [x + y*SIM_X_SIZE] = 1;
        new_field [x + y*SIM_X_SIZE] = 1;
    }
}

int count_alive_neighbors (int x, int y, int* field) {
    int alive_neighbors = 0;

    for (int x_n = x-1; x_n <= x+1; x_n++) {
        if (x_n < 0 || x_n >= SIM_X_SIZE) 
            continue;

        for (int y_n = y-1; y_n <= y+1; y_n++) {
            if (y_n < 0 || y_n >= SIM_Y_SIZE)
                continue;
            
            if (x_n == x && y_n == y)
                continue;
            
            if (field[x_n + y_n*SIM_X_SIZE])
                alive_neighbors++;
        }
    }

    return alive_neighbors;
}

void change_dead_cell (int x, int y, int* field, int* new_field) {
    int alive_neighbors = count_alive_neighbors (x, y, field);

    if (alive_neighbors == 3)
        new_field [x + y*SIM_X_SIZE] = 1;
}

void change_alive_cell (int x, int y, int* field, int* new_field) {
    int alive_neighbors = count_alive_neighbors (x, y, field);

    if (alive_neighbors > 3 || alive_neighbors < 2)
        new_field [x + y*SIM_X_SIZE] = 0;
}

void run_game (int* field, int* new_field) {
    while (1) {
        for (int x = 0; x < SIM_X_SIZE; x++) {
            for (int y = 0; y < SIM_Y_SIZE; y++) {
                if (field[x + y*SIM_X_SIZE] == 1) {
                    simPutPixel(x, y, 0x800080);
                    change_alive_cell (x, y, field, new_field);
                }

                else {
                    simPutPixel(x, y, 0xFFFFFF);
                    change_dead_cell (x, y, field, new_field);
                }    
            }
        }

        for (int x = 0; x < SIM_X_SIZE; x++) {
            for (int y = 0; y < SIM_Y_SIZE; y++) {
                field [x + y*SIM_X_SIZE] = new_field [x + y*SIM_X_SIZE];
            }
        }

        simFlush();
    } 
}

void app () {
    int field     [SIM_X_SIZE * SIM_Y_SIZE] = {0};
    int new_field [SIM_X_SIZE * SIM_Y_SIZE] = {0};
    start_config (field, new_field);
    run_game (field, new_field);
}