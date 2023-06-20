/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#include "hashtableADT.h"
#define RULES_COUNT 140
#define MAX_SYMBOL_LENGTH 40
#define SIZE 4 // number of integers at one rule
#define NON_TERMINALS_COUNT  69 // automate 
#define TERMINALS_COUNT 58 // Automation required

struct symbol
{
    int num;
    char type;
};

/*
struct node contains char * key which is for debugging purposes
*/
struct node
{
    struct symbol sym;
    char * key;
    struct node * next;
};

struct int_node
{
    int num;
    struct int_node* next;
};

struct hash_table* NTglobal, *Tglobal;
struct node* global_grammar[RULES_COUNT]; // Assignment remaining
unsigned long long int ** global_parse_table; // Assignment remaining
unsigned long long int** follow_set;
/* e - erraneous, c - correct */
char correctnessFlag;

int get_value(char * key, char type);
struct symbol get_value_unknown(char * key);
struct node * get_rule(int rule_no);
int get_parse_table_entry(int row, int col);
int is_present_Follow(int non_terminal_num,int terminal_num);
void setup_parser();