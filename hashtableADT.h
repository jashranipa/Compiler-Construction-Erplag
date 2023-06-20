/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#define C1 4
#define C2 10

/*
Using quadratic probing with sum of ASCII value of all characters
Make sure that C1 and C2 such that all indices reached in quadratic probing
*/

struct hash_table
{
    struct hash_table_item * entry;
    int size;
};

struct hash_table_item
{
    char *key;
    int deleted;
};

int contains(char * key, struct hash_table* HT);
int insert(char * key, struct hash_table * HT);
char * str_copy(char *src);
int find_ASCII_sum(char * key);
int string_cmp(char * s1, char * s2);
