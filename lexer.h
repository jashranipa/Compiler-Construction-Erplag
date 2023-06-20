/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#include"lexerDef.h"
#include"parserDef.h"

void setup_lexer(FILE* fp, int buffer_size);
tk getNextToken();
void removeComments(char* testcase_file, char* clean_file);