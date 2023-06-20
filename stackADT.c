/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#include"stackADT.h"

//I have hard-coded maximum size of stack as 30, change if needed
//IMP NOTE
//when u initialize stack, ensure a dummy element is put as first element also set size=0 after this


int isfull(struct stackADT* mystack) {
    if(mystack->stack_size==30000) 
        return 1;
    return 0;
}

int isempty(struct stackADT* mystack) {
    if(mystack->stack_size==0) 
        return 1;
    return 0;
}

void pop(struct stackADT* mystack) {
    if(isempty(mystack)) {
        // printf("the stack is already empty\n");
        return;
    }
    struct stack_node *temp;
    temp=mystack->stack_top;
    mystack->stack_top=mystack->stack_top->next;
    mystack->stack_size--;
    // free(temp);
    return ;
}

struct stack_node* top(struct stackADT* mystack) {
    if(isempty(mystack)) {
        // printf("stack empty, nothing to print\n");
        return NULL;
    }
    return mystack->stack_top;
}

void push(struct stackADT* mystack, struct symbol *new_sym, char * new_key,struct TreeNode* new_address) {
    if(isfull(mystack)) {
        printf("the stack if full already\n");
        return;
    }
    struct stack_node* new_node= (struct stack_node*)malloc(sizeof(struct stack_node));
    
    
    struct symbol* sym=(struct symbol * ) malloc(sizeof(struct symbol));
    sym->num=new_sym->num;
    sym->type=new_sym->type;
    new_node->sym=sym;

    new_node->key=str_copy(new_key);

    new_node->address=new_address;
    
    new_node->next=mystack->stack_top;
    mystack->stack_top=new_node;
    mystack->stack_size++;
    return;
}

/*
int main() {
    //code to initialize the base element with value=-1 and type='T' 
    struct stackADT* mystack=(struct stackADT*)malloc(sizeof(struct stackADT));
    struct stack_node* dummy=(struct stack_node*)malloc(sizeof(struct stack_node));
    struct symbol* curr_sym=(struct symbol*)malloc(sizeof(struct symbol));
    curr_sym->num=-1;
    curr_sym->type='T';
    (curr_ele->sym)=*curr_sym;
    curr_ele->key=NULL;
    curr_ele->next=NULL;
    dummy->address=NULL;
    dummy->next=NULL;
    dummy->ele=*curr_ele;
    mystack->stack_top=dummy;
    mystack->stack_size=0;

    if(isempty(mystack))
        printf("my stack is empty\n");
    if(isfull(mystack))
        printf("my stack is full\n");

    struct stack_node* p1_dummy=(struct stack_node*)malloc(sizeof(struct stack_node));
    struct node* p1_curr_ele=(struct node*)malloc(sizeof(struct node));
    struct symbol* p1_curr_sym=(struct symbol*)malloc(sizeof(struct symbol));
    p1_curr_sym->num=2;
    p1_curr_sym->type='T';
    (p1_curr_ele->sym)=*p1_curr_sym;
    p1_curr_ele->key=NULL;
    p1_curr_ele->next=NULL;
    p1_dummy->address=NULL;
    p1_dummy->next=NULL;
    p1_dummy->ele=*curr_ele;
    
    pop(mystack);
    struct stack_node* new_take_node1=top(mystack);
    push(mystack,*p1_curr_ele,NULL);

    struct stack_node* new_take_node=top(mystack);
    printf("%d\n",new_take_node->ele.sym.num);
    if(isfull(mystack))
        printf("my stack is full\n");
    pop(mystack);
    return 1;

}
*/