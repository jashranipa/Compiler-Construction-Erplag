/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#include<stdio.h>
#include<stdlib.h>
#include"parserDef.h"


/*
Returns hash table entry of the given key
Searches hash table based on type 'T' = Tglobal and 'N' = NTglobal
*/
int get_value(char *key, char type)
{
    if(type == 'N')
    {
        return contains(key, NTglobal);
    }
    else if(type == 'T')
    {
        return contains(key, Tglobal);
    }
    return -1;
}


/*
Return struct symbol of the given key
Searches in both the hash tables
*/
struct symbol get_value_unknown(char * key)
{
    // struct symbol sym;
    // sym.num = 0;
    // sym.type = 0;
    struct symbol sym;
    int j = contains(key, NTglobal);
    if(j == -1)
    {
        j = contains(key, Tglobal);
        if(j == -1)
        {
            printf("!!!!! %s not in both hash tables !!!!!\n", key);
        }
        sym.num = j;
        sym.type = 'T';
    }
    else
    {
        sym.num = j;
        sym.type = 'N';
    }
    return sym;
}


char * string_copy(char src[], int limit)
{
    char * dest = malloc((limit+1)*sizeof(char));
    for(int i = 0; i < limit; i++)
    {
        dest[i] = src[i];
        // printf("%c", dest[i]);
    }
    dest[limit] = '\0';
    return dest;
}

/*
Populates non terminals hash table
*/
void populate_NT()
{
    FILE * NT = fopen("non_terminals.txt", "r"); 
    struct hash_table * non_terminals_HT = (struct hash_table *)malloc(sizeof(struct hash_table));
    non_terminals_HT->size = NON_TERMINALS_COUNT;
    non_terminals_HT->entry = (struct hash_table_item *)malloc((NON_TERMINALS_COUNT)*sizeof(struct hash_table_item));
    for(int i = 0; i < non_terminals_HT->size; i++)
    {
        (non_terminals_HT->entry[i]).deleted = -1;
    }
    char buffer[MAX_SYMBOL_LENGTH];
    int index = 0;
    while(1)
    {
        char ch = fgetc(NT);
        if(ch == '\n' || ch == '\r' || ch == -1)
        {
            // printf("D\n");
            if(ch=='\r')continue;
            char * key = string_copy(buffer, index);
            int num = insert(key, non_terminals_HT);
            // printf("%s, %d, %d\n",key, find_ASCII_sum(key),num);
            // printf("%s, %d\n",key,num);
            index = 0;
            if(ch==-1)
            {
                break;
            }
        }
        else
        {
            // printf("%c\n", ch);
            buffer[index++] = ch;
        }
    }
    fclose(NT);
    NTglobal = non_terminals_HT;
}

/*
Populates terminals hash table
*/
void populate_T()
{
    FILE * T = fopen("terminals.txt", "r"); 
    struct hash_table * terminals_HT = (struct hash_table *)malloc(sizeof(struct hash_table));
    terminals_HT->size = TERMINALS_COUNT;
    terminals_HT->entry = (struct hash_table_item *)malloc(TERMINALS_COUNT*sizeof(struct hash_table_item));
    for(int i=0; i<terminals_HT->size;i++)
    {
        (terminals_HT->entry[i]).deleted = -1;
    }
    char buffer[MAX_SYMBOL_LENGTH];
    int index = 0;
    while(1)
    {
        char ch = fgetc(T);
        if(ch == '\n' || ch == '\r' || ch == -1)
        {
            if(ch=='\r')continue;
            char * key = string_copy(buffer, index);
            int num = insert(key, terminals_HT);
            // printf("%s, %d, %d\n",key, find_ASCII_sum(key),num);
            index = 0;
            if(ch==-1)
            {
                break;
            }
        }
        else
        {
            buffer[index++] = ch;
        }
    }
    fclose(T);
    Tglobal = terminals_HT;
}

void print_grammar(struct node * grammar[])
{
    for(int i = 0; i < RULES_COUNT; i++)
    {
        struct node * temp = grammar[i];
        printf("Rule No: %d ", i);
        while(temp != NULL)
        {
            char * x = temp->key;
            printf("%d --> ", (temp->sym).num);
            temp = temp->next;
        }
        printf("NULL \n");
    }
}

void make_grammar(FILE * fp)
{
    int rule_no = 0;
    int index = 0; 
    struct node * head, * tail;
    char sym[MAX_SYMBOL_LENGTH];

    while(rule_no < RULES_COUNT)
    {
        char ch = fgetc(fp);

        if(ch == ' ' || ch == '\n' || ch == '\r' || ch == -1)
        {
            if(ch == '\r')continue;
            char * key = string_copy(sym, index);
            index = 0;
            struct symbol s = get_value_unknown(key);
            // printf("%s, %d",key, s.num);
            if(!global_grammar[rule_no])
            {
                struct node * head = malloc(sizeof(struct node));
                head->sym = s;
                head->key = key;
                head->next = NULL;
                tail = head;
                global_grammar[rule_no] = head;
            }
            else
            {
                struct node * temp = malloc(sizeof(struct node));
                temp->sym = s;
                temp->key = key;
                temp->next = NULL;
                tail->next = temp;
                tail = tail->next;
            }
            if(ch=='\n' || ch == -1)
            {
                rule_no++;
                head = NULL;
            }
        }
        else
        {
            sym[index++] = ch;
        }
    }
}

/*make_grammar.c portion ends*/

struct node * get_rule(int rule_no)
{
    return global_grammar[rule_no];
}

int get_parse_table_entry(int row, int col)
{
    return global_parse_table[row][col];   
}
/* first_follow.c portion starts (Doesn't contain portion of make_grammar.c)*/

int isTerminal(struct node* head)
{
    if((head->sym).type == 'T')
    {
        return 1;
    }
    return 0;
}

void setBit(unsigned long long int** first_set, int rule_no, struct node* head)
{
    int number = (head->sym).num;

    int idx = number/64;
    int bit_number = 63 - ((number) - (idx*64));
    first_set[rule_no][idx] = (first_set[rule_no][idx] | (((unsigned long long int)1)<<bit_number)); 
    return;
}

void unionBit(unsigned long long int** destination,int dst,unsigned long long int** source, int src)
{
    for(int i=0;i<SIZE;i++)
    {
        destination[dst][i] = destination[dst][i] | source[src][i];
    }

    // if 
}

int containEpsilon(unsigned long long int** first_set,int rule_no)
{
    if((first_set[rule_no][0] & (((unsigned long long int)1)<<63))==0)
    {
        return 0;
    }
    return 1;
}


void computeFirstSet1(int rule_no,struct node * grammar[],unsigned long long int** first_set,int rule_visited[],struct int_node* non_terminals_lhs[])
{
    // base case
    // printf("%d\n",rule_no);
    // use dp

    if(rule_visited[rule_no]==1)
    {
        return;
    }

    struct node* head = grammar[rule_no];
    head = head -> next;

    while(head != NULL)
    {
        if(isTerminal(head) == 1)
        {
            setBit(first_set,rule_no,head);
            // printf("debug: %d\n",(head->sym).num);
            break;
        }  
        else
        {
            int num = (head->sym).num;

            // replace for loop by deep code

            struct int_node* head1 = non_terminals_lhs[num];

            while(head1 != NULL)
            {
                if((head1->num) == rule_no)
                {
                    continue;
                }



                computeFirstSet1(head1->num,grammar,first_set,rule_visited,non_terminals_lhs);
                
                unionBit(first_set,rule_no,first_set,head1->num);
                
                head1 = head1->next;
            }
            
            if(containEpsilon(first_set,rule_no)==0)
            {
                break;
            }
            else
            {
                first_set[rule_no][0] = (first_set[rule_no][0] & (~(((unsigned long long int)1)<<63))); // making epsilon bit unset
            }

        }  

        head = head -> next;
    }

    if(head == NULL)
    {
        first_set[rule_no][0] = (first_set[rule_no][0] | (((unsigned long long int)1)<<63));
    }

    rule_visited[rule_no] = 1;

    // print_function(first_set,rule_no); // debug

    return;

}

void computeFirstSet2(unsigned long long int** first_set1,unsigned long long int** first_set2,struct int_node* non_terminals_lhs[])
{
    for(int i=0;i<NON_TERMINALS_COUNT;i++)
    {   
        struct int_node* head = non_terminals_lhs[i];

        while(head!=NULL)
        {
            // for(int j=0;j<SIZE;j++)
            // {
            //     first_set2[i][j] = (first_set2[i][j]) | (first_set1[(head->num)][j]);
            // }
            unionBit(first_set2,i,first_set1,(head->num));
            head = head -> next;
        }
    }
    return;
}


// int cnt = 0; // added to debug
void computeFollowSet(int non_terminal_num,unsigned long long int** follow_set, unsigned long long int** first_set2,struct node * grammar[],int non_terminal_visited[],struct int_node* non_terminals_rhs[])
{
    // base case


    // cnt++;
    // if(cnt>=10)
    // {
    //     return;
    // }
    // printf("%d\n",non_terminal_num);
    // use dp

    if(non_terminal_visited[non_terminal_num]==1)
    {
        return;
    }
    non_terminal_visited[non_terminal_num] = 1;
    // add rule : <program'> --> <program> $

    struct int_node* head = non_terminals_rhs[non_terminal_num];

    while(head != NULL)
    {
        struct node* head1 = grammar[(head->num)];
        // printf("%d\n", head->num);
        head1 = head1 -> next;
        int f = 0;
        while(head1 != NULL)
        {
            if((((head1->sym).num != non_terminal_num) || ((head1->sym).type == 'T')) && (f==0))
            {
                head1 = head1 -> next; 
                continue;
            }
            if((f==0) && ((head1->sym).num == non_terminal_num) && ((head1->sym).type == 'N'))
            {
                f=1;
                head1 = head1 -> next;
                continue;
            }

            if((head1->sym).type == 'N')
            {
                unionBit(follow_set,non_terminal_num,first_set2,(head1->sym).num);
                if(containEpsilon(first_set2,(head1->sym).num) == 0)
                {
                    f=0;
                }
                else
                {
                    follow_set[non_terminal_num][0] = (follow_set[non_terminal_num][0] & (~(((unsigned long long int)1)<<63))); // making epsilon bit unset
                }
            }
            else if((head1->sym).type == 'T')
            {
                setBit(follow_set,non_terminal_num,head1);
                f=0;
            }
            head1 = head1 -> next;
        }

        if((head1 == NULL) && (f==1))
        {
            struct node* head2 = grammar[(head->num)];
            // printf("%d, %d\n",(head2->sym).num,head->num);
            computeFollowSet((head2->sym).num,follow_set,first_set2,grammar,non_terminal_visited,non_terminals_rhs);

            unionBit(follow_set,non_terminal_num,follow_set,(head2->sym).num);
        }

        head = head -> next; 
    } 

    // print_function2(follow_set,non_terminal_num,NTglobal,Tglobal);
    return;
}

/*first_follow.c portion ends*/

/*Linked list insert at first position*/
struct int_node * insert_first(struct int_node * head, int value)
{
    struct int_node * new_head = (struct int_node *)malloc(sizeof(struct int_node));
    new_head->next = head;
    new_head->num = value;
    return new_head;
}


/*Filling non_terminals_lhs and non_terminals_rhs portion starts*/

void compute_non_terminal_lhs(struct node * grammar[], struct int_node * non_terminals_lhs[])
{
    for(int i = 0; i < RULES_COUNT; i++)
    {
        int value = (grammar[i]->sym).num;
        // struct node * head = non_terminals_lhs[i];
        non_terminals_lhs[value] = insert_first(non_terminals_lhs[value], i);
    }
}

void compute_non_terminal_rhs(struct node * grammar[], struct int_node * non_terminals_rhs[])
{
    for(int i = 0; i < RULES_COUNT; i++)
    {
        struct node * search = grammar[i]->next;
        while(search)
        {
            int value = (search->sym).num;
            if((search->sym).type == 'N')
            {
                non_terminals_rhs[value] = insert_first(non_terminals_rhs[value], i);
            }
            search = search->next;
        }
    }
}

/*Filling non_terminals_lhs and non_terminals_rhs portion ends*/

void print_function(unsigned long long int** first,int rule_no,struct hash_table * NT)
{   
    printf("%d: ",rule_no);
    for(int i=0;i<SIZE;i++)
    {
        int cnt = 0;
        while(cnt <= 63)
        {
            if((first[rule_no][i] & ((unsigned long long int)1<<cnt)) != 0)
            {
                printf("%s, ", (NT->entry[(63 - ((i*64)+cnt))]).key);
            }
            cnt++;
        }
    }
    printf("\n");
    return;
}

void print_function2(unsigned long long int** first,int rule_no,struct hash_table * NT,struct hash_table * T)
{   
    printf("%s: ",(NT->entry[rule_no]).key);
    for(int i=0;i<SIZE;i++)
    {
        int cnt = 0;
        while(cnt <= 63)
        {
            if((first[rule_no][i] & ((unsigned long long int)1<<cnt)) != 0)
            {
                printf("%s, ", (T->entry[(63 - ((i*64)+cnt))]).key);
            }
            cnt++;
        }
    }
    printf("\n");
    return;
}

void fn(int non_terminal_num,unsigned long long int** parsetable,int idx, unsigned long long int** first,int rule_no)
{
    for(int i=0;i<SIZE;i++)
    {
        int cnt = 0;
        while(cnt <= 63)
        {
            if((first[idx][i] & ((unsigned long long int)1<<(63-cnt))) != 0)
            {
                // printf("Enter 2 : %s", (Tglobal->entry[(64*i)+cnt]).key);
                if(parsetable[non_terminal_num][(64*i)+cnt] != -1)
                {
                    printf("NOT in LL1 !!!!!! \n");
                }
                parsetable[non_terminal_num][(64*i)+cnt] = rule_no;
            }
            cnt++;
        }
    }
    return;
}

void make_parsetable(unsigned long long int** first,unsigned long long int** follow_set,struct node * grammar[],unsigned long long int** parsetable)
{
    for(int i=0;i<RULES_COUNT;i++)
    {
        fn((grammar[i]->sym).num,parsetable,i,first,i);
        if(containEpsilon(first,i) == 1)
        {
            // printf("enter1");
            fn((grammar[i]->sym).num,parsetable,(grammar[i]->sym).num,follow_set,i);
        }
    }
    return;
}

int is_present_Follow(int non_terminal_num,int terminal_num)
{
    int idx = terminal_num/64;
    int cnt = terminal_num%64;

    if((follow_set[non_terminal_num][idx] & ((unsigned long long int)1<<(63-cnt)))!=0)
    {
        return 1;
    }
    return 0;
}

void setup_parser()
{
    /*Reading non_terminals.txt and initialising non_terminals hashtable starts*/

    populate_NT();

    /*Reading non_terminals.txt and initialising non_terminals hashtable ends*/

    /*Reading terminals.txt and initialising non_terminals hashtable starts*/

    populate_T();

    /*Reading terminals.txt and initialising non_terminals hashtable ends*/


    /*make_grammar.c portion starts*/
 
    FILE * fp = fopen("grammar.txt", "r");

    for(int i = 0; i < RULES_COUNT; i++) global_grammar[i] = NULL;

    make_grammar(fp);

    // print_grammar(global_grammar);

    /*make_grammar.c portion ends*/

    /*first_follow.c portion starts*/

    // extra
    struct int_node* non_terminals_lhs[NON_TERMINALS_COUNT];
    struct int_node* non_terminals_rhs[NON_TERMINALS_COUNT];
    for(int i = 0; i < NON_TERMINALS_COUNT; i++)
    {
        non_terminals_lhs[i] = NULL;
        non_terminals_rhs[i] = NULL;
    }

    /*Filling non_terminals_lhs and non_terminals_rhs starts*/

    compute_non_terminal_lhs(global_grammar, non_terminals_lhs);
    compute_non_terminal_rhs(global_grammar, non_terminals_rhs);

    // for(int i=0;i<NON_TERMINALS_COUNT;i++)
    // {
    //     struct int_node* head = non_terminals_lhs[i];
    //     while(head!=NULL)
    //     {
    //         printf("%d --> ", head->num);
    //         head = head->next;
    // // printf("Hi\n");
    //     }
    //     printf("NULL %d\n", i);
    // }

    // for(int i=0;i<NON_TERMINALS_COUNT;i++)
    // {
    //     struct int_node* head = non_terminals_rhs[i];
    //     while(head!=NULL)
    //     {
    //         printf("%d --> ", head->num);
    //         head = head->next;
    //     }
    //     printf("NULL %d\n", i);
    // }
    /* Deep Code */
    
    unsigned long long int** first_set1 = malloc(RULES_COUNT * sizeof(unsigned long long int*));
    for(int i=0;i<RULES_COUNT;i++)
    {
        first_set1[i] = malloc(SIZE * sizeof(unsigned long long int));
    }
    // first_set1[RULES_COUNT][SIZE]
    
    unsigned long long int** first_set2 = malloc(NON_TERMINALS_COUNT * sizeof(unsigned long long int*));
    for(int i=0;i<NON_TERMINALS_COUNT;i++)
    {
        first_set2[i] = malloc(SIZE * sizeof(unsigned long long int));
    }
    // first_set2[NON_TERMINALS_COUNT][SIZE]
    
    follow_set = malloc(NON_TERMINALS_COUNT * sizeof(unsigned long long int*));
    for(int i=0;i<NON_TERMINALS_COUNT;i++)
    {
        follow_set[i] = malloc(SIZE * sizeof(unsigned long long int));
    }
    // follow_set[NON_TERMINALS_COUNT][SIZE]

    int rule_visited[RULES_COUNT];
    for(int i = 0; i < RULES_COUNT; i++)
    {
        rule_visited[i] = 0;
    }
    int non_terminal_visited[NON_TERMINALS_COUNT];
    for(int i = 0; i < NON_TERMINALS_COUNT; i++)
    {
        non_terminal_visited[i] = 0;
    }  

    for(int i=0;i<RULES_COUNT;i++)
    {
        computeFirstSet1(i,global_grammar,first_set1,rule_visited,non_terminals_lhs);
    }

    computeFirstSet2(first_set1,first_set2,non_terminals_lhs);
        
    for(int i=0;i<NON_TERMINALS_COUNT;i++)
    {
        computeFollowSet(i,follow_set,first_set2,global_grammar,non_terminal_visited,non_terminals_rhs);
    }
    
    /*
    
    // For Printing First and Follow Set

    for(int i=0;i<RULES_COUNT;i++)
    {
        print_function(first_set1,i,terminals_HT);
    }
    printf("\n\n\n\n\n");

    for(int i=0;i<NON_TERMINALS_COUNT;i++)
    {
        print_function2(first_set2,i,non_terminals_HT,terminals_HT);
    }
    printf("\n\n\n\n\n");

    for(int i=0;i<NON_TERMINALS_COUNT;i++)
    {
        print_function2(follow_set,i,non_terminals_HT,terminals_HT);
    }

    printf("\n\n\n");

    */


    global_parse_table = malloc(NON_TERMINALS_COUNT * sizeof(unsigned long long int*));
    for(int i=0;i<NON_TERMINALS_COUNT;i++)
    {
        global_parse_table[i] = malloc(TERMINALS_COUNT * sizeof(unsigned long long int));
    }
    for(int i=0;i<NON_TERMINALS_COUNT;i++)
    {
        for(int j=0;j<TERMINALS_COUNT;j++)
        {
            global_parse_table[i][j] = -1;
        }
    }
    // parsetable[NON_TERMINALS_COUNT][TERMINALS_COUNT]
    
    make_parsetable(first_set1,follow_set,global_grammar,global_parse_table);

    

    // print parse table 

        // for(int i=0;i<NON_TERMINALS_COUNT;i++)
        // {
        //     printf("%s:: ", (NTglobal->entry[i]).key);
        //     for(int j=0;j<TERMINALS_COUNT;j++)
        //     {

        //         printf("%s: %lld, ",(Tglobal->entry[j]).key,global_parse_table[i][j]);
        //     }
        //     printf("\n\n\n\n");
        // }
    

    


    printf("Finally!\n");
    /*first_follow.c portion ends*/
}