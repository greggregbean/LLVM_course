#include <stdio.h>

void func_start_logger (char* func){
    printf("-- START OF '%s'\n", func);
}

void func_end_logger (char* func, long int val_id){
    printf("-- END OF '%s' {%ld}\n", func, val_id);
}

void call_logger (char* callee, char* caller, long int val_id){
    printf("   CALL '%s' -> '%s' {%ld}\n", callee, caller, val_id);
}

void bin_op_logger (char* func, int val, int arg0, int arg1, char* op, long int val_id){
    printf("   %d = %d %s %d {%ld} in '%s'\n", val, arg0, op, arg1, val_id, func);
}
