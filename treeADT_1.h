/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#include<stdio.h>
#include<stdlib.h>
#include"lexer.h"


// struct symbol
// {
//     int num;
//     char type;
// };
/*struct node contains char * key which is for debugging purposes*/

struct TreeNode{
    unsigned int line_num;
    unsigned int rule;
    union variant property;
    int scope_start;
    int scope_end;
    struct symbol sym;
    char* key;
    char * label;
    struct TreeNode* rightsib;
    struct TreeNode* child;
    struct TreeNode* parent;

    struct TreeNode* syn_addr;
    struct TreeNode* inh_addr;
    
    // for gen_ir
    char visited;
    char* nt_label;
    struct ST * ST;
};
struct Tree{ 
    struct TreeNode* root;
};

/*FUNCTION PROTOTYPES */
void insert_child(struct TreeNode* curr,tk newtoken,char *key,struct symbol newsym);
void insert_right(struct TreeNode* curr,tk newtoken,char *key,struct symbol newsym);
struct TreeNode* go_down(struct TreeNode* curr);
struct TreeNode* go_right(struct TreeNode* curr);
struct TreeNode* go_up(struct TreeNode* curr);
void inorder_print(struct TreeNode *curr,FILE *ptr);
void printParseTree(struct Tree* mytree, char* fparsetree);
void count_nodes(struct TreeNode* node, int* count);
