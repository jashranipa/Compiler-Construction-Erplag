/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#include<stdio.h>
#include<stdlib.h>
#include"treeADT_1.h"

/*struct node contains char * key which is for debugging purposes*/
/* remove the char *key after debugging */
/* sym is redundant b/c TreeNode contains the sym */
struct stackADT
{
    struct stack_node* stack_top;
    int stack_size;
};
struct stack_node
{
    struct symbol *sym;
    char * key;
    struct TreeNode* address;
    struct stack_node* next;
};

/* FUNCTION PROTOTYPES          */
int isfull(struct stackADT* mystack);
int isempty(struct stackADT* mystack);
void pop(struct stackADT* mystack);
struct stack_node* top(struct stackADT* mystack);
char * str_copy(char *src);
void push(struct stackADT* mystack, struct symbol *new_sym, char * new_key,struct TreeNode* new_address);
