/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#include"treeADT_1.h"
#include<string.h>


void insert_child(struct TreeNode* curr,tk newtoken,char *key,struct symbol newsym) {
    struct TreeNode* new_treeNode=(struct TreeNode*)malloc(sizeof(struct TreeNode));

    new_treeNode->line_num = newtoken->line_num;
    new_treeNode->property = newtoken->property;
    
    new_treeNode->sym.num=newsym.num;
    new_treeNode->sym.type=newsym.type;

    new_treeNode->key=str_copy(key);
    
    new_treeNode->parent=curr;
    new_treeNode->child=NULL;
    new_treeNode->rightsib=NULL;

    curr->child=new_treeNode;
    return;
}

void insert_right(struct TreeNode* curr,tk newtoken,char *key,struct symbol newsym){
    struct TreeNode* new_treeNode=(struct TreeNode*)malloc(sizeof(struct TreeNode));

    new_treeNode->line_num = newtoken->line_num;
    new_treeNode->property = newtoken->property;
    
    new_treeNode->sym.num=newsym.num;
    new_treeNode->sym.type=newsym.type;

    new_treeNode->key=str_copy(key);
    
    new_treeNode->parent=curr->parent;
    new_treeNode->child=NULL;
    new_treeNode->rightsib=NULL;
    curr->rightsib=new_treeNode;
    return;
}

struct TreeNode* go_down(struct TreeNode* curr) {
    if(curr==NULL) {
        printf("pointer provided is bad");
        return NULL;
    }
    return curr->child;
}

struct TreeNode* go_right(struct TreeNode* curr) {
    if(curr==NULL) {
        printf("pointer provided is bad");
        return NULL;
    }
    return curr->rightsib;
}

struct TreeNode* go_up(struct TreeNode* curr) {
    if(curr==NULL) {
        printf("pointer provided is bad");
        return NULL;
    }
    return curr->parent;
}

void print_node(struct TreeNode* curr, FILE * ptr)
{
    int id_value = get_value("ID", 'T');
    int num_value = get_value("NUM", 'T');
    int rnum_value = get_value("RNUM", 'T');

    // lexeme
    if(curr->sym.type == 'N')
    {
        // printf("-----\t");
        fprintf(ptr, "-----\t");
    }
    else
    {
        if (curr->sym.num == id_value) {
            // printf("%s\t",curr->property.lexeme);
            fprintf(ptr, "%s\t",curr->property.lexeme);
            
        }
        else if (curr->sym.num == num_value) {
            // printf("%d\t",curr->property.value_int);
            int temp = curr->property.value_int;
            fprintf(ptr, "%d\t" , temp);
        }
        else if (curr->sym.num == rnum_value) {
            // printf("%f\t", curr->property.value_double);
            double temp = curr->property.value_double;
            fprintf(ptr, "%f\t",temp);
        }
        else {
            // printf("%s\t", Tglobal->entry[curr->sym.num].key);
            fprintf(ptr, "%s\t" , Tglobal->entry[curr->sym.num].key);
        }
    }

    // LINE NO.
    if (curr->sym.type == 'T') {
        // give proper line no
        // printf("%d\t", curr->line_num);
        unsigned int line_num_curr=curr->line_num;
        fprintf(ptr, "%d\t",line_num_curr);
    }
    else {
        // hardcode 0
        // printf("0\t");
        int line_num = 0;
        
        fprintf(ptr, "%d\t", line_num);
    }
    
    // TOKEN NAME
    if(curr->sym.type == 'N')
    {
        // What to print here? dummy value
        // printf("-----\t");
        fprintf(ptr, "-----\t");
    }
    else
    {
        char* mytoken_name = (Tglobal->entry[(curr->sym).num]).key;
        // printf("%s\t", mytoken_name);
        fprintf(ptr , "%s\t", mytoken_name);
    }

    // VALUE IF NUM
    if(curr->sym.type == 'T')
    {
        if(rnum_value == (curr->sym).num)
        {
            // What to print here?
            // printf("%f\t", curr->property.value_double);
            double temp = curr->property.value_double;
            
            fprintf(ptr, "%f\t" , temp);
        }
        if(num_value == (curr->sym).num)
        {
            // What to print here?
            // printf("%d\t",curr->property.value_int);
            int temp = curr->property.value_int;
            
            
            fprintf(ptr, "%d\t" , temp);
        }
        else
        {
            // What to print here? -- dummy value
            // printf("-----\t");
            fprintf(ptr, "-----\t");
        }
    }
    else
    {
        // What to print here? -- dummy value
        // printf("-----\t");
        fprintf(ptr, "-----\t");
    }

    // PARENT NODE SYMBOL
    if(curr->parent == NULL)
    {
        // printf("ROOT\t");
        fprintf(ptr, "ROOT\t");
    }
    else
    {
        // printf("%s\t",curr->parent->key);
        fprintf(ptr, "%s\t",curr->parent->key);
    }

    // IF LEAF
    if(curr->sym.type == 'T')
    {
        // printf("Yes\t");
        fprintf(ptr, "Yes\t");
    }
    else
    {
        // printf("No\t");
        fprintf(ptr, "No\t");
    }

    // NODE SYMBOL
    if(curr->sym.type == 'N')
    {
        // printf("%s\t",NTglobal->entry[curr->sym.num].key);
        fprintf(ptr, "%s\t",NTglobal->entry[curr->sym.num].key);
    }
    else
    {
        // printf("-----\t");
        fprintf(ptr, "-----\t");
    }
    // printf("\n");
    fprintf(ptr, "\n");
}

void inorder_print(struct TreeNode *curr,FILE *ptr) {
    // //traverse to the left
    // if(curr==NULL) 
    // {
    //     return;
    // }
    
    // inorder_print(curr->child,ptr);

    // //print the current values
    // if(curr->sym.type=='N') {
    //     fputs("-----",ptr);
    // }
    // else {
    //     //how to print a terminal lexeme
    //     // fputs(curr->property,ptr);
    // }
    // /* extract line number and the token value and complete this */
    // unsigned int line_num_curr=curr->line_num;
    // putw(line_num_curr,ptr);
    // char* mytoken_name = (Tglobal->entry[(curr->sym).num]).key;
    // fputs(mytoken_name,ptr);
    // //pending to print the token value (prob is about how to print value of union)
    
    // fputs(curr->parent->key,ptr);
    // if(curr->sym.type=='T')
    //     fputs("Yes",ptr);
    // else {
    //     fputs("No",ptr);
    //     fputs(curr->key,ptr);
    // }
    // fputc('\n',ptr);
    // //traverse to the right
    
    // inorder_print(curr->rightsib,ptr);

    // return;

    if(curr == NULL)
    {
        return;
    }
    inorder_print(curr->child, ptr);
    print_node(curr, ptr);
    struct TreeNode * temp = curr->child;
    if(temp)
    {
        temp = temp->rightsib;
        while(temp)
        {
            inorder_print(temp,ptr);
            temp = temp->rightsib;
        }
    }
}

void printParseTree(struct Tree* mytree, char* fparsetree) {
    FILE *ptr=fopen(fparsetree,"w");
    inorder_print(mytree->root,ptr);
    fclose(ptr);
    return;
}

void count_nodes(struct TreeNode* node, int* count)
{
    if (node == NULL)
    {
        return;
    }
    *count = *count + 1;

    struct TreeNode* temp = node->child;
    while (temp)
    {
        count_nodes(temp, count);
        temp = temp->rightsib;
    }
}
/*
int main() {
    struct Tree *mytree=(struct Tree*)malloc(sizeof(struct Tree));
    struct TreeNode *dummy_tree=(struct TreeNode*)malloc(sizeof(struct TreeNode));
    struct symbol *dummy_symbol_tree=(struct symbol*)malloc(sizeof(struct symbol));
    tk dummy_token_tree=(tk)malloc(sizeof(struct token_info));
    dummy_symbol_tree->num=-1;
    dummy_symbol_tree->type='T';
    dummy_token_tree->line_num=-1;
    dummy_token_tree->type=-2;
    dummy_token_tree->property.value_int=-1;

    dummy_tree->key=NULL;
    dummy_tree->mytoken=dummy_token_tree;
    dummy_tree->sym=*dummy_symbol_tree;
    mytree->root=dummy_tree;


    
    struct single_node *dummy_single_node1=(struct single_node*)malloc(sizeof(struct single_node));
    struct symbol *dummy_symbol1=(struct symbol*)malloc(sizeof(struct symbol));
    dummy_symbol1->num=2;
    dummy_symbol1->type='T';
    dummy_single_node1->key=NULL;
    dummy_single_node1->sym=*dummy_symbol1;
    insertchild(mytree->root,*dummy_single_node1);  
    struct TreeNode* curr=godown(mytree->root); 
    printf("%d\n",curr->value.sym.num);
    //inserting right child
    struct single_node *dummy_single_node2=(struct single_node*)malloc(sizeof(struct single_node));
    struct symbol *dummy_symbol2=(struct symbol*)malloc(sizeof(struct symbol));
    dummy_symbol2->num=4;
    dummy_symbol2->type='T';
    dummy_single_node2->key=NULL;
    dummy_single_node2->sym=*dummy_symbol2;

    insertRight(curr,*dummy_single_node2);
    curr=goright(curr);
    printf("%d\n",curr->value.sym.num);

    //go up now
    curr=goup(curr);
    printf("%d\n",curr->value.sym.num);
    return 0;
}
*/