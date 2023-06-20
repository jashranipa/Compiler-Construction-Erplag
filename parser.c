/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/

#include<stdio.h>
#include<stdlib.h>
#include"parser.h"

struct Tree* my_tree;
int id_value, num_value, rnum_value;

int is_Present_sync_set(int non_terminal_num,int terminal_num)
{
    char** sync_set = (char **)malloc(11*sizeof(char *));
    sync_set[0] = "<<";
    sync_set[1] = "<<<";
    sync_set[2] = ">>";
    sync_set[3] = ">>>";
    sync_set[4] = "START";
    sync_set[5] = "END";
    sync_set[6] = "FOR";
    sync_set[7] = ";";
    sync_set[8] = "SWITCH";
    sync_set[9] = "WHILE";
    sync_set[10] = "CASE";

    if(is_present_Follow(non_terminal_num,terminal_num)==1)
    {
        return 1;
    }

    for(int i=0;i<11;i++)
    {
        if(terminal_num == get_value(sync_set[i],'T'))
        {
            return 1;
        }
    }
    return 0;
}


void write_lexeme(FILE* fp, tk curr_token)
{
    int type = curr_token->type;
    if (type == id_value) {
        fprintf(fp, "%d          %s                  ID\n", curr_token->line_num, curr_token->property.lexeme);
    }
    else if (type == num_value) {
        fprintf(fp, "%d          %d                  NUM\n", curr_token->line_num, curr_token->property.value_int);
    }
    else if (type == rnum_value) {
        fprintf(fp, "%d          %f                  RNUM\n", curr_token->line_num, curr_token->property.value_double);
    }
    else {
        fprintf(fp, "%d          %s                  %s\n", curr_token->line_num,curr_token->property.lexeme, Tglobal->entry[type].key);
    }
}

struct TreeNode* make_parsetree()
{
    FILE* lexemes = fopen("lexemes.txt", "w");
    id_value = get_value("ID", 'T');
    num_value = get_value("NUM", 'T');
    rnum_value = get_value("RNUM", 'T');

    FILE* syntax_errors = fopen("syntax_errors.txt", "w");

    // flag - 'c' means correct, 'e' means erroneous
    correctnessFlag = 'c';

    // setting up empty Stack
    struct stackADT* my_stack = (struct stackADT*) malloc(sizeof(struct stackADT));
    my_stack->stack_top = NULL;
    my_stack->stack_size = 0;

    // setting up empty tree
    my_tree = (struct Tree*) malloc(sizeof(struct Tree));
    my_tree->root = NULL;

    // make tree node corr. to start symbol
    struct TreeNode* base_tree = (struct TreeNode*) malloc(sizeof(struct TreeNode));

    char* key = "<program'>";
    char type = 'N';
    int num = get_value(key, type);

    struct symbol* sym = (struct symbol*) malloc(sizeof(struct symbol));
    sym->num = num;
    sym->type = type;

    base_tree->line_num = 0;
    base_tree->sym.num = sym->num;
    base_tree->sym.type = sym->type;
    base_tree->key = str_copy(key);
    base_tree->rightsib = NULL;
    base_tree->child = NULL;
    base_tree->parent = NULL;

    my_tree->root = base_tree;

    // push start symbol onto stack 
    push(my_stack, sym, key, base_tree);

    tk curr_token = getNextToken();
    write_lexeme(lexemes, curr_token);
    while(1)
    {
        


        if(my_stack->stack_size == 0)
        {
            // Stack empty
            fprintf(syntax_errors, "Syntactical error (Stack empty and input remaining) (E4), at line_num: %d\n", curr_token->line_num);
            correctnessFlag = 'e';
            break;
        }

        struct stack_node * top_elem = top(my_stack);
        // printf("%s   ", Tglobal->entry[curr_token->type].key);
        // printf("%s\n", top_elem->key);
        if(top_elem->sym->type == 'T')
        {
            if(top_elem->sym->num == curr_token->type)
            {
                // Terminals match
                top_elem->address->property = curr_token->property;
                top_elem->address->line_num = curr_token->line_num;
                pop(my_stack);
                if(curr_token->type == get_value("$",'T'))
                {
                    break;
                }
                curr_token = getNextToken();
                write_lexeme(lexemes, curr_token);
                continue;
            }
            else
            {
                // Terminals mismatch
                fprintf(syntax_errors, "Syntactical error (terminal) (E1), at line_num: %d\n", curr_token->line_num);
                // printf("stack top: %s\t\t\t\tcurr_token: %s\n", top_elem->key, curr_token->property.lexeme);
                correctnessFlag = 'e';
                pop(my_stack);
                continue;
            }
        }
        else if(top_elem->sym->type == 'N')
        {
            int row = top_elem->sym->num;
            int col = curr_token->type;
            int rule_no = get_parse_table_entry(row,col);
            // printf("RULE NO: %d\n", rule_no);
            if(row == get_value(top_elem->key,'N'))
            {
                // printf("!!! %d !!!\n", rule_no);
            }
            if(rule_no != -1)
            {
                top_elem->address->rule = rule_no;
                struct node * rule = get_rule(rule_no);
                rule = rule->next;
                char* key = str_copy(rule->key);
                struct symbol sym = rule->sym;
                struct TreeNode* curr_tree_node = top_elem->address;
                insert_child(curr_tree_node, curr_token, key, sym);
                pop(my_stack);
                // curr_tree_node = go_down(curr_tree_node);
                curr_tree_node = curr_tree_node->child;
                if(rule->sym.num == get_value("epsilon", 'T') && rule->sym.type == 'T')
                {
                    
                    // printf("epsilon rule used for: %s %s\n", NTglobal->entry[row].key, Tglobal->entry[curr_token->type].key);
                    continue;
                }
                rule = rule->next;
                struct stackADT* temp_stack = (struct stackADT*) malloc(sizeof(struct stackADT));
                temp_stack->stack_top = NULL;
                temp_stack->stack_size = 0;
                push(temp_stack, &sym, key, curr_tree_node);
                while(rule)
                {
                    // if(curr_tree_node->parent)
                    // printf("%s DDDD %s\n Wait\n", curr_tree_node->key, curr_tree_node->parent->key);
                    key = str_copy(rule->key);
                    // printf("Inside1\n");
                    sym = rule->sym;
                    insert_right(curr_tree_node,curr_token,key,sym);
                    // go_right(curr_tree_node);
                    curr_tree_node = curr_tree_node->rightsib;
                    // printf("%s DDDD %s\n", curr_tree_node->key, curr_tree_node->parent->key);
                    push(temp_stack, &sym, key, curr_tree_node);
                    rule = rule->next;
                }
                // printf("Deep1\n");
                while(!isempty(temp_stack))
                {
                    // printf("Inside %d\n", temp_stack->stack_size);
                    struct stack_node * temp_top = top(temp_stack);
                    // if(temp_top == NULL)
                    // {
                    //     printf("WHY!!");
                    // }
                    // // printf("%s, ", temp_top->key); 
                    push(my_stack, temp_top->sym, temp_top->key, temp_top->address);
                    if(temp_stack->stack_size!=1)pop(temp_stack);
                    else break;
                }
                continue;
            }
            else
            {
                // Non terminal error recovery
                fprintf(syntax_errors, "Syntactical error (non-terminal) (E2), at line_num: %d\n", curr_token->line_num);
                correctnessFlag = 'e';
                /* INSERT RECOVERY CODE HERE */
                if(is_Present_sync_set(row, col)==1)
                {
                    pop(my_stack);
                    continue;
                }
                else
                {
                    if(curr_token->type == get_value("$",'T'))
                    {
                        break;
                    }
                    curr_token = getNextToken();
                    write_lexeme(lexemes, curr_token);
                } 
            }
        }
    }
    if(isempty(my_stack)==0)
    {
        fprintf(syntax_errors, "Syntactical error (Input empty and Stack remaining) (E3) \n");
        correctnessFlag = 'e';
    }
    
    // printParseTree(my_tree);
    fclose(lexemes);
    fclose(syntax_errors);
    return base_tree;
}

void print_parse_tree(char* fparsetree) {
    printParseTree(my_tree, fparsetree);
}

void print_to_console(char* fparsetree)
{
    FILE* fp = fopen(fparsetree, "r");
    if(fp==NULL)
    {
        printf("Error opening %s\n", fparsetree);
    }
    else
    {
        char c;
        while ((c = getc(fp)) != EOF)
        {
            putchar(c);
        }
        fclose(fp);
    }
}