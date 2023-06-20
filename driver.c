/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/

/*
STATUS OF OUR CODE:
a) FIRST and FOLLOW are automated
b) Both lexical and syntax analysis modules are implemented
c) Modules works with all testcases
d) All specified options (0 through 5) are working.
*/
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include"code_gen.h"

#define MAX_ERRORS 1000

int main(int argc, char* argv[]) {
    printf("Welcome to our compiler project\n");

    if (argc != 3) {
        printf("Arguments invalid\n");
        exit(0);
    }

    char* fname = argv[1];
    char* fparsetree = "parseTreeOp.txt";

    /* TO USE IN NASM CODE GEN. */
    char* codeFile = argv[2];


    int choice=1;
    int buffer_size = 4000;

    printf("\nLEVEL 4: AST Creation | Symbol Table | Type Checking | Semantic Rules | Static and Dynamic Arrays in Type Checking\n");
    printf("The above modules work.\n\n");
    
    /* To be called only once */

    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;
    start_time = clock();


    setup_parser();

    FILE* fp_2 = fopen(fname, "r");            
    setup_lexer(fp_2, buffer_size);

    struct TreeNode* base_tree = make_parsetree();
    // printf("HI\n");
    print_parse_tree(fparsetree);

    // case 4
    int parse_tree_count = 0, ast_count = 0;
    struct TreeNode* p_tree = base_tree;
    struct TreeNode* ast = base_tree;
    count_nodes(p_tree, &parse_tree_count);
    struct FT* function_table = function_table_constructor();
    struct ST * symbol_table = symbol_table_constructor();
    semantic_correctness_flag = 'c';

    // only if syntax is correct 
    if(correctnessFlag == 'c')
    {
        get_AST(base_tree);
        count_nodes(ast->syn_addr, &ast_count);

        ptr = fopen("semantic_errors.txt","w");
        if(ptr == NULL)
        {
            printf("Error opening semantic_errors.txt\n");
        }

        // case 5
        make_symbol_table(base_tree->syn_addr, symbol_table, 0, function_table);
        fclose(ptr);
        if (semantic_correctness_flag == 'c')
        {
            // printf("Code compiled successfully...!\n");
        }
    }
    // printf("hello\n");
    end_time = clock();
    total_CPU_time = (double) (end_time - start_time);
    total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

    while(choice != 0){
        printf("\n\nPlease enter your choice\n\n");
        printf(" 0) Exit \n");
        printf(" 1) Lexer - print token stream\n");
        printf(" 2) Parser - correctness and parse tree\n");
        printf(" 3) AST - print AST\n");
        printf(" 4) Memory - view compression ratio\n");
        printf(" 5) Symbol Table - print symbol table\n");
        printf(" 6) Activation Record Size\n");
        printf(" 7) Static and Dynamic Arrays\n");
        printf(" 8) Error Reporting and Total Computing Time\n");
        printf(" 9) Code Generation\n");

        /* DELETE BEFORE SUBMISSION */
        // printf(" -----\n");
        // printf(" 10) Remove Comments\n");
        // printf(" 11) Exec. Time of Lexer & Parser\n");

        printf("\n");
        scanf("%d",&choice);
        
        switch(choice) {
            case 0:
            {
                printf("\n");
                printf("Exiting Program\n");
                break;
            } 

            // case 10: // remove comments
            // {
            //     // printf("enter the filename having code: ");
            //     // char code_file_1[20];
            //     // scanf("%s",code_file_1);
            //     printf("\n");
            //     char* no_comments="commentless.txt";
            //     removeComments(fname,no_comments);

            //     FILE *ptr=fopen(no_comments,"r");
            //     char curr=getc(ptr);
            //     while(curr!=EOF){
            //         printf("%c",curr);
            //         curr=getc(ptr);
            //     }
            //     fclose(ptr);
            //     break;
            // }
            
            // case 11: // exec time lexer parser
            // {    
            //     // printf("enter the filename having code: ");
            //     // char code_file_4[20];
            //     // scanf("%s",code_file_4);
            //     // printf("enter the buffer size: ");
            //     // int buffer_size_4;
            //     // scanf("%d",&buffer_size_4);
            //     printf("\n");
            //     FILE* fp_4 = fopen(fname, "r");

            //     clock_t start_time, end_time;
            //     double total_CPU_time, total_CPU_time_in_seconds;
            //     start_time = clock();
                
            //     // invoke lexer and parser
            //     setup_lexer(fp_4, buffer_size);
            //     make_parsetree();

            //     end_time = clock();
            //     total_CPU_time = (double) (end_time - start_time);
            //     total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

            //     // print time taken
            //     printf("%f\n", total_CPU_time);
            //     printf("%f\n", total_CPU_time_in_seconds);
            //     break;
            // }
            
            case 1: // lexer
            {
                printf("\n");
                printf("Printing the lexemes \n\n");
                
                
                print_to_console("lexemes.txt");
                    

                break;
            }
            case 2: // parser
            {
                printf("\n");
                print_to_console(fparsetree);
                break;
            }
            case 3: // ast
            {
                printf("\n");
                // FILE* fp_5 = fopen(fname, "r");

                // invoke lexer and parser
                // setup_lexer(fp_5, buffer_size);
                // struct TreeNode* base_tree = make_parsetree();
                // printf("Reached1\n");
                // get_AST(base_tree);
                // printf("Reached2\n");

                // pre-order traversal
                printf("ORDER OF TRAVERSAL: Pre-Order\n\n");

                printf("Lexeme");
                printf("\t\t\t");
                printf("Current_node");
                printf("\t");
                printf("Line_no");
                printf("\t\t\t");
                printf("Token_name");
                printf("\t\t\t");
                printf("Parent_node");
                printf("\t\t\t");
                printf("isLeaf (y/n)");
                printf("\t\t\t\n\n");

                print_AST(base_tree->syn_addr);
                // printf("Reached3\n");
                break;
            }
            case 4: // memory
            {
                printf("\n");
                // int parse_tree_count = 0, ast_count = 0;

                // FILE* fp_7 = fopen(fname, "r");

                // invoke lexer and parser
                // setup_lexer(fp_7, buffer_size);
                // struct TreeNode* base_tree = make_parsetree();

                // printf("\n\n");

                // struct TreeNode* p_tree = base_tree;
                // struct TreeNode* ast = base_tree;
                // count_nodes(p_tree, &parse_tree_count);

                // get_AST(base_tree);

                // count_nodes(ast->syn_addr, &ast_count);

                int node_size = sizeof(struct TreeNode);
                int parse_tree_mem = node_size * parse_tree_count;
                int ast_mem = node_size * ast_count;

                printf("Parse Tree Nodes Count: %d\t\t Allocated Memory: %d\n", parse_tree_count, parse_tree_mem);
                printf("AST Nodes Count: %d\t\t\t Allocated Memory: %d\n", ast_count, ast_mem);

                double compression_ratio = ((parse_tree_mem - ast_mem)*100 / parse_tree_mem);
                printf("\nCompression Percentage: %f  %%\n", compression_ratio);
                

                break;
            }
            case 5: // symbol table
            {
                printf("\n");
                // FILE* fp_5 = fopen(fname, "r");

                // invoke lexer and parser
                // setup_lexer(fp_5, buffer_size);
                // struct TreeNode* base_tree = make_parsetree();
                // get_AST(base_tree);
                // printf("Created AST\n");
                // struct FT* function_table = function_table_constructor();
                // struct ST * symbol_table = symbol_table_constructor();
                // make_symbol_table(base_tree->syn_addr, symbol_table, 0, function_table);
                // printf("Symbol table created\n");
                print_symbol_table(function_table);
                break;
            }
            case 6: // activation record size
            {
                printf("\n");
                // FILE* fp_5 = fopen(fname, "r");

                // invoke lexer and parser
                // setup_lexer(fp_5, buffer_size);
                // struct TreeNode* base_tree = make_parsetree();
                // get_AST(base_tree);
                // struct FT* function_table = function_table_constructor();
                // struct ST * symbol_table = symbol_table_constructor();
                // make_symbol_table(base_tree->syn_addr, symbol_table, 0, function_table);
                activation_record_size(function_table);
                break;
            }
            case 7: //Static and dynamic Array
            {
                printf("\n");
                // FILE* fp_5 = fopen(fname, "r");

                // invoke lexer and parser
                // setup_lexer(fp_5, buffer_size);
                // struct TreeNode* base_tree = make_parsetree();
                // get_AST(base_tree);
                // struct FT* function_table = function_table_constructor();
                // struct ST * symbol_table = symbol_table_constructor();
                // make_symbol_table(base_tree->syn_addr, symbol_table, 0, function_table);
                print_array_table(function_table);
                break;
            }
            case 8: // total computing time
            {
                printf("\n");

                // FILE* fp_7 = fopen(fname, "r");

                

                // invoke lexer and parser
                // setup_lexer(fp_7, buffer_size);
                // struct TreeNode* base_tree = make_parsetree();
                // printf("\n\n");

                // if no error, run AST & symbolTable.c
                if (correctnessFlag == 'c')
                {
                    // get_AST(base_tree);
                    // semantic_correctness_flag = 'c';
                    // struct FT* function_table = function_table_constructor();
                    // struct ST * symbol_table = symbol_table_constructor();
                    // make_symbol_table(base_tree->syn_addr, symbol_table, 0, function_table);
                    if (semantic_correctness_flag == 'c')
                    {
                        printf("Code compiled successfully...!\n");
                    }
                    else
                    {
                        print_to_console("semantic_errors.txt");
                    }
                }
                else
                {
                    print_to_console("lexical_errors.txt");
                    print_to_console("syntax_errors.txt");
                    
                }

                // end_time = clock();
                // total_CPU_time = (double) (end_time - start_time);
                // total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

                // print time taken
                printf("Total CPU Time: %f\n", total_CPU_time);
                printf("Total CPU Time (s): %f\n", total_CPU_time_in_seconds);

                break;
            }
            case 9:
            {
                printf("\n");
                
                struct FT* function_table_1 = function_table_constructor();
                struct ST * symbol_table_1 = symbol_table_constructor();
                make_symbol_table(base_tree->syn_addr, symbol_table_1, 0, function_table_1);
                // use symbol_table_1 and function_table_1.
                struct quad_arr * quad_arr = setup_gen_ir();
                quad_arr = gen_ir(base_tree->syn_addr);
                printf("Intermediate Code Generated!!!\n");
                FILE * output = fopen(codeFile, "w");
                printf("curr : %d\n", quad_arr->curr);
                print_symbol_table(function_table_1);
                generate_code(output, function_table_1, quad_arr);
                break;
            }
            default:
            {
                printf("\n");
                printf("Invalid Choice\n");
                break;
            }
        }

    }

    printf("Compiler executed Successfully!!!\n");
    abort();

    return 0;
}