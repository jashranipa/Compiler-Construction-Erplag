/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#include <stdio.h>
#include "symbolTable.h"

#define MAX_NUMBER_OF_FUNCTIONS 50
#define MAX_NUMBER_OF_SYMBOLS 100
// int global_index = 0;

// void push_back(int line, char *key)
// {
//     arr[global_index].line = line;
//     arr[global_index].key = str_copy(key);
//     global_index++;
// }

char * concat(char * s1, char * s2)
{
    int cnt = 0;
    int i = 0;
    while(s1[i]!='\0')
    {
        cnt++;
        i++;
    }
    i = 0;
    while(s2[i]!='\0')
    {
        cnt++;
        i++;
    }
    char * c = (char *)malloc(cnt * sizeof(char));
    i = 0;
    cnt = 0;
    while(s1[i]!='\0')
    {
        c[cnt] = s1[i];
        cnt++;
        i++;
    }
    i = 0;
    while(s2[i]!='\0')
    {
        c[cnt] = s2[i];
        cnt++;
        i++;
    }
    return c;
}
/*
Return index of function table entry if the key is found
Else returns -1 if not found
*/
int contains_ft(char * key, struct FT* FT)
{
    int sum = find_ASCII_sum(key);
    int i = 0;
    while(i < FT->size)
    {
        int j = (i+sum)%(FT->size);
        if((FT->entry[j].deleted == 0) && string_cmp(key, FT->entry[j].key))
        {
            return j;
        }
        else if(FT->entry[j].deleted == -1)
        {
            return -1;
        }
        i++;
    }
    return -1;
}

/*
Inserts the key into function table
Return -1 if not inserted
*/
int insert_ft(char * key, struct FT * FT)
{
    int j = contains_ft(key, FT);
    if(j != -1)
    {
        return j;
    }
    int sum = find_ASCII_sum(key);
    int i = 0;
    while(i < FT->size)
    {
        j = (sum+i)%(FT->size);
        if(FT->entry[j].deleted == 0)
        {
            i++;
        }
        else
        {
            FT->entry[j].key = str_copy(key);
            FT->entry[j].deleted = 0;
            return j;
        }
    }
    return -1;
}

/*
Return index of symbol table entry if the key is found
Else returns -1 if not found
*/
int contains_st(char * key, struct ST* ST)
{
    int sum = find_ASCII_sum(key);
    int i = 0;
    while(i < ST->size)
    {
        int j = (i+sum)%(ST->size);
        if((ST->entry[j].deleted == 0) && string_cmp(key, ST->entry[j].key))
        {
            return j;
        }
        else if(ST->entry[j].deleted == -1)
        {
            return -1;
        }
        i++;
    }
    return -1;
}

/*
Inserts the key into symbol table
Return -1 if not inserted
*/
int insert_st(char * key, struct ST* ST)
{
    int j = contains_st(key, ST);
    if(j != -1)
    {
        return j;
    }
    int sum = find_ASCII_sum(key);
    int i = 0;
    while(i < ST->size)
    {
        j = (sum+i)%(ST->size);
        if(ST->entry[j].deleted == 0)
        {
            i++;
        }
        else
        {
            ST->entry[j].key = str_copy(key);
            ST->entry[j].deleted = 0;
            return j;
        }
    }
    return -1;
}

struct ST* symbol_table_constructor()
{
    struct ST* symbol_table = (struct ST*)malloc(sizeof(struct ST));

    symbol_table->size = MAX_NUMBER_OF_SYMBOLS;
    symbol_table->entry = (struct ST_entry*)malloc(MAX_NUMBER_OF_SYMBOLS * sizeof(struct ST_entry));
    symbol_table->next = NULL;
    symbol_table->scope_start = 0;
    symbol_table->scope_end = 0;
    for(int i=0;i<MAX_NUMBER_OF_SYMBOLS;i++)
    {
        (symbol_table->entry[i]).key = NULL;
        (symbol_table->entry[i]).width = 0;
        (symbol_table->entry[i]).offset = 0;
        (symbol_table->entry[i]).deleted = -1;
        (symbol_table->entry[i]).identifier = '\0';
    }
    return symbol_table;
}

struct FT* function_table_constructor()
{
    struct FT* function_table = (struct FT*)malloc(sizeof(struct FT));

    function_table->size = MAX_NUMBER_OF_FUNCTIONS;
    function_table->entry = (struct FT_entry*)malloc(MAX_NUMBER_OF_FUNCTIONS * sizeof(struct FT_entry));

    for(int i=0;i<MAX_NUMBER_OF_FUNCTIONS;i++)
    {
        (function_table->entry[i]).key = NULL;
        (function_table->entry[i]).child = NULL;
        (function_table->entry[i]).head_ip = NULL;
        (function_table->entry[i]).head_op = NULL;
        (function_table->entry[i]).deleted = -1;
        (function_table->entry[i]).scope_start = 0;
        (function_table->entry[i]).scope_end = 0;
        (function_table->entry[i]).flag = 0;
        (function_table->entry[i]).offset = 0;
    }

    return function_table;
}

struct TreeNode * search_module_definition(char * module_name, struct TreeNode * AST_root)
{
    struct TreeNode * module_node = AST_root->child->rightsib->child;
    struct TreeNode * ret_node = NULL;
    int found = 0;
    while(module_node)
    {
        if(string_cmp(module_name, module_node->child->property.lexeme))
        {
            if(found)
            {
                // Double definition of the module
                return ret_node;
            }
            else
            {
                found = 1;
                ret_node = module_node;
            }
        }
        module_node = module_node->rightsib;
    }
    module_node = AST_root->child->rightsib->rightsib->rightsib->child;
    while(module_node)
    {
        if(string_cmp(module_name, module_node->child->property.lexeme))
        {
            if(found)
            {
                // Double definition of the module
                return ret_node;
            }
            else
            {
                found = 1;
                ret_node = module_node;
            }
        }
        module_node = module_node->rightsib;
    }
    if(!ret_node)
    {
        // No definition of the module
        return NULL;
    }
    return ret_node;
}

struct list_node * create_list_node(struct TreeNode * node)
{
    //node - IPARAM or OPARAM
    struct list_node * list_node = (struct list_node *)malloc(sizeof(struct list_node));
    list_node->next=NULL;
    list_node->key = str_copy(node->child->property.lexeme);
    if(string_cmp(node->child->rightsib->label, "ARRAY"))
    {
        struct TreeNode * L = node->child->rightsib->child;
        struct TreeNode * R = L->rightsib;
        struct TreeNode * type = R->rightsib;
        list_node->dt.at.is_dynamic = 0;
        list_node->dt.at.left = NULL;
        list_node->dt.at.right = NULL;
        list_node->dt.at.a_type = (type->key)[0];
        if(string_cmp(L->child->key, "ID"))
        {
            list_node->dt.at.is_dynamic = 1;
            list_node->dt.at.left = str_copy(L->child->property.lexeme);
        }
        else if(string_cmp(L->child->key, "NUM"))
        {
            list_node->dt.at.l = L->child->property.value_int;
        }
        else if(string_cmp(L->child->key, "PLUS"))
        {
            if(string_cmp(L->child->rightsib->key, "ID"))
            {
                list_node->dt.at.is_dynamic = 1;
                char * ch = concat("+", L->child->property.lexeme);
                list_node->dt.at.left = str_copy(ch);
            }
            else
            {
                list_node->dt.at.l = L->child->rightsib->property.value_int;
            }
        }
        else if(string_cmp(L->child->key, "MINUS"))
        {
            if(string_cmp(L->child->rightsib->key, "ID"))
            {
                list_node->dt.at.is_dynamic = 1;
                char * ch = concat("-", L->child->property.lexeme);
                list_node->dt.at.left = str_copy(ch);
            }
            else
            {
                list_node->dt.at.l = (L->child->rightsib->property.value_int)*(-1);
            }
        }


        if(string_cmp(R->child->key, "ID"))
        {
            list_node->dt.at.is_dynamic = 1;
            list_node->dt.at.right = str_copy(R->child->property.lexeme);
        }
        else if(string_cmp(R->child->key, "NUM"))
        {
            list_node->dt.at.r = R->child->property.value_int;
        }
        else if(string_cmp(R->child->key, "PLUS"))
        {
            if(string_cmp(R->child->rightsib->key, "ID"))
            {
                list_node->dt.at.is_dynamic = 1;
                char * ch = concat("+", R->child->property.lexeme);
                list_node->dt.at.right = str_copy(ch);
            }
            else
            {
                list_node->dt.at.r = R->child->rightsib->property.value_int;
            }
        }
        else if(string_cmp(R->child->key, "MINUS"))
        {
            if(string_cmp(R->child->rightsib->key, "ID"))
            {
                list_node->dt.at.is_dynamic = 1;
                char * ch = concat("-", R->child->property.lexeme);
                list_node->dt.at.right = str_copy(ch);
            }
            else
            {
                list_node->dt.at.r = (R->child->rightsib->property.value_int)*(-1);
            }
        }
        list_node->identifier = 'A';
    }
    else
    {
        // printf("Primitive %s\n", node->child->property.lexeme);
        list_node->dt.pt.p_type = (node->child->rightsib->key)[0];
        list_node->identifier = 'P';
    }
    return list_node;
}

struct list_node * make_list(struct TreeNode * node)
{
    if(!node)
    {
        return NULL;
    }
    struct list_node * head = create_list_node(node);
    struct list_node * tail = head;
    node = node->rightsib;
    while(node)
    {
        tail->next = create_list_node(node);
        tail = tail->next;
        node = node->rightsib;
    }
    return head;
}

void debug_print_list(struct list_node * node)
{
    while(node)
    {
        printf("{Key: %s, TYPE: ", node->key);
        if(node->identifier == 'A')
        {
            printf("ARRAY, IS_Dynamic: %d, L: %d, R: %d, Datatype: %c} , ", node->dt.at.is_dynamic, node->dt.at.l, node->dt.at.r, node->dt.at.a_type);
        }
        else
        {
            printf("Datatype: %c} , ", node->dt.pt.p_type);
        }
        node = node->next;
    }
    printf("\n");
}

// Print function for debugging
void print_function_table(struct FT * function_table)
{
    printf("Printing Function table starts\n");

    printf("Size : %d\n", function_table->size);
    int i = 0;
    while((i < function_table->size))
    {
        if((function_table->entry[i]).deleted == 0)
        {
            printf("Index: %d, Key: %s\n", i, function_table->entry[i].key);
            printf("Scope: [%d - %d]\n", function_table->entry[i].scope_start, function_table->entry[i].scope_end);
            printf("INPUT: ");
            debug_print_list(function_table->entry[i].head_ip);
            printf("OUTPUT: ");
            debug_print_list(function_table->entry[i].head_op);
        }
        i++;
    }
    
    printf("Printing Function table ends\n");
}


//Print offset remaining
void print_list(struct list_node * node, struct FT_entry entry)
{
    while(node)
    {
        printf("%s\t\t", node->key);

        printf("%s\t\t", entry.key);

        printf("[%d - %d]\t\t", entry.scope_start, entry.scope_end);

        /*----------------------------------------------*/
        if(node->identifier == 'A')
        {
            if(node->dt.at.a_type == 'I')
            {
                printf("integer\t\t");
            }
            else if(node->dt.at.a_type == 'R')
            {
                printf("real\t\t");
            }
            else
            {
                printf("boolean\t\t");
            }
        }
        else
        {
            if(node->dt.pt.p_type == 'I')
            {
                printf("integer\t\t");
            }
            else if(node->dt.pt.p_type == 'R')
            {
                printf("real\t\t");
            }
            else
            {
                printf("boolean\t\t");
            }
        }
        /*----------------------------------------------*/
        if(node->identifier == 'A')
        {
            printf("yes\t\t");
            if(node->dt.at.is_dynamic==1)
            {
                printf("dynamic\t\t");
                printf("[");
                if(node->dt.at.left == NULL)
                {
                    printf("%d, ", node->dt.at.l);
                }
                else
                {
                    printf("%s, ", node->dt.at.left);
                }
                if(node->dt.at.right == NULL)
                {
                    printf("%d]\t\t", node->dt.at.r);
                }
                else
                {
                    printf("%s]\t\t", node->dt.at.right);
                }
            }
            else
            {
                printf("static\t\t");
                printf("[%d, %d]\t\t", node->dt.at.l, node->dt.at.r);
            }
        }
        else
        {
            printf("no\t\t**\t\t**\t\t");
        }
        /*----------------------------------------------*/
        if(node->identifier == 'A')
        {
            int size = 0, w = 0;
            
            if(node->dt.at.is_dynamic==1)
            {
                printf("1\t\t");
            }
            else
            {
                size = (node->dt.at.r) - (node->dt.at.l) + 1;
                if(node->dt.at.a_type=='I')
                {
                    w = 2;
                } 
                else if(node->dt.at.a_type=='B')
                {
                    w = 1;
                }
                else
                {
                    w = 4;
                }
                printf("%d\t\t", (size*w)+1);
            }
            
        }
        else
        {
            if(node->dt.pt.p_type == 'I')
            {
                printf("2\t\t");
            }
            else if(node->dt.pt.p_type == 'R')
            {
                printf("4\t\t");
            }
            else
            {
                printf("1\t\t");
            }
        }
        /*----------------------------------------------*/
        printf("0\t\t");
        printf("0\t\t");
        node = node->next;
        printf("\n");
    }
    printf("\n");
}

void print_ST_entry(struct ST_entry entry, int nested_level, char * module_name, int scope_start, int scope_end)
{
    printf("%s\t\t", entry.key);

    printf("%s\t\t", module_name);

    printf("[%d - %d]\t\t", scope_start, scope_end);
    /*----------------------------------------------*/
    if(entry.identifier == 'A')
    {
        if(entry.dt.at.a_type == 'I')
        {
            printf("integer\t\t");
        }
        else if(entry.dt.at.a_type == 'R')
        {
            printf("real\t\t");
        }
        else
        {
            printf("boolean\t\t");
        }
    }
    else
        {
        if(entry.dt.pt.p_type == 'I')
        {
            printf("integer\t\t");
        }
        else if(entry.dt.pt.p_type == 'R')
        {
            printf("real\t\t");
        }
        else
        {
            printf("boolean\t\t");
        }
    }
    /*----------------------------------------------*/
    if(entry.identifier == 'A')
    {
        printf("yes\t\t");
        if(entry.dt.at.is_dynamic==1)
        {
            printf("dynamic\t\t");
            printf("[");
            if(entry.dt.at.left == NULL)
            {
                printf("%d, ", entry.dt.at.l);
            }
            else
            {
                printf("%s, ", entry.dt.at.left);
            }
            if(entry.dt.at.right == NULL)
            {
                printf("%d]\t\t", entry.dt.at.r);
            }
            else
            {
                printf("%s]\t\t", entry.dt.at.right);
            }
        }
        else
        {
            printf("static\t\t");
            printf("[%d, %d]\t\t", entry.dt.at.l, entry.dt.at.r);
        }
    }
    else
    {
        printf("no\t\t**\t\t**\t\t");
    }
    /*----------------------------------------------*/
    printf("%lld\t\t", entry.width);
    /*----------------------------------------------*/
    printf("%lld\t\t", entry.offset);
    printf("%d\t\t", nested_level);
    printf("\n");
}

void print_nested_symbol_table(struct ST * ST, int nested_level, char * module_name)
{
    int i = 0;
    while(i < ST->size)
    {
        if(ST->entry[i].deleted == 0)
        {
            if(ST->entry[i].identifier != 'S')
            {
                // printf("Normal ST entry: %d\n", i);
                print_ST_entry(ST->entry[i],nested_level, module_name, ST->scope_start, ST->scope_end);
            }
        }
        i++;
    }
    i = 0;
    while(i < ST->size)
    {
        if(ST->entry[i].deleted == 0)
        {
            if(ST->entry[i].identifier == 'S')
            {
                // printf("Nested symbol table, Entry No. %d\n", i);
                struct ST * temp = ST->entry[i].dt.child;
                while(temp)
                {
                    // printf("Not NULL symbol table\n");
                    print_nested_symbol_table(temp, nested_level+1, module_name);
                    temp = temp->next;
                }
            }
            // print_ST_entry(ST->entry[i],nested_level, module_name);
        }
        i++;
    }
}

// Print function for submission
void print_symbol_table(struct FT * function_table)
{
    printf("Variable Name \t\t Module Name \t\t Scope \t\t Type \t\t Is Array \t\t Static/Dynamic \t\t Array Range \t\t Width \t\t Offset \t\t Nesting Level\n");

    int i = 0;
    while((i < function_table->size))
    {
        if((function_table->entry[i]).deleted == 0)
        {
            // printf("Index: %d, Key: %s\n", i, function_table->entry[i].key);
            // printf("Scope: [%d - %d]\n", function_table->entry[i].scope_start, function_table->entry[i].scope_end);
            // printf("INPUT: ");
            print_list(function_table->entry[i].head_ip, function_table->entry[i]);
            // printf("OUTPUT: ");
            print_list(function_table->entry[i].head_op, function_table->entry[i]);

            print_nested_symbol_table(function_table->entry[i].child, 1, function_table->entry[i].key);
        }
        i++;
    }

}

/* Jash code starts */

// printf("Module Name \t\t Scope \t\t Variable Name \t\t Static/Dynamic \t\t Array Range \t\t Type \t\t\n");

void print_list_array(struct list_node * node, struct FT_entry entry)
{
    while(node)
    {
        if(node->identifier == 'A')
        {
            printf("%s\t\t", entry.key); // Module Name
            
            printf("[%d - %d]\t\t", entry.scope_start, entry.scope_end); // Scope
            
            printf("%s\t\t", node->key); // variable name
            
            /*----------------------------------------------*/
            
            if(node->dt.at.is_dynamic==1)
            {
                printf("Dynamic Array\t\t");
                printf("[");
                if(node->dt.at.left == NULL)
                {
                    printf("%d, ", node->dt.at.l);
                }
                else
                {
                    printf("%s, ", node->dt.at.left);
                }
                if(node->dt.at.right == NULL)
                {
                    printf("%d]\t\t", node->dt.at.r);
                }
                else
                {
                    printf("%s]\t\t", node->dt.at.right);
                }
            }
            else
            {
                printf("Static Array\t\t");
                printf("[%d, %d]\t\t", node->dt.at.l, node->dt.at.r);
            }
            
            /*----------------------------------------------*/
            
            if(node->dt.at.a_type == 'I')
            {
                printf("integer\t\t");
            }
            else if(node->dt.at.a_type == 'R')
            {
                printf("real\t\t");
            }
            else
            {
                printf("boolean\t\t");
            }

            /*----------------------------------------------*/


            printf("\n");
        }
        node = node->next;
    }
    printf("\n");
}

void print_ST_entry_array(struct ST_entry entry, int nested_level, char * module_name, int scope_start, int scope_end)
{
    if(entry.identifier=='A')
    {
        // printf("JASH CODE !!!!!!!!!!!!!\n");
        printf("%s\t\t", module_name);

        printf("[%d - %d]\t\t", scope_start, scope_end);
        
        printf("%s\t\t", entry.key);

        /*----------------------------------------------*/

        if(entry.dt.at.is_dynamic==1)
        {
            printf("Dynamic Array\t\t");
            printf("[");
            if(entry.dt.at.left == NULL)
            {
                printf("%d, ", entry.dt.at.l);
            }
            else
            {
                printf("%s, ", entry.dt.at.left);
            }
            if(entry.dt.at.right == NULL)
            {
                printf("%d]\t\t", entry.dt.at.r);
            }
            else
            {
                printf("%s]\t\t", entry.dt.at.right);
            }
        }
        else
        {
            printf("Static Array\t\t");
            printf("[%d, %d]\t\t", entry.dt.at.l, entry.dt.at.r);
        }



        /*----------------------------------------------*/
        
        if(entry.dt.at.a_type == 'I')
        {
            printf("integer\t\t");
        }
        else if(entry.dt.at.a_type == 'R')
        {
            printf("real\t\t");
        }
        else
        {
            printf("boolean\t\t");
        }
        
        
        printf("\n");
    }
}

void print_nested_symbol_table_array(struct ST * ST, int nested_level, char * module_name)
{
    int i = 0;
    while(i < ST->size)
    {
        if(ST->entry[i].deleted == 0)
        {
            if(ST->entry[i].identifier != 'S')
            {
                // printf("!!!!!!!!!!!!1 JASH Normal ST entry: %d\n", i);
                print_ST_entry_array(ST->entry[i],nested_level, module_name, ST->scope_start, ST->scope_end);
            }
        }
        i++;
    }
    i = 0;
    while(i < ST->size)
    {
        if(ST->entry[i].deleted == 0)
        {
            if(ST->entry[i].identifier == 'S')
            {
                // printf("Nested symbol table, Entry No. %d\n", i);
                struct ST * temp = ST->entry[i].dt.child;
                while(temp)
                {
                    // printf("Not NULL symbol table\n");
                    print_nested_symbol_table_array(temp, nested_level+1, module_name);
                    temp = temp->next;
                }
            }
            // print_ST_entry(ST->entry[i],nested_level, module_name);
        }
        i++;
    }
}

void print_array_table(struct FT * function_table)
{
    printf("Module Name \t\t Scope \t\t Variable Name \t\t Static/Dynamic \t\t Array Range \t\t Type \t\t\n");
    
    int i = 0;
    while((i < function_table->size))
    {
        if((function_table->entry[i]).deleted == 0)
        {
            // printf("Index: %d, Key: %s\n", i, function_table->entry[i].key);
            // printf("Scope: [%d - %d]\n", function_table->entry[i].scope_start, function_table->entry[i].scope_end);
            // printf("INPUT: ");
            print_list_array(function_table->entry[i].head_ip, function_table->entry[i]);
            // printf("OUTPUT: ");
            print_list_array(function_table->entry[i].head_op, function_table->entry[i]);

            print_nested_symbol_table_array(function_table->entry[i].child, 1, function_table->entry[i].key);
        }
        i++;
    }

}

int compute_width_list(struct list_node* node, struct FT_entry entry)
{
    int ans = 0 , w = 0;
    while(node)
    {
        if(node->identifier == 'A')
        {
            int size = 0;
            size = (node->dt.at.r) - (node->dt.at.l) + 1;
            if(node->dt.at.a_type=='I')
            {
                w = 2;
            } 
            else if(node->dt.at.a_type=='B')
            {
                w = 1;
            }
            else
            {
                w = 4;
            }
            if(node->dt.at.is_dynamic == 1)
            {
                ans += 1;
            }
            else
            {
                ans += (1 + (size * w));
            }
        }
        else
        {
            if(node->dt.pt.p_type == 'I')
            {
                ans += 2;
            }
            else if(node->dt.pt.p_type == 'R')
            {
                ans += 4;
            }
            else
            {
                ans += 1;
            }
        }
        node = node->next;
    }
    return ans;
}

int compute_width_nested_symbol_table(struct ST * ST, int nested_level, char * module_name, int *cnt_ans)
{
    int i = 0;
    while(i < ST->size)
    {
        if(ST->entry[i].deleted == 0)
        {
            if(ST->entry[i].identifier != 'S')
            {
                // printf("Normal ST entry: %d\n", i);
                // print_ST_entry_array(ST->entry[i],nested_level, module_name, ST->scope_start, ST->scope_end);
                (*cnt_ans) += ST->entry[i].width;
            }
        }
        i++;
    }
    i = 0;
    while(i < ST->size)
    {
        if(ST->entry[i].deleted == 0)
        {
            if(ST->entry[i].identifier == 'S')
            {
                // printf("Nested symbol table, Entry No. %d\n", i);
                struct ST * temp = ST->entry[i].dt.child;
                while(temp)
                {
                    // printf("Not NULL symbol table\n");
                    compute_width_nested_symbol_table(temp, nested_level+1, module_name,cnt_ans);
                    temp = temp->next;
                }
            }
            // print_ST_entry(ST->entry[i],nested_level, module_name);
        }
        i++;
    }
}

void activation_record_size(struct FT* function_table)
{
    printf("Module\t\tTotal Width\t\t\n");
    int i = 0; 
    while((i < function_table->size))
    {
        if((function_table->entry[i]).deleted == 0)
        {
            printf("%s \t\t", function_table->entry[i].key);

            int ans = 0;
            ans += compute_width_list(function_table->entry[i].head_ip,function_table->entry[i]); 

            ans += compute_width_list(function_table->entry[i].head_op,function_table->entry[i]); 
            int * cnt_ans = &ans;
            compute_width_nested_symbol_table(function_table->entry[i].child, 1, function_table->entry[i].key,cnt_ans);
            // ans += *(cnt_ans);
            printf("%d \t\t\n", *cnt_ans);
        }
        i++;
    }
}

int module_entry_no(struct ST* symbol_table, struct FT* function_table)
{
    struct ST* temp = symbol_table;

    while(temp->identifier=='S')
    {
        temp = temp->par.ST; 
    }
    struct FT_entry* temp2 = temp->par.FT_entry;
    int ans = contains_ft(temp2->key,function_table);
    return ans;
}

void traverse_AST_2(struct list_node* list_node, struct TreeNode* stmt, char* visited)
{
    while(stmt != NULL)
    {
        if(string_cmp(stmt->label, "ASID"))
        {
            struct list_node* temp = list_node;
            int cnt = 0;
            while(temp != NULL)
            {
                if(string_cmp(stmt->child->property.lexeme,temp->key))
                {
                    visited[cnt] = 'm';
                }
                cnt++;
                temp = temp->next;
            }
        }
        else if(string_cmp(stmt->label, "MRS"))
        {
            struct TreeNode* idList = stmt->child->child;
            struct list_node* temp = list_node;

            while(idList != NULL)
            {
                int cnt = 0; temp = list_node;
                while(temp != NULL)
                {
                    if(string_cmp(temp->key,idList->property.lexeme))
                    {
                        visited[cnt] = 'm';
                    }
                    cnt++;
                    temp = temp -> next;
                }
                idList = idList->rightsib;
            }

        }
        else if(string_cmp(stmt->label, "FOR"))
        {
            traverse_AST_2(list_node, stmt->child->rightsib->rightsib->child,visited);
        }
        else if(string_cmp(stmt->label, "WHILE"))
        {
            traverse_AST_2(list_node, stmt->child->rightsib->child , visited);
        }
        else if(string_cmp(stmt->label, "SWITCH"))
        {
            struct TreeNode * case_node = stmt->child->rightsib->child;
            struct TreeNode * default_node = stmt->child->rightsib->rightsib->child;
            
            while(case_node != NULL)
            {
                traverse_AST_2(list_node, case_node->child->rightsib->child , visited);
                case_node = case_node->rightsib;
            }
            if(default_node)
            {
                default_node = default_node->child;
                traverse_AST_2(list_node, default_node,visited);
            }
            else
            {
                return;
            }
        }
        stmt = stmt->rightsib;
    }
}


void traverse_AST_3(struct TreeNode* stmt,struct hash_table * id_HT,char* char_ptr)
{
    while(stmt != NULL)
    {
        if(string_cmp(stmt->label, "ASID"))
        {
            int isPresent = contains( (stmt->child->property.lexeme) , id_HT);
            if(isPresent != -1)
            {
                *char_ptr = 'y';
            }
        }
        else if(string_cmp(stmt->label, "MRS"))
        {
            struct TreeNode* idList = stmt->child->child;
            
            while(idList != NULL)
            {
                int isPresent = contains( idList->property.lexeme , id_HT);
                if(isPresent != -1)
                {
                    *char_ptr = 'y';
                }
                idList = idList->rightsib;
            }

        }
        else if(string_cmp(stmt->label, "FOR"))
        {
            traverse_AST_3(stmt->child->rightsib->rightsib->child,id_HT,char_ptr);
        }
        else if(string_cmp(stmt->label, "WHILE"))
        {
            traverse_AST_3(stmt->child->rightsib->child , id_HT,char_ptr);
        }
        else if(string_cmp(stmt->label, "SWITCH"))
        {
            struct TreeNode * case_node = stmt->child->rightsib->child;
            struct TreeNode * default_node = stmt->child->rightsib->rightsib->child;
            
            while(case_node != NULL)
            {
                traverse_AST_3(case_node->child->rightsib->child , id_HT,char_ptr);
                case_node = case_node->rightsib;
            }
            if(default_node)
            {
                default_node = default_node->child;
                traverse_AST_3(default_node, id_HT,char_ptr);
            }
            else
            {
                return;
            }
        }
        stmt = stmt->rightsib;
    }
}


void while_expression_fill(struct hash_table* id_HT, struct TreeNode* node)
{
    if(node==NULL)
    {
        return;
    }

    if(string_cmp((node->key),"ID"))
    {
        insert(node->property.lexeme,id_HT);
    }
    else
    {
        if((node->child) != NULL)
        {
            while_expression_fill(id_HT,node->child);
            while_expression_fill(id_HT,node->child->rightsib);
        }
    }
}

void while_semantic_check(struct TreeNode* node , int line_num)
{
    struct hash_table * id_HT = (struct hash_table *)malloc(sizeof(struct hash_table));
    id_HT->size = 100;
    id_HT->entry = (struct hash_table_item *)malloc(100*sizeof(struct hash_table_item));
    for(int i=0; i<id_HT->size;i++)
    {
        (id_HT->entry[i]).deleted = -1;
    }

    while_expression_fill(id_HT, node->child);

    // printf("PRINT Start\n");
    // for(int i=0; i<id_HT->size;i++)
    // {
    //     if((id_HT->entry[i]).deleted == 0)
    //     {
    //         printf("%s\n", id_HT->entry[i].key);
    //     }
        
    // }
    // printf("PRINT ENDs\n");
    char to_check = 'n';
    char* char_ptr  = &to_check;
    traverse_AST_3(node->child->rightsib->child , id_HT,char_ptr);

    if(*char_ptr == 'n')
    {
        fprintf(ptr,"None of the condition variables get assigned any value of WHILE at line number %d\n", line_num);
        semantic_correctness_flag = 'e';
    }

}

/*
void function_parameters_change(struct TreeNode* node, struct FT* function_table)
{
    struct TreeNode* module_node = node->child->rightsib->child; // checking in other1
    while(module_node != NULL)
    {
        int entry_no = contains_ft(module_node->child->property.lexeme,function_table);
        printf("JASH ENTRY_NO: %d\n", entry_no);
        struct list_node* list_node = function_table->entry[entry_no].head_op;
        int line_num = function_table->entry[entry_no].scope_end;

        int size = 0;
        printf("JASH HI -1 !!!!!!!!!!!!!!!!!!\n");
        while(list_node != NULL)
        {
            size++;
            printf("!!!!!!!!!!! JASH: %s", list_node->key);
            list_node = list_node->next;
        }
        printf("JASH HI 0 !!!!!!!!!!!!!!!!!!\n");
        char* visited = (char*)malloc(size * sizeof(char));
        for(int i=0;i<size;i++)
        {
            visited[i] = 'n'; // m: modified  ,  n: not modified
        }

        list_node = function_table->entry[entry_no].head_op;
        printf("JASH HI 1 !!!!!!!!!!!!!!!!!!\n");
        traverse_AST_2(list_node , module_node->child->rightsib->rightsib->rightsib->child, visited);    
        printf("JASH HI 2 !!!!!!!!!!!!!!!!!!\n");
        int check_flag = 0;
        for(int i=0;i<size;i++)
        {
            if(visited[i]=='n')
            {
                check_flag += 1;
            }
        }

        if(check_flag >= 1)
        {
            printf("Output parameters: [");
            int i = 0; struct list_node* temp = list_node; 
            while(temp != NULL)
            {
                if(visited[i] == 'n')
                {
                    if(check_flag==1)
                    {
                        printf("%s ",temp->key);
                    }
                    else
                    {
                        printf(" %s, ",temp->key);
                    }
                    check_flag--;
                }
                temp = temp->next;
                i++;
            }
            printf("] is not assigned any value inside function %s at line number: %d\n", module_node->child->property.lexeme,line_num);
            semantic_correctness_flag = 'e';
        }

        module_node = module_node -> rightsib;
    }
    
    module_node = node->child->rightsib->rightsib->rightsib->child;  // checking in other2
    while(module_node != NULL)
    {
        int entry_no = contains_ft(module_node->child->property.lexeme,function_table);
        struct list_node* list_node = function_table->entry[entry_no].head_op;
        int line_num = function_table->entry[entry_no].scope_end;

        int size = 0;
        while(list_node != NULL)
        {
            size++;
            list_node = list_node->next;
        }
        
        char* visited = (char*)malloc(size * sizeof(char));
        for(int i=0;i<size;i++)
        {
            visited[i] = 'n'; // m: modified  ,  n: not modified
        }

        list_node = function_table->entry[entry_no].head_op;
        traverse_AST_2(list_node  , module_node->child->rightsib->rightsib->rightsib->child , visited);    

        int check_flag = 0;
        for(int i=0;i<size;i++)
        {
            if(visited[i]=='n')
            {
                check_flag += 1;
            }
        }

        if(check_flag >= 1)
        {
            printf("Output parameters: [");
            int i = 0; struct list_node* temp = list_node; 
            while(temp != NULL)
            {
                if(visited[i] == 'n')
                {
                    if(check_flag==1)
                    {
                        printf("%s ",temp->key);
                    }
                    else
                    {
                        printf(" %s, ",temp->key);
                    }
                    check_flag--;
                }
                temp = temp->next;
                i++;
            }
            printf("] is not assigned any value inside function %s at line number: %d\n", module_node->child->property.lexeme,line_num);
            semantic_correctness_flag = 'e';
        }
        module_node = module_node -> rightsib;
    }
}
*/

/* Jash code ends */

void free_symbol_table(struct ST_entry ST_entry)
{
    ST_entry.deleted = 1;
    ST_entry.identifier = 'S';
    ST_entry.key = NULL;
    free(ST_entry.dt.child);
}

void insert_ST_at_end(struct ST_entry * ST_entry, struct ST * ST)
{
    struct ST * temp = ST_entry->dt.child;
    // if(ST == NULL)
    // {
    //     printf("NULL ST passed to insert_ST_at_end\n");
    // }
    if(!temp)
    {
        // printf("In insert_ST_at_end\n");
        // print_nested_symbol_table(ST,2,"DRIVER");
        ST_entry->dt.child = ST;
        // if(ST_entry->dt.child == NULL)
        // {
        //     printf("Should not reach here\n");
        // }
        return;
    }
    while(temp->next)
    {
        temp = temp->next;
    }
    temp->next = ST;
}


// I - Integer, R - Real, B - Boolean
// J - Array Integer, S - Array Real, C - Array Boolean


// Checking in function table entry doubtful
// Assigning struct array_type directly - Works?
struct ST_entry * extract_type(char * id, struct ST * ST)
{
    struct ST * temp = ST;
    // if(!ST)
    // {
    //     printf("NULL ST\n");
    // }
    while(temp->identifier == 'S')
    {
        int entry_no = contains_st(id,temp);
        if(entry_no == -1)
        {
            temp = temp->par.ST;
        }
        else
        {
            return &(temp->entry[entry_no]);
        }
    }
    int entry_no = contains_st(id,temp);
    if(entry_no != -1)
    {
        return &(temp->entry[entry_no]);
    }
    else
    {
        struct FT_entry * FT_entry = temp->par.FT_entry;
        struct list_node * node = FT_entry->head_ip;
        while(node)
        {
            if(string_cmp(node->key, id))
            {
                // Constructing ST_entry;
                struct ST_entry * ST_entry = (struct ST_entry *) malloc(sizeof(struct ST_entry));
                ST_entry->deleted = 0;
                ST_entry->identifier = node->identifier;
                ST_entry->key = node->key;
                if(node->identifier == 'A')
                {
                    ST_entry->dt.at = node->dt.at;
                }
                else
                {
                    ST_entry->dt.pt = node->dt.pt;
                }
                return ST_entry;
            }
            node = node->next;
        }
        node = FT_entry->head_op;
        while(node)
        {
            if(string_cmp(node->key, id))
            {
                // Constructing ST_entry;
                struct ST_entry * ST_entry = (struct ST_entry *) malloc(sizeof(struct ST_entry));
                ST_entry->deleted = 0;
                ST_entry->identifier = node->identifier;
                ST_entry->key = node->key;
                if(node->identifier == 'A')
                {
                    ST_entry->dt.at = node->dt.at;
                }
                else
                {
                    ST_entry->dt.pt = node->dt.pt;
                }
                return ST_entry;
            }
            node = node->next;
        }
    }
    // printf("NOT DECLARED\n");
    return NULL;
}

char expression_type_check(struct TreeNode * node, struct ST * ST, int line_num)
{
    char child1,child2;
    if(string_cmp(node->label, "FACTOR"))
    {
        struct ST_entry * entry = extract_type(node->child->property.lexeme, ST);
        // if(contains_st(node->child->property.lexeme, ST))
        // {
        //     printf("YES\n");
        // }
        if(entry == NULL)
        {
            fprintf(ptr,"Variable %s not declared at line number: %d\n", node->child->property.lexeme, node->child->line_num);
            semantic_correctness_flag = 'e';
            return 'E';
        }
        else
        {
            if(node->child->rightsib == NULL)
            {
                if(entry->identifier == 'A')
                {
                    return (entry->dt.at.a_type+1);
                }
                else return entry->dt.pt.p_type;
            }
            else
            {
                if(entry->identifier == 'A')
                {
                    // Insert code for static bound checking
                    if(string_cmp(node->child->rightsib->child->key, "NUM"))
                    {
                        int index = node->child->rightsib->child->property.value_int;
                        if(entry->dt.at.is_dynamic == 0)
                        {
                            if(index < entry->dt.at.l || index > entry->dt.at.r)
                            {
                                fprintf(ptr,"Array variable: %s[%d] index out of bound at line number: %d\n",node->child->property.lexeme, index, node->child->line_num);
                                semantic_correctness_flag = 'e';
                                return 'E';
                            }
                        }
                    }
                    return entry->dt.at.a_type;
                }
                else
                {
                    fprintf(ptr,"ID index used but ID is not array at line num: %d\n", node->child->line_num);
                    semantic_correctness_flag = 'e';
                    return 'E';
                }
            }
        }
    }
    else if(string_cmp(node->key, "RNUM"))
    {
        return 'R';
    }
    else if(string_cmp(node->key, "NUM"))
    {
        return 'I';
    }
    else if(string_cmp(node->key, "TRUE") || string_cmp(node->key, "FALSE"))
    {
        return 'B';
    }
    else if(string_cmp(node->label, "UNARY_PLUS") || string_cmp(node->label, "UNARY_MINUS"))
    {
        child1 = expression_type_check(node->child, ST, line_num);
        return child1;
    }
    else if(string_cmp(node->label, "PLUS") || string_cmp(node->label, "MINUS"))
    {
        child1 = expression_type_check(node->child, ST, line_num);
        child2 = expression_type_check(node->child->rightsib, ST, line_num);
        if(child1 == 'E' || child2 == 'E')
        {
            return 'E';
        }
        else if(child1 == 'J' || child1 == 'S' || child1 == 'C' || child2 == 'J' || child2 == 'S' || child2 == 'C')
        {
            // printf("Array variables operation in expression at line number %d\n", line_num);
            fprintf(ptr,"Type mismatch error in expression at line number: %d\n", line_num);
            semantic_correctness_flag = 'e';
            return 'E';
        }
        else if(child1 == 'B' || child2 == 'B')
        {
            fprintf(ptr, "Type mismatch error in expression at line number: %d\n", line_num);
            semantic_correctness_flag = 'e';
            return 'E';
        }
        else if(child1 != child2)
        {
            fprintf(ptr, "Type mismatch error in expression at line number: %d\n", line_num);
            semantic_correctness_flag = 'e';
            return 'E';
        }
        return child1;
    }
    else if(string_cmp(node->label, "MUL"))
    {
        child1 = expression_type_check(node->child, ST, line_num);
        child2 = expression_type_check(node->child->rightsib, ST, line_num);
        if(child1 == 'E' || child2 == 'E')
        {
            return 'E';
        }
        else if(child1 == 'J' || child1 == 'S' || child1 == 'C' || child2 == 'J' || child2 == 'S' || child2 == 'C')
        {
            // printf("Array variables operation in expression at line number %d\n", line_num);
            fprintf(ptr,"Type mismatch error in expression at line number: %d\n", line_num);
            semantic_correctness_flag = 'e';
            return 'E';
        }
        else if(child1 == 'B' || child2 == 'B')
        {
            fprintf(ptr, "Type mismatch error in expression at line number: %d\n", line_num);
            semantic_correctness_flag = 'e';
            return 'E';
        }
        else if(child1 != child2)
        {
            fprintf(ptr, "Type mismatch error in expression at line number: %d\n", line_num);
            semantic_correctness_flag = 'e';
            return 'E';
        }
        return child1;
    }
    else if(string_cmp(node->label, "DIV"))
    {
        child1 = expression_type_check(node->child, ST, line_num);
        child2 = expression_type_check(node->child->rightsib, ST, line_num);
        if(child1 == 'E' || child2 == 'E')
        {
            return 'E';
        }
        else if(child1 == 'J' || child1 == 'S' || child1 == 'C' || child2 == 'J' || child2 == 'S' || child2 == 'C')
        {
            // printf("Array variables operation in expression at line number %d\n", line_num);
            fprintf(ptr,"Type mismatch error in expression at line number: %d\n", line_num);
            semantic_correctness_flag = 'e';
            return 'E';
        }
        else if(child1 == 'B' || child2 == 'B')
        {
            fprintf(ptr,"Type mismatch error in expression at line number: %d\n", line_num);
            semantic_correctness_flag = 'e';
            return 'E';
        }
        return 'R';
    }
    else if(string_cmp(node->label, "LT") || string_cmp(node->label, "LE") || string_cmp(node->label, "GT") || string_cmp(node->label, "GE") || string_cmp(node->label, "EQ") || string_cmp(node->label, "NE"))
    {
        child1 = expression_type_check(node->child, ST, line_num);
        child2 = expression_type_check(node->child->rightsib, ST, line_num);
        if(child1 == 'E' || child2 == 'E')
        {
            return 'E';
        }
        else if(child1 == 'J' || child1 == 'S' || child1 == 'C' || child2 == 'J' || child2 == 'S' || child2 == 'C' || child1 == 'B' || child2 == 'B')
        {
            fprintf(ptr,"Type mismatch error in expression at line number: %d\n", line_num);
            semantic_correctness_flag = 'e';
            return 'E';
        }
        else if(child1 != child2)
        {
            fprintf(ptr,"Type mismatch error in expression at line number: %d\n", line_num);
            semantic_correctness_flag = 'e';
            return 'E';
        }
        return 'B';
    }
    else if(string_cmp(node->label, "AND") || string_cmp(node->label, "OR"))
    {
        child1 = expression_type_check(node->child, ST, line_num);
        child2 = expression_type_check(node->child->rightsib, ST, line_num);
        if(child1 == 'E' || child2 == 'E')
        {
            return 'E';
        }
        else if(child1 == 'J' || child1 == 'S' || child1 == 'C' || child2 == 'J' || child2 == 'S' || child2 == 'C' || child1 == 'I' || child2 == 'I' || child1 == 'R' || child2 == 'R')
        {
            fprintf(ptr,"Type mismatch error in expression at line number: %d\n", line_num);
            semantic_correctness_flag = 'e';
            return 'E';
        }
        return 'B';
    }
}

void type_check(struct TreeNode * node, struct ST * ST)
{
    if(node == NULL)
    {
        return;
    }
    if(node->label == NULL)
    {

    }
    else if(string_cmp(node->label, "ASID"))
    {
        char lhs, rhs;
        int line_num = node->child->line_num;
        rhs = expression_type_check(node->child->rightsib, ST, line_num);
        // printf("RHS: %c\n", rhs);
        struct ST_entry * entry = extract_type(node->child->property.lexeme,ST);
        if(entry == NULL)
        {
            fprintf(ptr, "Variable %s not declared at line number: %d\n", node->child->property.lexeme, node->child->line_num);
            semantic_correctness_flag = 'e';
        }
        else
        {
            if(entry->identifier == 'A')
            {
                lhs = entry->dt.at.a_type+1;
            }
            else if(entry->identifier == 'P')
            {
                lhs = entry->dt.pt.p_type;
            }
            if(rhs == 'E')
            {
                return;
            }
            if(lhs!=rhs)
            {
                fprintf(ptr,"Type mismatch error at line number: %d\n", node->line_num);
                semantic_correctness_flag = 'e';
                return;
            }
            if(entry->identifier == 'A')
            {
                struct TreeNode * rhs_node = node->child->rightsib->child;
                struct ST_entry * entry2 = extract_type(rhs_node->property.lexeme,ST);
                if(entry->dt.at.is_dynamic != entry2->dt.at.is_dynamic)
                {
                    fprintf(ptr,"Array bound incompatible in Assignment Statement at line number: %d\n", node->line_num);
                    semantic_correctness_flag = 'e';
                    return;
                }
                if(entry->dt.at.is_dynamic == 1)
                {
                    // Insert code after doubt is resolved
                }
                else
                {
                    if(entry->dt.at.l != entry2->dt.at.l || entry->dt.at.r != entry2->dt.at.r)
                    {
                        fprintf(ptr,"Array bound incompatible in Assignment Statement at line number: %d\n", node->line_num);
                        semantic_correctness_flag = 'e';
                        return;
                    }
                }
            }
        }
    }
    else if(string_cmp(node->label, "ASARR"))
    {
        char lhs, rhs, index_expression;
        int line_num = node->child->line_num;
        index_expression = expression_type_check(node->child->rightsib, ST, line_num);
        rhs = expression_type_check(node->child->rightsib->rightsib, ST, line_num);
        struct ST_entry * entry = extract_type(node->child->property.lexeme,ST);
        if(entry == NULL)
        {
            fprintf(ptr,"Variable %s not declared at line number: %d\n", node->child->property.lexeme, node->child->line_num);
            semantic_correctness_flag = 'e';
        }
        else
        {
            if(entry->identifier == 'A')
            {
                lhs = entry->dt.at.a_type;
            }
            else if(entry->identifier == 'P')
            {
                fprintf(ptr,"LHS not an array at line number %d\n", node->child->line_num);
                semantic_correctness_flag = 'e';
                return;
            }
            if(index_expression != 'I')
            {
                fprintf(ptr, "Invalid type for index of Array at line number %d\n", node->child->line_num);
                semantic_correctness_flag = 'e';
                return;
            }
            else if(string_cmp(node->child->rightsib->key, "NUM") && entry->dt.at.is_dynamic == 0)
            {
                int val = node->child->rightsib->property.value_int;
                if(val < entry->dt.at.l || val > entry->dt.at.r)
                {
                    fprintf(ptr, "Array variable: %s[%d] index out of bound at line number: %d\n",node->child->property.lexeme, val, node->child->line_num);
                    semantic_correctness_flag = 'e';
                }
            }
            if(rhs == 'E')
            {
                return;
            }
            if(lhs!=rhs)
            {
                fprintf(ptr, "Type mismatch at line number: %d\n", node->line_num);
                semantic_correctness_flag = 'e';
                return;
            }
        }
    }
    // else
    // {
    //     struct TreeNode * child = node->child;
    //     while(child)
    //     {
    //         type_check(child, ST);
    //         child = child->rightsib;
    //     }
    // }
}

void switch_semantic_analysis(struct TreeNode *node, struct ST * ST)
{
    struct ST_entry * entry = extract_type(node->child->property.lexeme, ST);
    if(entry == NULL)
    {
        fprintf(ptr, "Variable %s not declared at line number: %d\n", node->child->property.lexeme, node->child->line_num);
        semantic_correctness_flag = 'e';
    }
    else
    {
        if(entry->identifier == 'A')
        {
            fprintf(ptr,"Array identifier: %s in switch at line number %d\n",node->child->property.lexeme, node->child->line_num);
            semantic_correctness_flag = 'e';
            return;
        }
        char type = entry->dt.pt.p_type;
        if(type == 'R')
        {
            fprintf(ptr, "Real identifier: %s in switch at line number %d\n",node->child->property.lexeme,  node->child->line_num);
            semantic_correctness_flag = 'e';
            return;
        }
        else if(type == 'I')
        {
            if(node->child->rightsib->rightsib->child == NULL)
            {
                fprintf(ptr,"No default specified for integer identifier: %s in switch at line number %d\n",node->child->property.lexeme, node->child->line_num);
                semantic_correctness_flag = 'e';
            }

            struct TreeNode * case_node = node->child->rightsib->child;
            while(case_node)
            {
                if(string_cmp(case_node->child->key, "TRUE") || string_cmp(case_node->child->key, "FALSE"))
                {
                    fprintf(ptr,"Case value is incorrect as condition variable type is integer at line number %d\n", case_node->child->line_num);
                    semantic_correctness_flag = 'e';
                }

                case_node = case_node->rightsib;
            }
            return;
        }
        else
        {
            if(node->child->rightsib->rightsib->child != NULL)
            {
                fprintf(ptr,"Default specified for boolean identifier: %s in switch at line number %d\n",node->child->property.lexeme, node->child->line_num);
                semantic_correctness_flag = 'e';
            }
            struct TreeNode * case_node = node->child->rightsib->child;
            while(case_node)
            {
                if(string_cmp(case_node->child->key, "NUM") || string_cmp(case_node->child->key, "MINUS"))
                {
                    fprintf(ptr,"Case value is incorrect as condition variable type is boolean at line number %d\n", case_node->child->line_num);
                    semantic_correctness_flag = 'e';
                }

                case_node = case_node->rightsib;
            }
            return;
        }
    }
}

void list_type_check(struct TreeNode * node, struct list_node * list_node, struct ST * ST, int line_num)
{
    while(node != NULL && list_node != NULL)
    {
        struct ST_entry * entry = extract_type(node->property.lexeme, ST);
        if(entry == NULL)
        {
            fprintf(ptr,"Variable %s not declared at line number: %d\n", node->property.lexeme, line_num);
            semantic_correctness_flag = 'e';
        }
        else
        {
            if(entry->identifier != list_node->identifier)
            {
                // printf("%s %c %c - Debug print\n", node->property.lexeme, entry->identifier, list_node->identifier);
                fprintf(ptr, "Type incompatible of %s in Module Reuse Statement at line number: %d\n", node->property.lexeme, line_num);
                semantic_correctness_flag = 'e';
            }
            else
            {
                if(entry->identifier == 'A')
                {
                    int flag = 0;
                    if(entry->dt.at.a_type != list_node->dt.at.a_type && !flag)
                    {
                        fprintf(ptr,"Type incompatible of %s in Module Reuse Statement at line number: %d\n",node->property.lexeme, line_num);
                        semantic_correctness_flag = 'e';
                        flag = 1;
                    }
                    if(entry->dt.at.is_dynamic != list_node->dt.at.is_dynamic && !flag)
                    {
                        fprintf(ptr,"Array bound incompatible of %s in Module Reuse Statement at line number: %d\n", node->property.lexeme, line_num);
                        semantic_correctness_flag = 'e';
                        flag = 1;
                    }
                    if(entry->dt.at.is_dynamic == 1)
                    {
                        // Insert code after doubt is resolved
                    }
                    else
                    {
                        if((entry->dt.at.l != list_node->dt.at.l || entry->dt.at.r != list_node->dt.at.r) && !flag)
                        {
                            fprintf(ptr,"Array bound incompatible of %s in Module Reuse Statement at line number: %d\n", node->property.lexeme, line_num);
                            semantic_correctness_flag = 'e';
                            flag = 1;
                        }
                    }
                }
                else
                {
                    if(entry->dt.pt.p_type != list_node->dt.pt.p_type)
                    {
                        fprintf(ptr, "Type incompatible of %s in Module Reuse Statement at line number: %d\n", node->property.lexeme, line_num);
                        semantic_correctness_flag = 'e';
                    }
                }
            }
        }
        node = node->rightsib;
        list_node = list_node->next;
    }

    if(list_node || node)
    {
        fprintf(ptr, "Number of parameters not matching in module Reuse Statement at line number: %d\n", line_num);
        semantic_correctness_flag = 'e';
    }
}

// Implementation remaining
// void while_semantic_analysis(struct TreeNode * node, struct TreeNode * stmt)
// {
//     // struct TreeNode * temp = node->child;
    
// }

void for_semantic_analysis(struct TreeNode * node, struct TreeNode * stmt) // node=FOR, stmt = 
{
    struct TreeNode * iterator_node = node->child;
    while(stmt != NULL)
    {
        if(string_cmp(stmt->label, "ASID"))
        {
            if(string_cmp(iterator_node->property.lexeme, stmt->child->property.lexeme))
            {
                fprintf(ptr, "For loop iterator variable: %s cannot be assigned a value at line number: %d\n",iterator_node->property.lexeme, stmt->child->line_num);
                semantic_correctness_flag = 'e';
            }
        }
        else if(string_cmp(stmt->label, "FOR"))
        {
            // printf("IN for_semantic_analysis: FOR\n");
            for_semantic_analysis(node, stmt->child->rightsib->rightsib->child);
        }
        else if(string_cmp(stmt->label, "WHILE"))
        {
            // printf("IN for_semantic_analysis: WHILE\n");
            for_semantic_analysis(node, stmt->child->rightsib->child);
        }
        else if(string_cmp(stmt->label, "SWITCH"))
        {
            struct TreeNode * case_node = stmt->child->rightsib->child;
            struct TreeNode * default_node = stmt->child->rightsib->rightsib->child;
            // printf("IN for_semantic_analysis: SWITCH\n");
            while(case_node != NULL)
            {
                for_semantic_analysis(node, case_node->child->rightsib->child);
                case_node = case_node->rightsib;
            }
            if(default_node)
            {
                default_node = default_node->child;
                for_semantic_analysis(node, default_node);
            }
            else
            {
                return;
            }
        }
        else if(string_cmp(stmt->label,"MRS"))
        {
            struct TreeNode* idNode = stmt->child->child;
            while(idNode != NULL)
            {
                if(string_cmp(idNode->property.lexeme,iterator_node->property.lexeme))
                {
                    fprintf(ptr,"For loop iterator variable: %s cannot be assigned a value at line number: %d\n",iterator_node->property.lexeme, idNode->line_num);
                    semantic_correctness_flag = 'e';
                    break;
                }
                idNode = idNode->rightsib;
            }
        }
        stmt = stmt->rightsib;
    }
}

void function_overloading_analysis(struct TreeNode * node)
{
    // Driver redeclaration check
    // Multiple Declarations 
    // Declaration present but definition absent
    // Multiple Definition
    
    // Checking other 1
    struct TreeNode * module_node = node->child->rightsib->child;
    while(module_node != NULL)
    {
        struct TreeNode * dummy_module_node = module_node->rightsib;
        while(dummy_module_node != NULL)
        {
            if(string_cmp(module_node->child->property.lexeme, dummy_module_node->child->property.lexeme))
            {
                fprintf(ptr,"Function: %s definition redeclared at line number: %d\n",dummy_module_node->child->property.lexeme, dummy_module_node->child->line_num);
                semantic_correctness_flag = 'e';
            }
            dummy_module_node = dummy_module_node->rightsib;
        }
        module_node = module_node->rightsib;
    }
    // Checking other 2
    module_node = node->child->rightsib->rightsib->rightsib->child;
    while(module_node != NULL)
    {
        struct TreeNode * dummy_module_node = module_node->rightsib;
        while(dummy_module_node != NULL)
        {
            if(string_cmp(module_node->child->property.lexeme, dummy_module_node->child->property.lexeme))
            {
                fprintf(ptr, "Function: %s definition redeclared at line number: %d\n",dummy_module_node->child->property.lexeme, dummy_module_node->child->line_num);
                semantic_correctness_flag = 'e';
            }
            dummy_module_node = dummy_module_node->rightsib;
        }
        module_node = module_node->rightsib;
    }

    // One from other1 and one from other2
    module_node = node->child->rightsib->child;
    while(module_node != NULL)
    {
        struct TreeNode * dummy_module_node = node->child->rightsib->rightsib->rightsib->child;
        while(dummy_module_node != NULL)
        {
            if(string_cmp(module_node->child->property.lexeme, dummy_module_node->child->property.lexeme))
            {
                fprintf(ptr, "Function: %s definition redeclared at line number: %d\n",dummy_module_node->child->property.lexeme, dummy_module_node->child->line_num);
                semantic_correctness_flag = 'e';
            }
            dummy_module_node = dummy_module_node->rightsib;
        }
        module_node = module_node->rightsib;
    }
}

void traverse_AST(struct TreeNode * idNode, struct TreeNode * stmt)
{
    // printf("IN traverse_AST\n");
    while(stmt != NULL)
    {
        if(string_cmp(stmt->label, "MRS"))
        {
            // printf("IN traverse_AST: MRS\n");
            if(string_cmp(idNode->property.lexeme, stmt->child->rightsib->property.lexeme))
            {
                fprintf(ptr, "%s Function cannot call itself at line number: %d\n",stmt->child->rightsib->property.lexeme, stmt->child->rightsib->line_num);
                semantic_correctness_flag = 'e';
            }
        }
        else if(string_cmp(stmt->label, "FOR"))
        {
            // printf("IN traverse_AST: FOR\n");
            traverse_AST(idNode, stmt->child->rightsib->rightsib->child);
        }
        else if(string_cmp(stmt->label, "WHILE"))
        {
            // printf("IN traverse_AST: WHILE\n");
            traverse_AST(idNode, stmt->child->rightsib->child);
        }
        else if(string_cmp(stmt->label, "SWITCH"))
        {
            struct TreeNode * case_node = stmt->child->rightsib->child;
            struct TreeNode * default_node = stmt->child->rightsib->rightsib->child;
            // printf("IN traverse_AST: SWITCH\n");
            while(case_node != NULL)
            {
                traverse_AST(idNode, case_node->child->rightsib->child);
                case_node = case_node->rightsib;
            }
            if(default_node)
            {
                default_node = default_node->child;
                traverse_AST(idNode, default_node);
            }
            else
            {
                return;
            }
        }
        stmt = stmt->rightsib;
    }
}

void check_recursion(struct TreeNode * node)
{
    struct TreeNode * module_node = node->child->rightsib->child;
    //Checking in Other 1
    while(module_node != NULL)
    {
        struct TreeNode * idNode = module_node->child;
        // printf("IN check_recursion: %s\n", idNode->property.lexeme);
        traverse_AST(idNode, module_node->child->rightsib->rightsib->rightsib->child);
        module_node = module_node->rightsib;
    }
    module_node = node->child->rightsib->rightsib->rightsib->child;
    //Checking in Other 2
    while(module_node != NULL)
    {
        struct TreeNode * idNode = module_node->child;
        traverse_AST(idNode, module_node->child->rightsib->rightsib->rightsib->child);
        module_node = module_node->rightsib;
    }
}

int function_parameter_redeclaration(char * id, struct ST * symbol_table, int line)
{
    struct ST * temp = symbol_table;
    while(temp->identifier == 'S')
    {
        temp = temp->par.ST;
    }
    struct FT_entry * temp2 = temp->par.FT_entry;
    struct list_node * node = temp2->head_op;
    while(node != NULL)
    {
        if(string_cmp(id, node->key))
        {
            fprintf(ptr, "Output Parameter: %s is redeclared at line num: %d\n", node->key, line);
            semantic_correctness_flag = 'e';
            return 1;
        }
        node = node->next;
    }
    return 0;
}

// Width and offset not initialized
// Free symbol table after scope ends
// Add semantic checks in this function
void make_symbol_table(struct TreeNode* node, struct ST* symbol_table, int nested_level, struct FT*function_table)
{
    if (node == NULL)
        return;
    
    if(node->label == NULL)
    {

    }
    else if(string_cmp(node->label, "DEC"))
    {
        struct TreeNode* type = node->child->rightsib;
        struct TreeNode* id = node->child->child;
        // printf("IN DEC:\n");
        while(id != NULL)
        {
            int entry_no;
            // printf("%s\n", id->property.lexeme);
            if(function_parameter_redeclaration(id->property.lexeme, symbol_table,id->line_num))
            {

            }
            else if(string_cmp(type->label, "ARRAY"))
            {
                entry_no = contains_st(id->property.lexeme, symbol_table);
                if(entry_no != -1)
                {
                    // Redeclaration
                    fprintf(ptr,"Redeclaration of variable : %s at line number: %d\n", id->property.lexeme, id->line_num);
                    semantic_correctness_flag = 'e';
                }
                else
                {
                    entry_no = insert_st(id->property.lexeme, symbol_table);
                    if(entry_no == -1)
                    {
                        // ID not inserted in symbol table
                    }
                    else
                    {
                        struct TreeNode * L = node->child->rightsib->child->child;
                        struct TreeNode * R = node->child->rightsib->child->rightsib->child;
                        struct TreeNode * Dtype = node->child->rightsib->child->rightsib->rightsib;
                        (symbol_table->entry[entry_no]).dt.at.a_type = (Dtype->key)[0];
                        (symbol_table->entry[entry_no]).dt.at.is_dynamic = 0;
                        (symbol_table->entry[entry_no]).dt.at.left = NULL;
                        (symbol_table->entry[entry_no]).dt.at.right = NULL;
                        if(string_cmp(L->key, "ID"))
                        {
                            (symbol_table->entry[entry_no]).dt.at.is_dynamic = 1;
                            (symbol_table->entry[entry_no]).dt.at.left = str_copy(L->property.lexeme);
                        }
                        else if(string_cmp(L->key, "NUM"))
                        {
                            (symbol_table->entry[entry_no]).dt.at.l = L->property.value_int;
                            // printf("L value in Declaration : %s\n", L->key);
                        }
                        else if(string_cmp(L->key, "PLUS"))
                        {
                            if(string_cmp(L->rightsib->key, "ID"))
                            {
                                (symbol_table->entry[entry_no]).dt.at.is_dynamic = 1;
                                char * ch = concat("+", L->property.lexeme);
                                (symbol_table->entry[entry_no]).dt.at.left = str_copy(ch);
                            }
                            else
                            {
                                (symbol_table->entry[entry_no]).dt.at.l = L->rightsib->property.value_int;
                            }
                        }
                        else if(string_cmp(L->key, "MINUS"))
                        {
                            if(string_cmp(L->rightsib->key, "ID"))
                            {
                                (symbol_table->entry[entry_no]).dt.at.is_dynamic = 1;
                                char * ch = concat("-", L->property.lexeme);
                                (symbol_table->entry[entry_no]).dt.at.left = str_copy(ch);
                            }
                            else
                            {
                                (symbol_table->entry[entry_no]).dt.at.l = (L->rightsib->property.value_int)*(-1);
                            }
                        }

                        if(string_cmp(R->key, "ID"))
                        {
                            (symbol_table->entry[entry_no]).dt.at.is_dynamic = 1;
                            (symbol_table->entry[entry_no]).dt.at.right = str_copy(R->property.lexeme);
                        }
                        else if(string_cmp(R->key, "NUM"))
                        {
                            (symbol_table->entry[entry_no]).dt.at.r = R->property.value_int;
                            // printf("R value in Declaration : %s\n", R->key);
                        }
                        else if(string_cmp(R->key, "PLUS"))
                        {
                            if(string_cmp(R->rightsib->key, "ID"))
                            {
                                (symbol_table->entry[entry_no]).dt.at.is_dynamic = 1;
                                char * ch = concat("+", R->property.lexeme);
                                (symbol_table->entry[entry_no]).dt.at.right = str_copy(ch);
                            }
                            else
                            {
                                (symbol_table->entry[entry_no]).dt.at.r = R->rightsib->property.value_int;
                            }
                        }
                        else if(string_cmp(R->key, "MINUS"))
                        {
                            if(string_cmp(R->rightsib->key, "ID"))
                            {
                                (symbol_table->entry[entry_no]).dt.at.is_dynamic = 1;
                                char * ch = concat("-", R->property.lexeme);
                                (symbol_table->entry[entry_no]).dt.at.right = str_copy(ch);
                            }
                            else
                            {
                                (symbol_table->entry[entry_no]).dt.at.r = (R->rightsib->property.value_int)*(-1);
                            }
                        }
                        (symbol_table->entry[entry_no]).deleted = 0;
                        (symbol_table->entry[entry_no]).identifier = 'A';
                        
                        /* Jash added code */

                        if(symbol_table->entry[entry_no].dt.at.is_dynamic==1)
                        {
                            (symbol_table->entry[entry_no]).width = 1;
                            int module_no = module_entry_no(symbol_table,function_table);
                            (symbol_table->entry[entry_no]).offset = function_table->entry[module_no].offset;
                            function_table->entry[module_no].offset += 8;
                        }
                        else
                        {
                            int w = 0;
                            char ch = symbol_table->entry[entry_no].dt.at.a_type;
                            if(ch=='I')
                            {
                                w = 2;
                            }
                            else if(ch=='B')
                            {
                                w=1;
                            }
                            else
                            {
                                w = 4;
                            }
                            int size = ((symbol_table->entry[entry_no]).dt.at.r - (symbol_table->entry[entry_no]).dt.at.l)+1;
                            (symbol_table->entry[entry_no]).width = (size*w)+1;
                            int module_no = module_entry_no(symbol_table,function_table);
                            (symbol_table->entry[entry_no]).offset = function_table->entry[module_no].offset;
                            function_table->entry[module_no].offset += (8*(size+1));
                        }
                    }

                }

            }
            else if(string_cmp(type->key, "INTEGER"))
            {
                entry_no = contains_st(id->property.lexeme, symbol_table);
                if(entry_no != -1)
                {
                    // Redeclaration
                    fprintf(ptr,"Redeclaration of variable : %s at line number: %d\n", id->property.lexeme, id->line_num);
                    semantic_correctness_flag = 'e';
                }
                else
                {
                    entry_no = insert_st(id->property.lexeme, symbol_table);
                    if(entry_no == -1)
                    {
                        // ID not inserted in symbol table
                    }
                    else
                    {
                        (symbol_table->entry[entry_no]).dt.pt.p_type = 'I';
                        (symbol_table->entry[entry_no]).width = 2;
                        
                        int module_no = module_entry_no(symbol_table,function_table);
                        (symbol_table->entry[entry_no]).offset = function_table->entry[module_no].offset;
                        function_table->entry[module_no].offset += 8;

                        (symbol_table->entry[entry_no]).deleted = 0;
                        // if(nested_level == 1)
                        // {
                        //     (symbol_table->entry[entry_no]).par.FT = function_table;
                        // }
                        // else
                        // {
                        //     (symbol_table->entry[entry_no]).par.ST = symbol_table;
                        // }
                        (symbol_table->entry[entry_no]).identifier = 'P';
                    }
                }
            }
            else if(string_cmp(type->key, "REAL"))
            {
                entry_no = contains_st(id->property.lexeme, symbol_table);
                if(entry_no != -1)
                {
                    // Redeclaration
                    fprintf(ptr,"Redeclaration of variable : %s at line number: %d\n", id->property.lexeme, id->line_num);
                    semantic_correctness_flag = 'e';
                }
                else
                {
                    entry_no = insert_st(id->property.lexeme, symbol_table);
                    if(entry_no == -1)
                    {
                        // ID not inserted in symbol table
                    }
                    else
                    {
                        (symbol_table->entry[entry_no]).dt.pt.p_type = 'R';
                        (symbol_table->entry[entry_no]).width = 4;

                        int module_no = module_entry_no(symbol_table,function_table);
                        (symbol_table->entry[entry_no]).offset = function_table->entry[module_no].offset;
                        function_table->entry[module_no].offset += 8;

                        (symbol_table->entry[entry_no]).deleted = 0;
                        // if(nested_level == 1)
                        // {
                        //     (symbol_table->entry[entry_no]).par.FT = function_table;
                        // }
                        // else
                        // {
                        //     (symbol_table->entry[entry_no]).par.ST = symbol_table;
                        // }
                        
                        (symbol_table->entry[entry_no]).identifier = 'P';
                    }
                }
            }
            else if(string_cmp(type->key, "BOOLEAN"))
            {
                entry_no = contains_st(id->property.lexeme, symbol_table);
                if(entry_no != -1)
                {
                    // Redeclaration
                    fprintf(ptr,"Redeclaration of variable : %s at line number: %d\n", id->property.lexeme, id->line_num);
                    semantic_correctness_flag = 'e';
                }
                else
                {
                    entry_no = insert_st(id->property.lexeme, symbol_table);
                    if(entry_no == -1)
                    {
                        // ID not inserted in symbol table
                    }
                    else
                    {
                        (symbol_table->entry[entry_no]).dt.pt.p_type = 'B';
                        (symbol_table->entry[entry_no]).width = 1;

                        int module_no = module_entry_no(symbol_table,function_table);
                        (symbol_table->entry[entry_no]).offset = function_table->entry[module_no].offset;
                        function_table->entry[module_no].offset += 8;

                        (symbol_table->entry[entry_no]).deleted = 0;
                        // if(nested_level == 1)
                        // {
                        //     (symbol_table->entry[entry_no]).par.FT = function_table;
                        // }
                        // else
                        // {
                        //     (symbol_table->entry[entry_no]).par.ST = symbol_table;
                        // }
                        
                        (symbol_table->entry[entry_no]).identifier = 'P';
                    }
                }
            }
            id = id->rightsib;
        }
        node->ST = symbol_table;
    }
    else if(string_cmp(node->label, "MOD_DECS"))
    {
        // printf("Reached MOD_DECS\n");
        struct TreeNode * module_dec = node->child;
        // printf("Module Node: %s\n", module_dec->property.lexeme);
        while(module_dec)
        {
            struct TreeNode * module_node = search_module_definition(module_dec->property.lexeme, node->parent);
            if(module_node)
            {
                int entry_no = insert_ft(module_dec->property.lexeme, function_table);
                if(entry_no == -1)
                {
                    // Module ID not inserted in function table
                }
                else
                {
                    (function_table->entry[entry_no]).deleted = 0;
                    (function_table->entry[entry_no]).head_ip = make_list(module_node->child->rightsib->child);
                    (function_table->entry[entry_no]).head_op = make_list(module_node->child->rightsib->rightsib->child);
                    (function_table->entry[entry_no]).flag = 1;
                    struct ST * cur_symbol_table = symbol_table_constructor();
                    cur_symbol_table->scope_start = module_node->scope_start;
                    cur_symbol_table->scope_end = module_node->scope_end;
                    function_table->entry[entry_no].scope_start = cur_symbol_table->scope_start;
                    function_table->entry[entry_no].scope_end = cur_symbol_table->scope_end;
                    cur_symbol_table->identifier = 'F';
                    cur_symbol_table->par.FT_entry = &(function_table->entry[entry_no]);
                    (function_table->entry[entry_no]).child = cur_symbol_table;
                    make_symbol_table(module_node->child->rightsib->rightsib->rightsib, cur_symbol_table, nested_level+1, function_table);
                }
            }
            module_dec = module_dec->rightsib;
        }
        // printf("IN MOD_DECS\n");
    }
    else if(string_cmp(node->label, "OTHER_1") || string_cmp(node->label, "OTHER_2"))
    {
        struct TreeNode * module = node->child;
        while(module)
        {
            // printf("Reached here\n");
            int entry_no = contains_ft(module->child->property.lexeme, function_table);
            if(entry_no != -1)
            {
                int flag = function_table->entry[entry_no].flag;
                if(flag == 1)
                {
                    function_table->entry[entry_no].flag = 3;
                }
                // else if(flag == 2)
                // {
                //     printf("Function: %s definition redeclared at line number: %d\n", module->child->property.lexeme, module->child->line_num);
                // }
                // else
                // {
                //     printf("Function: %s definition redeclared at line number: %d\n", module->child->property.lexeme, module->child->line_num);
                // }
            }
            else
            {
                // printf("Working here\n");
                entry_no = insert_ft(module->child->property.lexeme, function_table);
                if(entry_no == -1)
                {
                    // Module ID not inserted in function table
                    printf("Module ID not inserted in function table\n");
                }
                else
                {
                    (function_table->entry[entry_no]).deleted = 0;
                    (function_table->entry[entry_no]).head_ip = make_list(module->child->rightsib->child);
                    (function_table->entry[entry_no]).head_op = make_list(module->child->rightsib->rightsib->child);
                    struct ST * cur_symbol_table = symbol_table_constructor();
                    cur_symbol_table->scope_start = module->scope_start;
                    cur_symbol_table->scope_end = module->scope_end;
                    function_table->entry[entry_no].scope_start = cur_symbol_table->scope_start;
                    function_table->entry[entry_no].scope_end = cur_symbol_table->scope_end;
                    function_table->entry[entry_no].flag = 2;
                    cur_symbol_table->identifier = 'F';
                    cur_symbol_table->par.FT_entry = &(function_table->entry[entry_no]);
                    (function_table->entry[entry_no]).child = cur_symbol_table;
                    make_symbol_table(module->child->rightsib->rightsib->rightsib, cur_symbol_table, nested_level+1, function_table);
                }
            }
            module = module->rightsib;
        }

        /*  Jash code starts  */

        struct TreeNode* module_node = node->child; // checking in other1
        while(module_node != NULL)
        {
            int entry_no = contains_ft(module_node->child->property.lexeme,function_table);
            // printf("JASH ENTRY_NO: %d\n", entry_no);
            struct list_node* list_node = function_table->entry[entry_no].head_op;
            int line_num = function_table->entry[entry_no].scope_end;

            int size = 0;
            // printf("JASH HI -1 !!!!!!!!!!!!!!!!!!\n");
            while(list_node != NULL)
            {
                size++;
                // printf("!!!!!!!!!!! JASH: %s", list_node->key);
                list_node = list_node->next;
            }
            // printf("JASH HI 0 !!!!!!!!!!!!!!!!!!\n");
            char* visited = (char*)malloc(size * sizeof(char));
            for(int i=0;i<size;i++)
            {
                visited[i] = 'n'; // m: modified  ,  n: not modified
            }

            list_node = function_table->entry[entry_no].head_op;
            // printf("JASH HI 1 !!!!!!!!!!!!!!!!!!\n");
            traverse_AST_2(list_node , module_node->child->rightsib->rightsib->rightsib->child, visited);    
            // printf("JASH HI 2 !!!!!!!!!!!!!!!!!!\n");
            int check_flag = 0;
            for(int i=0;i<size;i++)
            {
                if(visited[i]=='n')
                {
                    check_flag += 1;
                }
            }

            if(check_flag >= 1)
            {
                fprintf(ptr,"Output parameters: [");
                int i = 0; struct list_node* temp = list_node; 
                while(temp != NULL)
                {
                    if(visited[i] == 'n')
                    {
                        if(check_flag==1)
                        {
                            fprintf(ptr," %s ",temp->key);
                        }
                        else
                        {
                            fprintf(ptr," %s,",temp->key);
                        }
                        check_flag--;
                    }
                    temp = temp->next;
                    i++;
                }
                fprintf(ptr,"] is not assigned any value inside function %s at line number: %d\n", module_node->child->property.lexeme,line_num);
                semantic_correctness_flag = 'e';
            }

            module_node = module_node -> rightsib;
        }

        /* Jash code ends  */
    }
    else if(string_cmp(node->label, "DRIVER"))
    {
        // printf("IN DRIVER\n");
        int entry_no = insert_ft(node->label, function_table);
        if(entry_no == -1)
        {
            // Module ID not inserted in function table
        }
        else
        {
            (function_table->entry[entry_no]).deleted = 0;
            (function_table->entry[entry_no]).head_ip = NULL;
            (function_table->entry[entry_no]).head_op = NULL;
            struct ST * cur_symbol_table = symbol_table_constructor();
            cur_symbol_table->scope_start = node->scope_start;
            cur_symbol_table->scope_end = node->scope_end;
            function_table->entry[entry_no].scope_start = cur_symbol_table->scope_start;
            function_table->entry[entry_no].scope_end = cur_symbol_table->scope_end;
            cur_symbol_table->identifier = 'F';
            cur_symbol_table->par.FT_entry = &(function_table->entry[entry_no]);
            (function_table->entry[entry_no]).child = cur_symbol_table;
            // printf("In Driver for symbol table\n");
            make_symbol_table(node->child, cur_symbol_table, nested_level+1, function_table);
        }
    }
    else if(string_cmp(node->label, "FOR"))
    {
        
        int entry_no = insert_st("FOR", symbol_table);
        if(entry_no == -1)
        {

        }
        else
        {
            struct ST* cur_symbol_table = symbol_table_constructor();
            cur_symbol_table->scope_start = node->scope_start;
            cur_symbol_table->scope_end = node->scope_end;
            cur_symbol_table->identifier = 'S';
            cur_symbol_table->par.ST = symbol_table;
            int nested_ST_entry_no = insert_st(node->child->property.lexeme, cur_symbol_table);
            cur_symbol_table->entry[nested_ST_entry_no].deleted = 0;
            cur_symbol_table->entry[nested_ST_entry_no].dt.pt.p_type = 'I';
            cur_symbol_table->entry[nested_ST_entry_no].identifier = 'P';
            symbol_table->entry[entry_no].deleted = 0;
            symbol_table->entry[entry_no].identifier = 'S';
            // printf("Entry no. : %d, Nested entry no. : %d\n", entry_no, nested_ST_entry_no);
            insert_ST_at_end(&(symbol_table->entry[entry_no]), cur_symbol_table);
            // if(symbol_table->entry[entry_no].dt.child == NULL)
            // {
            //     printf("NULL symbol table!!!\n");
            // }
            // print_symbol_table(function_table);
            // printf("In FOR in symbol table\n");
            for_semantic_analysis(node,node->child->rightsib->rightsib->child);
            make_symbol_table(node->child->rightsib->rightsib, cur_symbol_table, nested_level+1, function_table);
            node->ST = cur_symbol_table;
            // Add semantic checks in this function
            // free_symbol_table(symbol_table->entry[entry_no]);
        }
    }
    else if(string_cmp(node->label, "WHILE"))
    {

        int entry_no = insert_st("WHILE", symbol_table);
        if(entry_no==-1)
        {

        }
        else
        {
            struct ST* cur_symbol_table = symbol_table_constructor();
            cur_symbol_table->scope_start = node->scope_start;
            cur_symbol_table->scope_end = node->scope_end;
            cur_symbol_table->identifier = 'S';
            cur_symbol_table->par.ST = symbol_table;
            symbol_table->entry[entry_no].deleted = 0;
            insert_ST_at_end(&(symbol_table->entry[entry_no]), cur_symbol_table);
            symbol_table->entry[entry_no].identifier = 'S';
            char type = expression_type_check(node->child, symbol_table, node->line_num);
            if(type != 'B' && type != 'E')
            {
                fprintf(ptr,"While expression not of boolean type at line number: %d\n", node->line_num);
                semantic_correctness_flag = 'e';
            }
            make_symbol_table(node->child->rightsib, cur_symbol_table, nested_level+1,function_table);
            while_semantic_check(node , cur_symbol_table->scope_end);
            node->ST = cur_symbol_table;
            // Add semantic checks in this function
            // free_symbol_table(symbol_table->entry[entry_no]);
        }

        
    }
    else if(string_cmp(node->label, "SWITCH"))
    {
        node->ST = symbol_table;
        switch_semantic_analysis(node, symbol_table);
        struct TreeNode * case_node = node->child->rightsib->child;
        while(case_node)
        {
            int entry_no = insert_st("CASE", symbol_table);
            if(entry_no == -1)
            {

            }
            else
            {
                // printf("Inside switch statement\n");
                struct ST* cur_symbol_table = symbol_table_constructor();
                cur_symbol_table->scope_start = case_node->scope_start;
                cur_symbol_table->scope_end = case_node->scope_end;
                cur_symbol_table->identifier = 'S';
                cur_symbol_table->par.ST = symbol_table;
                symbol_table->entry[entry_no].deleted = 0;
                symbol_table->entry[entry_no].identifier = 'S';
                insert_ST_at_end(&(symbol_table->entry[entry_no]), cur_symbol_table);
                make_symbol_table(case_node->child->rightsib, cur_symbol_table, nested_level+1, function_table);
                // Add semantic checks in this function
                // free_symbol_table(symbol_table->entry[entry_no]);
            }
            case_node = case_node->rightsib;
        }
        struct TreeNode * default1 = node->child->rightsib->rightsib; 
        if(default1->child != NULL)
        {
            int entry_no = insert_st("DEFAULT", symbol_table);
            if(entry_no == -1)
            {

            }
            else
            {
                // printf("Inside Default\n");
                struct ST* cur_symbol_table = symbol_table_constructor();
                cur_symbol_table->scope_start = default1->scope_start;
                cur_symbol_table->scope_end = default1->scope_end;
                cur_symbol_table->identifier = 'S';
                cur_symbol_table->par.ST = symbol_table;
                symbol_table->entry[entry_no].deleted = 0;
                symbol_table->entry[entry_no].identifier = 'S';
                insert_ST_at_end(&(symbol_table->entry[entry_no]), cur_symbol_table);
                make_symbol_table(default1->child, cur_symbol_table, nested_level+1, function_table);
                // Add semantic checks in this function
                // free_symbol_table(symbol_table->entry[entry_no]);
            }
        }
    }
    else if(string_cmp(node->label, "PROGRAM"))
    {
        function_overloading_analysis(node);
        check_recursion(node);
        // printf("IN PROGRAM\n");
        // function_parameters_change(node,function_table);
        struct TreeNode * temp = node->child;
        while(temp)
        {
            make_symbol_table(temp, symbol_table, nested_level, function_table);
            temp = temp->rightsib;
        }
    }
    else if(string_cmp(node->label, "MRS"))
    {
        // printf("IN MRS\n");
        node->ST = symbol_table;
        int entry_no = contains_ft(node->child->rightsib->property.lexeme, function_table);
        if(entry_no == -1)
        {
            fprintf(ptr,"Invoked module: %s does not exist at line number: %d\n",node->child->rightsib->property.lexeme,  node->child->rightsib->line_num);
            semantic_correctness_flag = 'e';
            return;
        }
        else if(function_table->entry[entry_no].flag == 3)
        {
            fprintf(ptr, "Module: %s definition and its declaration both appear before its call at line number: %d\n", node->child->rightsib->property.lexeme, node->child->rightsib->line_num);
            semantic_correctness_flag = 'e';
        }
        else
        {
            struct TreeNode * lhs_node = node->child->child;
            struct TreeNode * rhs_node = node->child->rightsib->rightsib->child;
            function_table->entry[entry_no].flag = 2;
            list_type_check(lhs_node, function_table->entry[entry_no].head_op, symbol_table,node->child->rightsib->line_num);
            list_type_check(rhs_node, function_table->entry[entry_no].head_ip, symbol_table, node->child->rightsib->line_num);
            return;
        }
    }
    else if(string_cmp(node->label, "ASARR") || string_cmp(node->label, "ASID"))
    {
        node->ST = symbol_table;
        type_check(node, symbol_table);
        // printf("IN ASID or ASARR\n");
        return;
    }
    else if(string_cmp(node->label, "PRINT"))
    {
        node->ST = symbol_table;
        if(string_cmp(node->child->key, "ID"))
        {
            struct ST_entry *entry = extract_type(node->child->property.lexeme, symbol_table);
            if(entry == NULL)
            {
                fprintf(ptr, "Variable %s not declared at line number: %d\n", node->child->property.lexeme, node->child->line_num);
                semantic_correctness_flag = 'e';
            }
            else
            {
                if(node->child->rightsib != NULL)
                {
                    if(entry->identifier != 'A')
                    {
                        fprintf(ptr, "Variable: %s is not an array at line number: %d\n", node->child->property.lexeme, node->child->line_num);
                        semantic_correctness_flag = 'e';
                    }
                    else
                    {
                        if(string_cmp(node->child->rightsib->child->key, "NUM") && entry->dt.at.is_dynamic == 0)
                        {
                            if(entry->dt.at.l > node->child->rightsib->child->property.value_int || entry->dt.at.r < node->child->rightsib->child->property.value_int)
                            {
                                fprintf(ptr, "Array variable: %s[%d] index out of bound at line number: %d\n", node->child->property.lexeme,node->child->rightsib->child->property.value_int, node->child->line_num);
                                semantic_correctness_flag = 'e';
                            }
                        }
                    }
                }
            }
        }
    }
    else if(string_cmp(node->label, "GET_VALUE"))
    {
        node->ST = symbol_table;
        // printf("IN GET_VALUE\n");
        // printf("%s\n", node->child->property.lexeme);
        struct ST_entry * entry = extract_type(node->child->property.lexeme, symbol_table);
        if(entry == NULL)
        {
            fprintf(ptr, "Variable %s not declared at line number: %d\n", node->child->property.lexeme, node->child->line_num);
            semantic_correctness_flag = 'e';
        }

    }
    else
    {
        struct TreeNode * child = node->child;
        while(child)
        {
            make_symbol_table(child, symbol_table,nested_level,function_table);
            child = child->rightsib;
        }
    }
    
    // struct TreeNode* temp = node->child;
    // while (temp) {
    //     make_symbol_table(temp);
    //     temp = temp->rightsib;
    // }
}