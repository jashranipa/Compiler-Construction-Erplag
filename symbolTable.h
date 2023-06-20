/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#include "AST.h"
struct primitive_type 
{
    char p_type;
};

struct array_type 
{
    int is_dynamic;
    char a_type;
    int l;
    int r;
    char * left;
    char * right;
};

union param_type
{
    struct primitive_type pt;
    struct array_type at;
};

struct list_node
{
    char * key;
    char identifier; // A - AT, P - PT
    union param_type dt;
    struct list_node* next;
};

union data_type
{
    struct primitive_type pt;

    struct array_type at;

    struct ST* child;
};

union parent
{
    struct ST* ST;
    struct FT_entry* FT_entry;
};

struct FT
{
    struct FT_entry* entry; // array of FT_entry
    int size;
};

// 0 (N,N), 1 (N,Y), 2 (Y,N), 3 (Y,Y)
// (defination, declaration)
struct FT_entry
{
    char *key;
    struct list_node* head_ip;
    struct list_node* head_op;
    struct ST* child;
    int deleted;
    int scope_start;
    int scope_end;
    int flag;
    int offset;
};

struct ST
{
    struct ST_entry* entry; // array of ST_entry
    char identifier;
    union parent par; 
    int size;
    struct ST * next;
    int scope_start;
    int scope_end;
};

struct ST_entry
{
    char *key;
    char identifier;  // S-Symbol table, A - Array, P - Primitive
    union data_type dt;
    unsigned long long int width;
    unsigned long long int offset;
    int deleted;
};
struct ST_entry * extract_type(char * id, struct ST * ST);
int contains_ft(char * key, struct FT* FT);
int insert_ft(char * key, struct FT * FT);

int contains_st(char * key, struct ST* ST);
int insert_st(char * key, struct ST* ST);
// int delete_st(char* key, struct ST* ST);

struct ST* symbol_table_constructor();

struct FT* function_table_constructor();

void print_function_table(struct FT * function_table);

void print_symbol_table(struct FT * symbol_table);

void print_array_table(struct FT * function_table);

void activation_record_size(struct FT* function_table);

void make_symbol_table(struct TreeNode* node, struct ST* symbol_table, int nested_level, struct FT*function_table);

char semantic_correctness_flag;

FILE* ptr;