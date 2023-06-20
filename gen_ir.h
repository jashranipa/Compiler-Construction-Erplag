/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#include "symbolTable.h"

#define BLOCK_SIZE 500
#define LABEL_LEN 10
#define MAX_LENGTH 20
#define MAX_LABELS 50
// note array in type3 pending

struct quad_block* curr_block;
int curr_node;

enum operators {
    PLUS,MINUS,MUL,DIV,AND,OR,LT,LE,GE,GT,EQ,NE, 
    UNARY_MINUS,
    UNARY_PLUS, ASID, ASSIGNOP, ARRASSIGNOP,
    FOR, WHILE,
    ASARR,
    GET_VALUE, PRINT};

struct quad_node 
{
    enum operators op;
    char* arg1;
    char* arg2;
    char* res;
    struct ST * ST;
};

struct quad_block 
{
    struct quad_node* nodes; // array of quad_nodes
    struct quad_block* next;
};

struct quad_arr 
{
    struct quad_block* head;
    struct quad_block* tail;
    int curr;
};

// struct temp_labels
// {
//     struct temp_label[MAX_LABELS];
// };

// struct temp_label

struct quad_arr * setup_gen_ir();
struct quad_arr * gen_ir(struct TreeNode* ast_root);
struct quad_node * get_next_quad();
char * new_label();

/* GLOBAL VARIABLES */
struct quad_arr* qa;
int labels;