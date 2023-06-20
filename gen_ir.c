/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/

#include<stdio.h>
#include"gen_ir.h"

int label_count;

void assign_st(struct TreeNode * node, struct ST * ST)
{
    if(!node)
    {
        return;
    }
    node->ST = ST;
    struct TreeNode * temp = node->child;
    while(temp)
    {
        assign_st(temp, ST);
        temp = temp->rightsib;
    }
}

struct quad_node* get_next_quad()
{
    if(curr_node == qa->curr)return NULL;
    // printf("%d\n", curr_node);
    struct quad_node* curr = &(curr_block->nodes[curr_node]);
    // printf("%d, %s, %s, %s\n", curr->op, curr->arg1, curr->arg2, curr->res);
    curr_node = curr_node + 1;
    if (curr_node >= BLOCK_SIZE)
    {
        curr_block = curr_block->next;
        curr_node = 0;
    }
    return curr;
}

char * new_label()
{
    char * label = (char *)malloc(sizeof(char)*LABEL_LEN);
    snprintf(label, LABEL_LEN,"label%d", labels);
    labels++;
    return label;
}

/*
Initialize global variable qa (quad_arr)
*/
struct quad_arr * setup_gen_ir() 
{
    label_count = 0;

    struct quad_node* qn_arr = (struct quad_node*) malloc(BLOCK_SIZE * sizeof(struct quad_node));
    struct quad_block* qb = (struct quad_block*) malloc(sizeof(struct quad_block));
    qb->nodes = qn_arr;
    qb->next = NULL;
    qa = (struct quad_arr*) malloc(sizeof(struct quad_arr));
    qa->head = qb;
    if(!qb->nodes)
    {
        printf("NULL qb\n");
    }
    qa->tail = qb;
    qa->curr = 0;
    curr_node = 0;
    curr_block = qa->head;
    labels = 0;
}

/*
Generate temp_label
*/
char* temp_label()
{
    char * label = (char *) malloc(sizeof(char) * LABEL_LEN);
    snprintf(label, LABEL_LEN, "temp%d", label_count);
    label_count = (label_count + 1) % 1000;
    return label;
}

/*
Called when current block is full
malloc for next block
*/
void add_next_qb() 
{
    struct quad_node* qn_arr = (struct quad_node*) malloc(BLOCK_SIZE * sizeof(struct quad_node));
    struct quad_block* qb = (struct quad_block*) malloc(sizeof(struct quad_block));
    qb->nodes = qn_arr;
    qb->next = NULL;

    qa->tail->next = qb;
    qa->tail = qb;
    qa->curr = 0;
}

/*
returns 1 if qb is full
else 0
*/
int is_full()
{
    if (qa->curr >= BLOCK_SIZE)
    return 1;
    else    
        return 0;
}

struct FT_entry * get_FT_entry(struct ST * ST)
{
    struct ST * temp = ST;
    while(temp->identifier == 'S')
    {
        temp = temp->par.ST;
    }
    return temp->par.FT_entry;
}

/*
returns 1 if node is non-terminal
else 0
*/
int is_non_terminal(struct TreeNode* node)
{
    if (node->label == NULL)
        return 0;
    else
        return 1;
}

/*
write to qa
*/
void write_quad(enum operators op, char* arg1, char* arg2, char* res, struct ST * ST)
{
    if(!qa->tail)
    {
        printf("NULL qa !!!!!!!!\n");
    }
    printf("IN write_quad\n");
    
    printf("curr: %d\n", qa->curr);
    struct quad_node * curr = &(qa->tail->nodes[qa->curr]);
    (qa->tail->nodes[qa->curr]).op = op;
    (qa->tail->nodes[qa->curr]).arg1 = arg1;
    (qa->tail->nodes[qa->curr]).arg2 = arg2;
    (qa->tail->nodes[qa->curr]).res = res;
    (qa->tail->nodes[qa->curr]).ST = ST;
    qa->curr++;
    if (is_full())
        add_next_qb();  
    printf("%d, %s, %s, %s\n",qa->tail->nodes[qa->curr - 1].op, qa->tail->nodes[qa->curr - 1].arg1,qa->tail->nodes[qa->curr - 1].arg2,qa->tail->nodes[qa->curr - 1].res);
}

/*
IMPLEMENTATION PENDING
acess enum operators & return correct op
*/
enum operators find_op(char* label)
{

}


struct quad_arr * gen_ir(struct TreeNode* node)
{
    // struct TreeNode * child = node->child;
    if(string_cmp(node->label, "PROGRAM"))
    {
        struct TreeNode * temp = node->child;
        while(temp)
        {
            gen_ir(temp);
            temp = temp->rightsib;
        }
        printf("curr: %d\n", qa->curr);
    }
    else if(string_cmp(node->label, "MOD_DECS") || string_cmp(node->label, "OTHER_1") || string_cmp(node->label, "OTHER_2"))
    {

    }
    else if(string_cmp(node->label, "DRIVER"))
    {
        struct TreeNode * temp = node->child->child;
        while(temp)
        {
            gen_ir(temp);
            printf("%s\n", temp->label);
            temp = temp->rightsib;
        }
        // printf("curr: %d\n", qa->curr);
        // printf("Reached safely\n");
    }
    else if(string_cmp(node->label, "GET_VALUE"))
    {
        write_quad(GET_VALUE, NULL, NULL, /*Extract from ST_entry*/ node->child->property.lexeme, node->ST);
        struct ST_entry * entry = extract_type(node->child->property.lexeme, node->ST);
        if(entry != NULL)
        {
            // Semantic error
        }
        else
        {
            if(entry->identifier == 'A')
            {

            }
            else
            {
                write_quad(GET_VALUE, NULL, NULL, node->child->property.lexeme, node->ST);
            }
        }
    }
    else if(string_cmp(node->label, "PRINT"))
    {
        // printf("IN PRINT\n");
        if(string_cmp(node->child->key, "ID"))
        {
            if(node->child->rightsib)
            {

            }
            else
            {
                
                write_quad(PRINT, NULL, NULL, node->child->property.lexeme, node->ST);
            }
        }
    }
    else if(string_cmp(node->label, "ASID"))
    {
        //evaluate right expression
        // gen_ir(node->child->rightsib);
        // char * res = temp_label();
        // node->child->nt_label = res;
        // write_quad(ASSIGNOP, node->child->rightsib->nt_label, NULL, res, node->ST);
        // write_quad(ASSIGNOP, res, NULL, /*Extract from ST_entry*/ node->child->property.lexeme, node->child->ST);
        //ASSIGN node->ST
        // printf("Entering ASID\n");
        // if(!node->ST)
        // {
        //     printf("NULL ST in ASID in IR!!!!!!!!!!!!!!!!\n");
        // }
        printf("IN ASID\n");
        assign_st(node, node->ST);
        // node->child->rightsib->ST = node->ST;
        gen_ir(node->child->rightsib);
        write_quad(ASSIGNOP, node->child->rightsib->nt_label,NULL, node->child->property.lexeme, node->ST);
    }
    else if(string_cmp(node->label, "PLUS"))
    {
        // if(!node->ST)
        // {
        //     printf("NULL ST in PLUS in IR!!!!!!!!!!!!!!!!\n");
        // }
        node->child->ST = node->ST;
        gen_ir(node->child);
        node->child->rightsib->ST = node->ST;
        gen_ir(node->child->rightsib);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        write_quad(PLUS, node->child->nt_label, node->child->rightsib->nt_label, res, node->ST);
        node->nt_label = str_copy(res);
    }
    else if(string_cmp(node->label, "MINUS"))
    {
        // if(!node->ST)
        // {
        //     printf("NULL ST in MINUS in IR!!!!!!!!!!!!!!!!\n");
        // }
        node->child->ST = node->ST;
        gen_ir(node->child);
        node->child->rightsib->ST = node->ST;
        gen_ir(node->child->rightsib);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        write_quad(MINUS, node->child->nt_label, node->child->rightsib->nt_label, res, node->ST);
        node->nt_label = str_copy(res);
    }
    else if(string_cmp(node->label, "UNARY_MINUS"))
    {
        node->child->ST = node->ST;
        gen_ir(node->child);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        write_quad(MUL, node->child->nt_label, "-1", res, node->ST);
        node->nt_label = str_copy(res);
    }
    else if(string_cmp(node->label, "UNARY_PLUS"))
    {
        node->child->ST = node->ST;
        gen_ir(node->child);
        node->nt_label = str_copy(node->child->nt_label);
    }
    else if(string_cmp(node->label, "MUL"))
    {
        // if(!node->ST)
        // {
        //     printf("NULL ST in MUL in IR!!!!!!!!!!!!!!!!\n");
        // }
        node->child->ST = node->ST;
        gen_ir(node->child);
        node->child->rightsib->ST = node->ST;
        gen_ir(node->child->rightsib);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        node->nt_label = str_copy(res);
        write_quad(MUL, node->child->nt_label, node->child->rightsib->nt_label, node->nt_label, node->ST);
    }
    else if(string_cmp(node->label, "DIV"))
    {
        // if(!node->ST)
        // {
        //     printf("NULL ST in DIV in IR!!!!!!!!!!!!!!!!\n");
        // }
        node->child->ST = node->ST;
        gen_ir(node->child);
        node->child->rightsib->ST = node->ST;
        gen_ir(node->child->rightsib);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        write_quad(MINUS, node->child->nt_label, node->child->rightsib->nt_label, res, node->ST);
        node->nt_label = str_copy(res);
    }
    else if(string_cmp(node->label, "AND"))
    {
        // if(!node->ST)
        // {
        //     printf("NULL ST in PLUS in IR!!!!!!!!!!!!!!!!\n");
        // }
        node->child->ST = node->ST;
        gen_ir(node->child);
        node->child->rightsib->ST = node->ST;
        gen_ir(node->child->rightsib);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        write_quad(AND, node->child->nt_label, node->child->rightsib->nt_label, res, node->ST);
        node->nt_label = str_copy(res);
    }
    else if(string_cmp(node->label, "OR"))
    {
        // if(!node->ST)
        // {
        //     printf("NULL ST in PLUS in IR!!!!!!!!!!!!!!!!\n");
        // }
        node->child->ST = node->ST;
        gen_ir(node->child);
        node->child->rightsib->ST = node->ST;
        gen_ir(node->child->rightsib);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        write_quad(OR, node->child->nt_label, node->child->rightsib->nt_label, res, node->ST);
        node->nt_label = str_copy(res);
    }
    else if(string_cmp(node->label, "LT"))
    {
        // if(!node->ST)
        // {
        //     printf("NULL ST in PLUS in IR!!!!!!!!!!!!!!!!\n");
        // }
        node->child->ST = node->ST;
        gen_ir(node->child);
        node->child->rightsib->ST = node->ST;
        gen_ir(node->child->rightsib);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        write_quad(LT, node->child->nt_label, node->child->rightsib->nt_label, res, node->ST);
        node->nt_label = str_copy(res);
    }
    else if(string_cmp(node->label, "LE"))
    {
        // if(!node->ST)
        // {
        //     printf("NULL ST in PLUS in IR!!!!!!!!!!!!!!!!\n");
        // }
        node->child->ST = node->ST;
        gen_ir(node->child);
        node->child->rightsib->ST = node->ST;
        gen_ir(node->child->rightsib);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        write_quad(LE, node->child->nt_label, node->child->rightsib->nt_label, res, node->ST);
        node->nt_label = str_copy(res);
    }
    else if(string_cmp(node->label, "GT"))
    {
        // if(!node->ST)
        // {
        //     printf("NULL ST in PLUS in IR!!!!!!!!!!!!!!!!\n");
        // }
        node->child->ST = node->ST;
        gen_ir(node->child);
        node->child->rightsib->ST = node->ST;
        gen_ir(node->child->rightsib);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        write_quad(GT, node->child->nt_label, node->child->rightsib->nt_label, res, node->ST);
        node->nt_label = str_copy(res);
    }
    else if(string_cmp(node->label, "GE"))
    {
        // if(!node->ST)
        // {
        //     printf("NULL ST in PLUS in IR!!!!!!!!!!!!!!!!\n");
        // }
        node->child->ST = node->ST;
        gen_ir(node->child);
        node->child->rightsib->ST = node->ST;
        gen_ir(node->child->rightsib);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        write_quad(GE, node->child->nt_label, node->child->rightsib->nt_label, res, node->ST);
        node->nt_label = str_copy(res);
    }
    else if(string_cmp(node->label, "EQ"))
    {
        // if(!node->ST)
        // {
        //     printf("NULL ST in PLUS in IR!!!!!!!!!!!!!!!!\n");
        // }
        node->child->ST = node->ST;
        gen_ir(node->child);
        node->child->rightsib->ST = node->ST;
        gen_ir(node->child->rightsib);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        write_quad(EQ, node->child->nt_label, node->child->rightsib->nt_label, res, node->ST);
        node->nt_label = str_copy(res);
    }
    else if(string_cmp(node->label, "NE"))
    {
        // if(!node->ST)
        // {
        //     printf("NULL ST in PLUS in IR!!!!!!!!!!!!!!!!\n");
        // }
        node->child->ST = node->ST;
        gen_ir(node->child);
        node->child->rightsib->ST = node->ST;
        gen_ir(node->child->rightsib);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        write_quad(NE, node->child->nt_label, node->child->rightsib->nt_label, res, node->ST);
        node->nt_label = str_copy(res);
    }
    else if(string_cmp(node->label, "FACTOR"))
    {
        // No array
        if(node->child->rightsib == NULL)
        {
            char * res = temp_label();
            int entry_no = insert_st(res, node->ST);
            if(entry_no == -1)
            {

            }
            else
            {
                node->ST->entry[entry_no].deleted = 0;
                node->ST->entry[entry_no].dt.pt.p_type = 'I';
                node->ST->entry[entry_no].identifier = 'P';
                node->ST->entry[entry_no].width = 2;
                node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
                get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
                node->nt_label = str_copy(res);
                write_quad(ASSIGNOP, node->child->property.lexeme, NULL, node->nt_label, node->ST);
            }
        }
        else
        {
            /*
            // gen_ir(node->child->rightsib->child);
            // char * res = temp_label();
            // int entry_no = insert_st(res, node->ST);
            // if(entry_no == -1)
            // {

            // }
            // else
            // {
            //     node->ST->entry[entry_no].deleted = 0;
            //     node->ST->entry[entry_no].dt.pt.p_type = 'I';
            //     node->ST->entry[entry_no].identifier = 'P';
            //     node->ST->entry[entry_no].width = 2;
            //     node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            //     get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
            //     int id_entry_no = contains_st(node->child->property.lexeme, node->ST);
                
                
            //     int l = node->ST->entry[id_entry_no].dt.at.l;
            //     //temp1 = index_temp - l
            //     char * temp1 = temp_label();
            //     int temp1_entry_no = insert_st(temp1, node->ST);
            //     node->ST->entry[temp1_entry_no].offset = get_FT_entry(node->ST)->offset;
            //     get_FT_entry(node->ST)->offset = node->ST->entry[temp1_entry_no].offset+8;
            //     char * arg_l = (char *)malloc(sizeof(char)*MAX_LENGTH);
            //     snprintf(arg_l, MAX_LENGTH, "%d", l);
            //     write_quad(MINUS, node->child->rightsib->child->nt_label, arg_l, temp1, node->ST);

            //     //temp1 = temp1*8;
            //     write_quad(MUL, temp1, "8", temp1, node->ST);

            //     //temp2 = base+8;
            //     int base_offset = node->ST->entry[id_entry_no].offset+8;
            //     char * temp2 = temp_label();
            //     int temp2_entry_no = insert_st(temp2, node->ST);
            //     node->ST->entry[temp2_entry_no].offset = get_FT_entry(node->ST)->offset;
            //     get_FT_entry(node->ST)->offset = node->ST->entry[temp2_entry_no].offset+8;
            //     char * arg_base_offset = (char *)malloc(sizeof(char)*MAX_LENGTH);
            //     // printf("base offset: %d\n", base_offset);
            //     snprintf(arg_base_offset, MAX_LENGTH, "%d",base_offset);
            //     write_quad(ASSIGNOP,arg_base_offset, NULL, temp2, node->ST);

            //     // res = temp2 + temp1;
            //     write_quad(PLUS, temp2, temp1, res, node->ST);
            // }
            */
           gen_ir(node->child->rightsib->child);
           char * res = temp_label();
           int entry_no = insert_st(res, node->ST);
            if(entry_no == -1)
            {

            }
            else
            {
                node->ST->entry[entry_no].deleted = 0;
                node->ST->entry[entry_no].dt.pt.p_type = 'I';
                node->ST->entry[entry_no].identifier = 'P';
                node->ST->entry[entry_no].width = 2;
                node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
                get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
                node->nt_label = str_copy(res);
                // write_quad(ASSIGNOP, node->child->property.lexeme, NULL, node->nt_label, node->ST);
                // res:=a[temp]
           write_quad(ARRASSIGNOP, node->child->property.lexeme, node->child->rightsib->child->nt_label, node->nt_label, node->ST);
            }
        }
    }
    else if(string_cmp(node->key, "NUM"))
    {
        char * val = (char *)malloc(sizeof(char) * MAX_LENGTH);
        snprintf(val, MAX_LENGTH, "%d", node->property.value_int);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        node->nt_label = str_copy(res);
        write_quad(ASSIGNOP, val, NULL, node->nt_label, node->ST);
        printf("IN NUM: %d\n", node->property.value_int);
    }
    else if(string_cmp(node->key, "ID"))
    {
        char * val = (char *)malloc(sizeof(char) * MAX_LENGTH);
        snprintf(val, MAX_LENGTH, "%s", node->property.lexeme);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        node->nt_label = str_copy(res);
        write_quad(ASSIGNOP, val, NULL, node->nt_label, node->ST);
    }
    else if(string_cmp(node->key, "TRUE"))
    {
        char * val = (char *)malloc(sizeof(char) * MAX_LENGTH);
        snprintf(val, MAX_LENGTH, "1");
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        node->nt_label = str_copy(res);
        write_quad(ASSIGNOP, val, NULL, node->nt_label, node->ST);
    }
    else if(string_cmp(node->key, "FALSE"))
    {
        char * val = (char *)malloc(sizeof(char) * MAX_LENGTH);
        snprintf(val, MAX_LENGTH, "0");
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        node->nt_label = str_copy(res);
        write_quad(ASSIGNOP, val, NULL, node->nt_label, node->ST);
    }
    else if(string_cmp(node->key, "RNUM"))
    {
        char * val = (char *)malloc(sizeof(char) * MAX_LENGTH);
        snprintf(val, MAX_LENGTH, "%f", node->property.value_double);
        char * res = temp_label();
        int entry_no = insert_st(res, node->ST);
        if(entry_no == -1)
        {

        }
        else
        {
            node->ST->entry[entry_no].deleted = 0;
            node->ST->entry[entry_no].dt.pt.p_type = 'I';
            node->ST->entry[entry_no].identifier = 'P';
            node->ST->entry[entry_no].width = 2;
            node->ST->entry[entry_no].offset = get_FT_entry(node->ST)->offset;
            get_FT_entry(node->ST)->offset = node->ST->entry[entry_no].offset+8;
        }
        node->nt_label = str_copy(res);
        write_quad(ASSIGNOP, val, NULL, node->nt_label, node->ST);
    }
    else if(string_cmp(node->label, "FOR"))
    {
        printf("!!!!In FOR\n");
        // Get L
        struct TreeNode * range = node->child->rightsib;
        int l,r;
        char * iterator = temp_label();
        // printf("%s\n", range->child->key);
        if(string_cmp(range->child->child->key, "NUM"))
        {
            l = range->child->child->property.value_int;
            // printf("%d\n",l);
        }
        else if(string_cmp(range->child->child->key, "PLUS"))
        {
            l = range->child->child->rightsib->property.value_int;
        }
        else
        {
            l = (range->child->child->rightsib->property.value_int)*(-1);
        }


        if(string_cmp(range->child->rightsib->child->key, "NUM"))
        {
            r = range->child->rightsib->child->property.value_int;
        }
        else if(string_cmp(range->child->rightsib->child->key, "PLUS"))
        {
            r = range->child->rightsib->child->property.value_int;
        }
        else
        {
            r = (range->child->rightsib->child->property.value_int)*(-1);
        }
        printf("%d %d\n", l, r);
        for(int i = l; i <= r; i++)
        {
            struct TreeNode * temp = node->child->rightsib->rightsib->child;
            while(temp)
            {
                gen_ir(temp);
                temp = temp->rightsib;
            }
        }
    }
    else
    {
        // gen_ir(node->child->rightsib);
        // write_quad(ASSIGNOP, node->child->rightsib->nt_label, NULL, node->child->property.lexeme, node->ST);
    }
    return qa;
}