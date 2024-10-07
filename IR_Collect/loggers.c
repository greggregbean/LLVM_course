#include <stdio.h>

void func_start_logger (char* func_name){
    printf("START OF '%s' {\n", func_name);
}

void bb_start_logger (char* func_name, long int bb_addr) {
    printf("   bb 0x%lx of '%s':\n", bb_addr, func_name);
}

void func_end_logger (char* func_name, long int inst_addr){
    printf("} END OF '%s';					        {0x%lx}\n", func_name, inst_addr);
}

void call_logger (char* callee, char* caller, long int inst_addr){
    printf("      call '%s' -> '%s';			    {0x%lx}\n", callee, caller, inst_addr);
}

void bin_op_logger (char* func_name, int val, int arg0, int arg1, char* bin_op_name, long int inst_addr){
    printf("      %d = %d %s %d;				    {0x%lx} in '%s'\n", val, arg0, bin_op_name, arg1, inst_addr, func_name);
}

void load_logger (char* func_name, int val, long int p_operand, long int inst_addr){
    printf("      %d = load 0x%lx;					{0x%lx} in '%s'\n", val, p_operand, inst_addr, func_name);
}

void store_logger (char* func_name, int v_operand, long int p_operand, long int inst_addr){
    printf("      store %d, 0x%lx;					{0%lx} in '%s'\n", v_operand , p_operand, inst_addr, func_name);
}

void cast_logger (char* func_name, int val, int operand, char* cast_name, long int inst_addr){
    printf("      %d = %s %d;					    {0x%lx} in '%s'\n", val, cast_name, operand, inst_addr, func_name);
}

void unreachable_logger (char* func_name, long int inst_addr){
    printf("      unreachable;					    {0x%lx} in '%s'\n", inst_addr, func_name);
}

void uncond_br_logger (char* func_name, long int dest_addr, long int inst_addr) {
    printf("      br label 0x%lx;				    {0x%lx} in '%s'\n", dest_addr, inst_addr, func_name);
}

void cond_br_logger (char* func_name, int cond, long int true_dest_addr, long int false_dest_addr, long int inst_addr) {
    printf("      br %d, label 0x%lx, label 0x%lx;	{0x%lx} in '%s'\n", cond, true_dest_addr, false_dest_addr, inst_addr, func_name);
}

void alloca_logger (char* func_name, int val, long int inst_addr){
    printf("      %d = alloca;					    {0x%lx} in '%s'\n", val, inst_addr, func_name);
}

void icmp_logger (char* func_name, int val, char* p_name, int lhs, int rhs, long int inst_addr){
    printf("      %d = icmp %s %d, %d			    {0x%lx} in '%s'\n", val, p_name, lhs, rhs, inst_addr, func_name);
}

void getelementptr_logger (char* func_name, int val, int ptr_op, long int inst_addr){
    printf("      %d = getelementptr %d			    {0x%lx} in '%s'\n", val, ptr_op, inst_addr, func_name);
}
