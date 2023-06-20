/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
union variant
{
    unsigned int value_int;
    double value_double;
    char lexeme[20];
    char no_value;
};

struct token_info {
    int type;
    unsigned int line_num;
    union variant property;
};
typedef struct token_info* tk;