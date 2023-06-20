/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#include<stdio.h>
#include "code_gen.h"

void generate_code(FILE * ptr, struct FT * FT, struct quad_arr * quad_arr)
{
    if(!quad_arr->head->nodes)
    {
        printf("Error:\n");
    }
    // printf("%d\n", quad_arr->curr);
    // printf("%d, %s, %s, %s\n",quad_arr->head->nodes[0].op, quad_arr->head->nodes[0].arg1, quad_arr->head->nodes[0].arg2, quad_arr->head->nodes[0].res);
    // printf("Reached here\n");
    fprintf(ptr, "extern printf, scanf, exit\n");
    fprintf(ptr, "section .data\n");
    fprintf(ptr, "int: db \"%%d\", 0\n");
    fprintf(ptr, "real: db \"%%lf\", 0\n");
    fprintf(ptr, "bool: db \"%%d\", 0\n");
    fprintf(ptr, "str: db \"%%s\",10, 0\n");
    fprintf(ptr, "str_no_new_line: db \"%%s\", 0\n");
    fprintf(ptr, "section .text\n");
    fprintf(ptr, ";Push all registers\n");
    fprintf(ptr, "%%macro push_all 0\npush RAX\npush RBX\npush RCX\npush RDX\npush RSP\npush RBP\npush RSI\npush RDI\n%%endmacro\n\n");
    fprintf(ptr, ";Pop all registers\n");
    fprintf(ptr, "%%macro pop_all 0\npop RDI\npop RSI\npop RBP\npop RSP\npop RDX\npop RCX\npop RBX\npop RAX\n%%endmacro\n\n");
    fprintf(ptr, "%%macro print_str 1\nsection .data\n%%%%str db %%1,0\nsection .text\npush_all\nmov RDI, str\nmov RSI, %%%%str\nmov RAX,0\nalign_16_rsp\ncall printf\nremove_align_16_rsp\npop_all\n%%endmacro\n\n");
    fprintf(ptr, "%%macro print_str_no_new_line 1\nsection .data\n%%%%str_no_new_line db %%1,0\nsection .text\npush_all\nmov RDI, str_no_new_line\nmov RSI, %%%%str\nmov RAX,0\nalign_16_rsp\ncall printf\nremove_align_16_rsp\npop_all\n%%endmacro\n\n");
    fprintf(ptr, "%%macro align_16_rsp 0 \npush RBX\npush RDX\npush RAX\nmov RDX, 0\nmov RAX, RSP\nmov RBX, 16\nidiv RBX\nmov RBX, RDX\npop RAX\npop RDX\nsub RSP, RBX\n%%endmacro\n");
    fprintf(ptr, "%%macro remove_align_16_rsp 0\nadd RSP, RBX\npop RBX\n%%endmacro\n");
    int entry_no = contains_ft("DRIVER", FT);
    int act_memory = FT->entry[entry_no].offset;
    fprintf(ptr,"global main\nmain:");
    fprintf(ptr, "enter %d, 0\n", act_memory);
    // int act_memory = get_activation_memory("DRIVER");
    // printf("In generate_code\n");
    while(1)
    {
        struct quad_node * quadruple = get_next_quad();
        if(quadruple == NULL)
        {
            // printf("NULL quadruple\n");
            break;
        }
        else
        {
            // printf("quadruple: %s, %s, %s\n", quadruple->arg1, quadruple->arg2, quadruple->res);
            switch(quadruple->op)
            {
                case PLUS:
                {
                    char * arg1 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    char * arg2 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    struct ST * cur_ST = quadruple->ST;
                    if(contains_st(quadruple->arg1,cur_ST) != -1)
                    {
                        // arg1 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg1,cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg1, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg1, cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg1 = str_copy(quadruple->arg1);
                    }
                    if(contains_st(quadruple->arg2,quadruple->ST) != -1)
                    {
                        // arg2 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg2,cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg2, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg2, cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg2 = str_copy(quadruple->arg2);
                    }
                    int entry_no = contains_st(quadruple->res, cur_ST);
                    int offset = cur_ST->entry[entry_no].offset;
                    fprintf(ptr, "push_all\n");
                    fprintf(ptr, "mov RAX, %s\nmov RBX, %s\nadd RAX, RBX\nmov [RBP - %d], RAX\npop_all\n", arg1, arg2, offset);
                    break;
                }
                case MINUS:
                {
                    char * arg1 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    char * arg2 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    struct ST * cur_ST = quadruple->ST;
                    if(contains_st(quadruple->arg1,cur_ST) != -1)
                    {
                        // arg1 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg1,cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg1, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg1, cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg1 = str_copy(quadruple->arg1);
                    }
                    if(contains_st(quadruple->arg2,quadruple->ST) != -1)
                    {
                        // arg2 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg2,cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg2, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg2, cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg2 = str_copy(quadruple->arg2);
                    }
                    int entry_no = contains_st(quadruple->res, cur_ST);
                    int offset = cur_ST->entry[entry_no].offset;
                    fprintf(ptr, "push_all\n");
                    fprintf(ptr, "mov RAX, %s\nmov RBX, %s\nsub RAX, RBX\nmov [RBP - %d], RAX\npop_all\n", arg1, arg2, offset);
                    break;
                }
                case MUL:
                {
                    char * arg1 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    char * arg2 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    struct ST * cur_ST = quadruple->ST;
                    // if(!cur_ST)
                    // {
                    //     printf("NULL ST\n");
                    // }
                    // printf("IN MUL in code_gen: %s\n", quadruple->arg1);
                    if(contains_st(quadruple->arg1,cur_ST) != -1)
                    {
                        // arg1 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg1,cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg1, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg1, cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg1 = str_copy(quadruple->arg1);
                    }
                    if(contains_st(quadruple->arg2,quadruple->ST) != -1)
                    {
                        // arg2 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg2,cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg2, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg2, cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg2 = str_copy(quadruple->arg2);
                    }
                    int entry_no = contains_st(quadruple->res, cur_ST);
                    int offset = cur_ST->entry[entry_no].offset;
                    fprintf(ptr, "push_all\n");
                    fprintf(ptr, "mov RAX, %s\nmov RBX, %s\nmul RBX\nmov [RBP - %d], RAX\npop_all\n", arg1, arg2, offset);
                    break;
                }
                case DIV:
                {
                    char * arg1 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    char * arg2 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    struct ST * cur_ST = quadruple->ST;
                    if(contains_st(quadruple->arg1,cur_ST) != -1)
                    {
                        // arg1 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg1,cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg1, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg1, cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg1 = str_copy(quadruple->arg1);
                    }
                    if(contains_st(quadruple->arg2,quadruple->ST) != -1)
                    {
                        // arg2 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg2,cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg2, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg2, cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg2 = str_copy(quadruple->arg2);
                    }
                    int entry_no = contains_st(quadruple->res, cur_ST);
                    int offset = cur_ST->entry[entry_no].offset;
                    fprintf(ptr, "push_all\n");
                    fprintf(ptr, "mov RAX, %s\nmov RBX, %s\ndiv RBX\nmov [RBP - %d], RAX\npop_all\n", arg1, arg2, offset);
                    break;
                }
                case AND:
                {
                    char * arg1 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    char * arg2 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    struct ST * cur_ST = quadruple->ST;
                    if(contains_st(quadruple->arg1,cur_ST) != -1)
                    {
                        // arg1 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg1,cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg1, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg1, cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg1 = str_copy(quadruple->arg1);
                    }
                    if(contains_st(quadruple->arg2,quadruple->ST) != -1)
                    {
                        // arg2 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg2,cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg2, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg2, cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg2 = str_copy(quadruple->arg2);
                    }
                    int entry_no = contains_st(quadruple->res, cur_ST);
                    int offset = cur_ST->entry[entry_no].offset;
                    fprintf(ptr,"push_all\n");
                    fprintf(ptr, "mov RAX, %s\nmov RBX, %s\nand RAX, RBX\nmov [RBP - %d], RAX\n", arg1,arg2,offset);
                    fprintf(ptr,"pop_all\n");
                    break;
                }
                case OR:
                {
                    char * arg1 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    char * arg2 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    struct ST * cur_ST = quadruple->ST;
                    if(contains_st(quadruple->arg1,cur_ST) != -1)
                    {
                        // arg1 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg1,cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg1, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg1, cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg1 = str_copy(quadruple->arg1);
                    }
                    if(contains_st(quadruple->arg2,quadruple->ST) != -1)
                    {
                        // arg2 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg2,cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg2, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg2, cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg2 = str_copy(quadruple->arg2);
                    }
                    int entry_no = contains_st(quadruple->res, cur_ST);
                    int offset = cur_ST->entry[entry_no].offset;
                    fprintf(ptr,"push_all\n");
                    fprintf(ptr, "mov RAX, %s\nmov RBX, %s\nor RAX, RBX\nmov [RBP - %d], RAX\n", arg1,arg2,offset);
                    fprintf(ptr,"pop_all\n");
                    break;
                }
                case LT:
                {
                    char * arg1 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    char * arg2 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    struct ST * cur_ST = quadruple->ST;
                    if(contains_st(quadruple->arg1,cur_ST) != -1)
                    {
                        // arg1 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg1,cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg1, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg1, cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg1 = str_copy(quadruple->arg1);
                    }
                    if(contains_st(quadruple->arg2,quadruple->ST) != -1)
                    {
                        // arg2 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg2,cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg2, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg2, cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg2 = str_copy(quadruple->arg2);
                    }
                    int entry_no = contains_st(quadruple->res, cur_ST);
                    int offset = cur_ST->entry[entry_no].offset;
                    fprintf(ptr,"push_all\n");
                    fprintf(ptr, "mov RAX, %s\nmov RBX, %s\nmov RCX, 0\nmov RDX, 1\ncmp RAX, RBX\ncmovl RCX, RDX\nmov [RBP - %d], RCX\n", arg1,arg2,offset);
                    fprintf(ptr,"pop_all\n");
                    break;
                }
                case LE:
                {
                    char * arg1 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    char * arg2 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    struct ST * cur_ST = quadruple->ST;
                    if(contains_st(quadruple->arg1,cur_ST) != -1)
                    {
                        // arg1 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg1,cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg1, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg1, cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg1 = str_copy(quadruple->arg1);
                    }
                    if(contains_st(quadruple->arg2,quadruple->ST) != -1)
                    {
                        // arg2 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg2,cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg2, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg2, cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg2 = str_copy(quadruple->arg2);
                    }
                    int entry_no = contains_st(quadruple->res, cur_ST);
                    int offset = cur_ST->entry[entry_no].offset;
                    fprintf(ptr,"push_all\n");
                    fprintf(ptr, "mov RAX, %s\nmov RBX, %s\nmov RCX, 0\nmov RDX, 1\ncmp RAX, RBX\ncmovle RCX, RDX\nmov [RBP - %d], RCX\n", arg1,arg2,offset);
                    fprintf(ptr,"pop_all\n");
                    break;
                }
                case GT:
                {
                    char * arg1 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    char * arg2 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    struct ST * cur_ST = quadruple->ST;
                    if(contains_st(quadruple->arg1,cur_ST) != -1)
                    {
                        // arg1 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg1,cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg1, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg1, cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg1 = str_copy(quadruple->arg1);
                    }
                    if(contains_st(quadruple->arg2,quadruple->ST) != -1)
                    {
                        // arg2 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg2,cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg2, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg2, cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg2 = str_copy(quadruple->arg2);
                    }
                    int entry_no = contains_st(quadruple->res, cur_ST);
                    int offset = cur_ST->entry[entry_no].offset;
                    fprintf(ptr,"push_all\n");
                    fprintf(ptr, "mov RAX, %s\nmov RBX, %s\nmov RCX, 0\nmov RDX, 1\ncmp RAX, RBX\ncmovg RCX, RDX\nmov [RBP - %d], RCX\n", arg1,arg2,offset);
                    fprintf(ptr,"pop_all\n");
                    break;
                }
                case GE:
                {
                    char * arg1 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    char * arg2 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    struct ST * cur_ST = quadruple->ST;
                    if(contains_st(quadruple->arg1,cur_ST) != -1)
                    {
                        // arg1 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg1,cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg1, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg1, cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg1 = str_copy(quadruple->arg1);
                    }
                    if(contains_st(quadruple->arg2,quadruple->ST) != -1)
                    {
                        // arg2 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg2,cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg2, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg2, cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg2 = str_copy(quadruple->arg2);
                    }
                    int entry_no = contains_st(quadruple->res, cur_ST);
                    int offset = cur_ST->entry[entry_no].offset;
                    fprintf(ptr,"push_all\n");
                    fprintf(ptr, "mov RAX, %s\nmov RBX, %s\nmov RCX, 0\nmov RDX, 1\ncmp RAX, RBX\ncmovge RCX, RDX\nmov [RBP - %d], RCX\n", arg1,arg2,offset);
                    fprintf(ptr,"pop_all\n");
                    break;
                }
                case NE:
                {
                    char * arg1 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    char * arg2 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    struct ST * cur_ST = quadruple->ST;
                    if(contains_st(quadruple->arg1,cur_ST) != -1)
                    {
                        // arg1 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg1,cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg1, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg1, cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg1 = str_copy(quadruple->arg1);
                    }
                    if(contains_st(quadruple->arg2,quadruple->ST) != -1)
                    {
                        // arg2 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg2,cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg2, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg2, cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg2 = str_copy(quadruple->arg2);
                    }
                    int entry_no = contains_st(quadruple->res, cur_ST);
                    int offset = cur_ST->entry[entry_no].offset;
                    fprintf(ptr,"push_all\n");
                    fprintf(ptr, "mov RAX, %s\nmov RBX, %s\nmov RCX, 0\nmov RDX, 1\ncmp RAX, RBX\ncmovne RCX, RDX\nmov [RBP - %d], RCX\n", arg1,arg2,offset);
                    fprintf(ptr,"pop_all\n");
                    break;
                }
                case EQ:
                {
                    char * arg1 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    char * arg2 = (char *)malloc(sizeof(char)*MAX_LENGTH);
                    struct ST * cur_ST = quadruple->ST;
                    if(contains_st(quadruple->arg1,cur_ST) != -1)
                    {
                        // arg1 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg1,cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg1, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg1, cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg1 = str_copy(quadruple->arg1);
                    }
                    if(contains_st(quadruple->arg2,quadruple->ST) != -1)
                    {
                        // arg2 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg2,cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg2, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg2, cur_ST);
                        snprintf(arg2, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg2 = str_copy(quadruple->arg2);
                    }
                    int entry_no = contains_st(quadruple->res, cur_ST);
                    int offset = cur_ST->entry[entry_no].offset;
                    fprintf(ptr,"push_all\n");
                    fprintf(ptr, "mov RAX, %s\nmov RBX, %s\nmov RCX, 0\nmov RDX, 1\ncmp RAX, RBX\ncmove RCX, RDX\nmov [RBP - %d], RCX\n", arg1,arg2,offset);
                    fprintf(ptr,"pop_all\n");
                    break;
                }
                case ASSIGNOP:
                {
                    char * arg1 = (char *)malloc(sizeof(char)* MAX_LENGTH);
                    struct ST * cur_ST = quadruple->ST;
                    if(contains_st(quadruple->arg1,cur_ST) != -1)
                    {
                        // arg1 = quadruple->ST_entry->offset;
                        int entry_no = contains_st(quadruple->arg1,cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", cur_ST->entry[entry_no].offset);
                    }
                    else if(extract_type(quadruple->arg1, cur_ST) != NULL)
                    {
                        struct ST_entry * entry = extract_type(quadruple->arg1, cur_ST);
                        snprintf(arg1, MAX_LENGTH, "[RBP - %lld]", entry->offset);
                    }
                    else
                    {
                        arg1 = str_copy(quadruple->arg1);
                    }
                    // else
                    // {
                    //     int offset = get_temp_offset(quadruple->arg1);
                    //     snprintf(arg1, MAX_LENGTH, "[RBP - %d]", offset);
                    // }
                    int offset;
                    struct ST_entry * res = extract_type(quadruple->res, cur_ST);
                    // int entry_no = contains_st(quadruple->res, quadruple->ST);
                    offset = res->offset;
                    // if(contains_st(quadruple->res, quadruple->ST) != -1)
                    // {
                    // }
                    // else
                    // {
                    //     offset = get_temp_offset(quadruple->res);
                    // }
                    fprintf(ptr, "push_all\n");
                    fprintf(ptr, "mov RAX, %s\nmov [RBP - %d], RAX\n", arg1,offset);
                    fprintf(ptr, "pop_all\n");
                    break;
                }
                case ARRASSIGNOP:
                {
                    // res = arg1[arg2]

                    int entry_no1 = contains_st(quadruple->arg1, quadruple->ST);
                    int entry_no2 = contains_st(quadruple->arg2, quadruple->ST);
                    int entry_no3 = contains_st(quadruple->res, quadruple->ST);
                    int arg2_offset = quadruple->ST->entry[entry_no2].offset;
                    int base_offset = quadruple->ST->entry[entry_no1].offset;
                    int res_offset = quadruple->ST->entry[entry_no3].offset;
                    base_offset+=8;
                    fprintf(ptr, "push_all\n");
                    int l = quadruple->ST->entry[entry_no1].dt.at.l;
                    fprintf(ptr, "mov RAX, [RBP - %d]\nmov RBX, %d\nsub RAX,RBX\nmov RBX, 8\nmul RBX\nmov RBX, %d\nadd RAX, RBX\nmov RDX,RBP\nsub RDX,RAX\nmov RAX,[RDX]\n\nmov [RBP - %d], RAX\n", arg2_offset, l,base_offset,res_offset);
                    fprintf(ptr, "pop_all\n");
                    break;
                }
                case GET_VALUE:
                {
                    // int entry_no = contains_st(quadruple->res, quadruple->ST);
                    struct ST_entry * entry = extract_type(quadruple->res, quadruple->ST);
                    int offset = entry->offset;
                    printf("IN GET_VALUE in code gen\n");
                    if(entry->identifier == 'A')
                    {
                        if(entry->dt.at.a_type == 'I')
                        {
                            int l = entry->dt.at.l;
                            int r = entry->dt.at.r;
                            int size = r-l+1;
                            fprintf(ptr, "print_str \"Input: Enter %d array elements of integer type for range %d to %d\"\n", size, l, r);
                            offset+=8;
                            for(int i = 0; i < size; i++)
                            {
                                // fprintf(ptr, "mov RDI, int\nmov RSI, [RBP - %d]\nxor RAX, RAX\ncall scanf\n", offset);
                                fprintf(ptr, "push_all\n");
                                fprintf(ptr, "mov RDI, int\nmov RDX, RBP\nsub RDX, %d\nxor RAX, RAX\nmov [RDX], RAX\nmov RSI, RDX\nmov RAX, 0\nalign_16_rsp\ncall scanf\nremove_align_16_rsp\n", offset);
                                fprintf(ptr, "pop_all\n");
                                offset+=8;
                            }
                        }
                    }
                    else if(entry->dt.pt.p_type == 'I')
                    {
                        fprintf(ptr, "push_all\n");
                        fprintf(ptr, "print_str \"Input: Enter an integer value\"\n");
                        // fprintf(ptr, "mov RDI, int\nmov RSI, [RBP - %d]\nxor RAX, RAX\ncall scanf\n", offset);
                        fprintf(ptr, "mov RDI, int\nmov RDX, RBP\nsub RDX, %d\nxor RAX, RAX\nmov [RDX], RAX\nmov RSI, RDX\nmov RAX, 0\nalign_16_rsp\ncall scanf\nremove_align_16_rsp\n", offset);
                        fprintf(ptr, "pop_all\n");
                    }
                    break;
                }
                case PRINT:
                {
                    // int entry_no = contains_st(quadruple->res, quadruple->ST);
                    struct ST_entry * entry = extract_type(quadruple->res, quadruple->ST);
                    if((entry->identifier == 'P'))
                    {
                        int offset = entry->offset;
                        fprintf(ptr, "push_all\n");
                        if(entry->dt.pt.p_type == 'I')
                        {
                            fprintf(ptr, "print_str \"Output:\"\n");
                            fprintf(ptr, "mov RDI, int\nmov RSI, [RBP - %d]\nxor RAX, RAX\ncall printf\n", offset);
                        }
                        else if(entry->dt.pt.p_type == 'B')
                        {
                            char * false = new_label();
                            char * next = new_label();
                        // printf("IN PRINT in code gen\n");
                            fprintf(ptr,"mov RDI, bool\nmov RDX, RBP\nsub RDX, %d\nmov RAX, [RDX]\ncmp RAX, 0\njz %s\nprint_str \"Output: true\"\njmp %s\n%s:\nprint_str \"Output: false\"\n%s:\n", offset, false,next,false,next);
                        }
                        fprintf(ptr, "print_str \"\"\n");
                        fprintf(ptr, "pop_all\n");
                    }
                    else
                    {
                        int offset = entry->offset;
                        offset+=8;
                        // printf("%c: Type\n", entry->dt.at.a_type);
                        int l = entry->dt.at.l;
                        int r = entry->dt.at.r;
                        // printf("L: %d r: %d\n", l, r);
                        fprintf(ptr, "print_str \"Output Array: \"\n");
                        for(int i = l; i <= r; i++)
                        {
                            fprintf(ptr, "push_all\n");
                            if(entry->dt.at.a_type == 'I')
                            {
                                printf("%d: %d\n", i, offset);
                                fprintf(ptr, "mov RDI, int\nmov RSI, [RBP - %d]\nxor RAX, RAX\ncall printf\n", offset);
                            }
                            fprintf(ptr, "pop_all\n");
                            fprintf(ptr,"print_str \"\"\n");
                            offset+=8;
                        }
                    }
                }
            }
        }
    }
    fprintf(ptr, "call exit\n");
    fclose(ptr);
}