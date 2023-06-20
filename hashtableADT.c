/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#include<stdio.h>
#include<stdlib.h>
#include "hashtableADT.h"

/*
Compares if both string is equal
Returns 1 if both strings are equal
*/
int string_cmp(char * s1, char * s2)
{
    if(!s1 || !s2)return 0;
    int i = 0;
    while(s1[i]!='\0' && s2[i] != '\0')
    {
        if(s1[i]!=s2[i])return 0;
        i++;
    }
    if(s1[i] == s2[i])return 1;
    else return 0;
}

/*
Find ASCII sum of all characters in the string
*/
int find_ASCII_sum(char * key)
{
    int ASCII_sum = 0, index = 0;
    while(key[index] != '\0')
    {
        ASCII_sum += key[index++];
    }
    return ASCII_sum;
}

/*
Return index of hash table entry if the key is found
Else returns -1 if not found
*/
int contains(char * key, struct hash_table* HT)
{
    char * eps = "epsilon";
    if(string_cmp(key, eps)==1)
    {
        // printf("Enter 1\n");
        // printf("Value: %s",(HT->entry[0]).key);
        if(string_cmp((HT->entry[0]).key, key)==1)
        {
            // printf("Entry 2\n");
            return 0;
        }
    }
    int sum = find_ASCII_sum(key);
    int i = 0;
    while(i != HT->size)
    {
        int j = (sum + i)%(HT->size);
        if((HT->entry[j]).deleted == -1)return -1;
        if((HT->entry[j]).deleted==1)continue;

        if(string_cmp((HT->entry[j]).key, key) == 1)
        {
            return j;
        }

        i++;
    }
    return -1;
}

char * str_copy(char *src)
{
    int length = 0;
    while(src[length] != '\0')
    {
        length++;
    }
    char * dest = malloc((length+1)*sizeof(char));
    for(int i = 0; i <= length; i++)
    {
        dest[i] = src[i];
    }
    return dest;
}

/*
Inserts the key into hash table
Return -1 if not inserted
*/
int insert(char * key, struct hash_table * HT)
{
    int j = contains(key, HT);
    char * eps = "epsilon";
    if(string_cmp(key, eps)==1)
    {
        (HT->entry[0]).key = eps;
        (HT->entry[0]).deleted = 0;
        return 0;
    }
    if(j != -1)return j;
    int i = 0;
    int sum = find_ASCII_sum(key);
    while(i < HT->size)
    {
        int j = (sum + i)%(HT->size);
        if((HT->entry[j]).deleted == -1)
        {
            char * data = str_copy(key);
            (HT->entry[j]).key = data;
            (HT->entry[j]).deleted = 0;
            return j;
        }
        else if((HT->entry[j]).deleted == 1)
        {
            char * data = str_copy(key);
            (HT->entry[j]).key = data;
            (HT->entry[j]).deleted = 0;
            return j;
        }
        i++;
    }
    return -1;
}