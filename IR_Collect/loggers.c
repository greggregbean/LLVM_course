#include <stdio.h>

void func_start_logger (char* func_name){
    printf("START OF '%s'\n", func_name);
}

void func_end_logger (char* func_name, long int val_id){
    printf("END OF '%s' {%ld}\n", func_name, val_id);
}

void call_logger (char* callee, char* caller, long int val_id){
    printf("   CALL '%s' -> '%s' {%ld}\n", callee, caller, val_id);
}

void bin_op_logger (char* func_name, int val, int arg0, int arg1, char* bin_op_name, long int val_id){
    printf("   %d = %d %s %d {%ld} in '%s'\n", val, arg0, bin_op_name, arg1, val_id, func_name);
}

void load_logger (char* func_name, int val, int p_operand, char* load_name, long int val_id){
    printf("   %d = %s %d {%ld} in '%s'\n", val, load_name, p_operand, val_id, func_name);
}

void store_logger (char* func_name, int v_operand, int p_operand, char* store_name, long int val_id){
    printf("   %s %d %d {%ld} in '%s'\n", store_name, v_operand , p_operand, val_id, func_name);
}

void cast_logger (char* func_name, int val, int operand, char* cast_name, long int val_id){
    printf("   %d = %s %d {%ld} in '%s'\n", val, cast_name, operand, val_id, func_name);
}
