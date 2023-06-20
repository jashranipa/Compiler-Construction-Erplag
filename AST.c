/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#include<stdio.h>
#include"AST.h"
int string_compare(char * s1, char * s2)
{
    int i = 0;
    while(s1[i] != '\0' && s2[i] != '\0')
    {
        if(s1[i]!=s2[i])return 0;
        i++;
    }
    if(s1[i] != '\0' && s2[i] != '\0')return 0;
    return 1;
}

struct TreeNode* make_node(char* label, struct TreeNode** arr, int n) 
{
    // make the root w/ label
    struct TreeNode* root = (struct TreeNode*) malloc(sizeof(struct TreeNode));
    root->label = str_copy(label);
    root->rightsib = NULL;
    root->parent = NULL;
    // attach child (if n != 0)
    // attach rightsib also
    if(n==1)
    {
        root->child = arr[0];
        struct TreeNode *child = root->child;
        while(child)
        {
            child->parent = root;
            child = child->rightsib;
        }
    }
    else if (n != 0)
    {
        struct TreeNode* temp = arr[0];
        root->child = temp;
        temp->parent = root;
        for (int j = 1; j < n; j++) 
        {
            if(!arr[j])
            {
                printf("Error in making node, Label: %s, index: %d\n", label, j);
            }
            temp->rightsib = arr[j];
            temp = temp->rightsib;
            temp->parent = root;
        }
        temp->rightsib = NULL;
    }
    else
    {
        root->child = NULL;
    }

    // return root
    return root;
}

void get_AST(struct TreeNode * node)
{
    int rule = node->rule;
    rule++;
    // printf("Rule : %d\n", rule);
    switch(rule)
    {
        case 1: // <program'> <program> $
        {
            get_AST(node->child);
            node->syn_addr = node->child->syn_addr;
            // // free(node->child->rightsib);
            // // free(node->child);
            // node->child = NULL;
        }
        break;
        case 2: // <program> <moduleDeclarations> <otherModules> <driverModule> <otherModules>
        {
            struct TreeNode * temp = node->child;
            struct TreeNode ** arr = (struct TreeNode**)malloc(4*sizeof(struct TreeNode *));
            int i = 0;
            char * label[] = {"MOD_DECS", "OTHER_1", "DRIVER", "OTHER_2"};
            while(temp)
            {
                // printf("Node : %s\n", temp->key);
                get_AST(temp);
                if (temp->syn_addr == NULL)
                {
                    arr[i] = make_node(label[i], NULL, 0);
                }
                else 
                {

                    arr[i] = make_node(label[i], &temp->syn_addr, 1);
                    if(i==2)
                    {
                        arr[i]->scope_start = temp->syn_addr->scope_start;
                        arr[i]->scope_end = temp->syn_addr->scope_end;
                    }
                }
                i++;
                temp = temp->rightsib;
            }
            node->syn_addr = make_node("PROGRAM", arr, 4);
            node->syn_addr->parent = NULL;
            temp = node->child;
            // free(temp->rightsib->rightsib->rightsib);
            // free(temp->rightsib->rightsib);
            // free(temp->rightsib);
            // free(temp);
            node->child = NULL;
        }
        break;
        case 3: // <moduleDeclarations> <moduleDeclaration> <moduleDeclarations>
        {
            get_AST(node->child->rightsib);
            get_AST(node->child);
            struct TreeNode * modDec = node->child;
            struct TreeNode * modDecs1 = node->child->rightsib;
            modDec->syn_addr->rightsib = modDecs1->syn_addr;
            node->syn_addr = modDec->syn_addr;
            node->child = NULL;
            // free(modDec);
            // free(modDecs1);
        }
        break;
        case 4: // <moduleDeclarations> epsilon
        {
            node->syn_addr = NULL;
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 5: // <moduleDeclaration> DECLARE MODULE ID SEMICOL
        {
            struct TreeNode * temp = node->child;
            node->syn_addr = temp->rightsib->rightsib;
            // free(temp->rightsib->rightsib->rightsib);
            // free(temp->rightsib);
            // free(temp);
            node->child = NULL;
        }
        break;
        case 6: // <otherModules> <module> <otherModules>
        {
            struct TreeNode * child = node->child;
            get_AST(child);
            get_AST(child->rightsib);
            child->syn_addr->rightsib = child->rightsib->syn_addr;
            node->syn_addr = child->syn_addr;
            // free(child->rightsib);
            // free(child);
            node->child = NULL;
        }
        break;
        case 7: // <otherModules> epsilon
        {
            node->syn_addr = NULL;
            // free(node->child);
            node->child = NULL;            
        }
        break;
        case 8: // <driverModule> DRIVERDEF DRIVER PROGRAM DRIVERENDDEF <moduleDef>
        {
            struct TreeNode * temp = node->child;
            while(temp->rightsib)
            {
                temp = temp->rightsib;
            }
            get_AST(temp);
            node->syn_addr=temp->syn_addr;
            node->syn_addr->scope_start = node->child->line_num;
            // free(temp);
            temp = node->child;
            // free(temp->rightsib->rightsib->rightsib);
            // free(temp->rightsib->rightsib);
            // free(temp->rightsib);
            // free(temp);
            node->child = NULL;
        }
        break;
        case 9: // <module> DEF MODULE ID ENDDEF TAKES INPUT SQBO <input_plist> SQBC SEMICOL <ret> <moduleDef>
        {
            struct TreeNode ** children = (struct TreeNode **)malloc(12*sizeof(struct TreeNode *));
            struct TreeNode ** arr = (struct TreeNode **)malloc(4*sizeof(struct TreeNode *));
            int i = 0;
            struct TreeNode * temp = node->child;
            while(temp)
            {
                children[i++] = temp;
                temp = temp->rightsib;
            }
            get_AST(children[7]);
            get_AST(children[10]);
            get_AST(children[11]);
            arr[0] = children[2];
            arr[1] = children[7]->syn_addr;
            arr[2] = children[10]->syn_addr;
            arr[3] = children[11]->syn_addr;
            node->syn_addr = make_node("MODULE", arr, 4);
            node->syn_addr->scope_start = children[0]->line_num;
            node->syn_addr->scope_end = children[11]->syn_addr->scope_end;
            for(int i = 0; i < 12; i++)
            {
                if(i==2)continue;
                // free(children[i]);
            }
            node->child = NULL;
        }
        break;
        case 10: // <ret> RETURNS SQBO <output_plist> SQBC SEMICOL
        {
            struct TreeNode ** children = (struct TreeNode **) malloc(5*sizeof(struct TreeNode*));
            struct TreeNode * temp = node->child;
            int i = 0;
            while(temp)
            {
                children[i++] = temp;
                temp = temp->rightsib;
            }
            get_AST(children[2]);
            node->syn_addr = make_node("RET", &children[2]->syn_addr, 1);
            for(int i = 0; i < 5; i++)
            {
                // free(children[i]);
            }
            node->child = NULL;
        }
        break;
        case 11:
        {
            // <ret> epsilon
            node->syn_addr = make_node("RET", NULL, 0);
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 12:
        {
            // <input_plist> ID COLON <dataType> <input_plist_temp>
            struct TreeNode* dataType = node->child->rightsib->rightsib;
            struct TreeNode * colon = node->child->rightsib;
            struct TreeNode* iptemp = dataType->rightsib;
            get_AST(dataType);
            struct TreeNode* arr[] = {node->child, dataType->syn_addr};
            struct TreeNode* iParam = make_node("IPARAM", arr, 2);
            get_AST(iptemp);
            iParam->rightsib = iptemp->syn_addr;
            node->syn_addr = make_node("INPUT", &iParam, 1);
            // free(iptemp);
            // free(dataType);
            // free(colon);
        }
        break;
        case 13:
        {
            // <input_plist_temp> COMMA ID COLON <dataType> <input_plist_temp>
            struct TreeNode* dataType = node->child->rightsib->rightsib->rightsib;
            struct TreeNode * colon = node->child->rightsib->rightsib; 
            struct TreeNode* iptemp1 = dataType->rightsib;
            get_AST(dataType);
            struct TreeNode* arr[] = {node->child->rightsib, dataType->syn_addr};
            struct TreeNode* iParam = make_node("IPARAM", arr, 2);
            get_AST(iptemp1);
            iParam->rightsib = iptemp1->syn_addr;
            node->syn_addr = iParam;
            // free(iptemp1);
            // free(dataType);
            // free(colon);
            // free(node->child);
        }
        break;
        case 14:
        {
            // <input_plist_temp> epsilon
            node->syn_addr = NULL;
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 15:
        {
            // <output_plist> ID COLON <type> <output_plist_temp>
            struct TreeNode* type = node->child->rightsib->rightsib;
            struct TreeNode* colon = node->child->rightsib;
            struct TreeNode* optemp1 = type->rightsib;
            get_AST(type);
            struct TreeNode* arr[] = {node->child, type->syn_addr};
            struct TreeNode* oParam = make_node("OPARAM", arr, 2);
            get_AST(optemp1);
            oParam->rightsib = optemp1->syn_addr;
            node->syn_addr = oParam;
            // free(optemp1);
            // free(type);
            // free(colon);
        }
        break;
        case 16:
        {
            // <output_plist_temp> COMMA ID COLON <type> <output_plist_temp>
            struct TreeNode* type = node->child->rightsib->rightsib->rightsib;
            struct TreeNode* colon = node->child->rightsib->rightsib;
            struct TreeNode* optemp1 = type->rightsib;
            get_AST(type);
            struct TreeNode* arr[] = {node->child->rightsib, type->syn_addr};
            struct TreeNode* oParam = make_node("OPARAM", arr, 2);
            get_AST(optemp1);
            oParam->rightsib = optemp1->syn_addr;
            node->syn_addr = oParam;
            // free(optemp1);
            // free(type);
            // free(colon);
            // free(node->child);
        }
        break;
        case 17:
        {
            // <output_plist_temp> epsilon
            node->syn_addr = NULL;
            // free(node->child);
        }
        break;
        case 18:
        {
            // <dataType> INTEGER
            node->syn_addr = node->child;      
        }
        break;
        case 19:
        {
            // <dataType> REAL
            node->syn_addr = node->child;
        }
        break;
        case 20:
        {
            // <dataType> BOOLEAN
            node->syn_addr = node->child;
        }
        break;
        case 21: // <dataType> ARRAY SQBO <range_dtype> SQBC OF <type>
        {
            struct TreeNode ** children = (struct TreeNode **) malloc(6*sizeof(struct TreeNode*));
            struct TreeNode * temp = node->child;
            int i = 0;
            while(temp)
            {
                children[i++] = temp;
                temp = temp->rightsib;
            } 
            get_AST(children[2]);
            get_AST(children[5]);
            struct TreeNode ** arr = (struct TreeNode **) malloc(3*sizeof(struct TreeNode*));
            arr[0] = children[2]->syn_addr;
            arr[1] = children[2]->syn_addr->rightsib;
            arr[2] = children[5]->syn_addr;
            node->syn_addr = make_node("ARRAY", arr, 3);
            for(int i = 0; i < 6; i++)
            {
                // free(children[i]);
            }
            node->child = NULL;
        }
        break;
        case 22: // <type> INTEGER
        {
            node->syn_addr = node->child;
            node->child = NULL;
        }
        break;
        case 23: // <type> REAL
        {
            node->syn_addr = node->child;
            node->child = NULL;
        }
        break;
        case 24: // <type> BOOLEAN
        {
            node->syn_addr = node->child;
            node->child = NULL;
        }
        break;
        case 25: // <moduleDef> START <statements> END
        {
            struct TreeNode * temp = node->child, *stmts;
            get_AST(temp->rightsib);
            if(temp->rightsib->syn_addr)
            {
                stmts = make_node("STATEMENTS", &(temp->rightsib->syn_addr), 1);
            }
            else stmts = make_node("STATEMENTS", NULL, 0);
            stmts->scope_end = temp->rightsib->rightsib->line_num;
            node->syn_addr=stmts;
            // free(temp->rightsib->rightsib);
            // free(temp->rightsib);
            // free(temp);
            node->child = NULL;
        }
        break;
        case 26: // <range_dtype> <range_bound> RANGEOP <range_bound>
        {
            struct TreeNode * temp = node->child;
            get_AST(temp);
            get_AST(temp->rightsib->rightsib);
            struct TreeNode * temp_L = make_node("L", &(temp->syn_addr), 1);
            struct TreeNode * temp_R = make_node("R", &(temp->rightsib->rightsib->syn_addr), 1);
            temp_L->rightsib = temp_R;
            node->syn_addr = temp_L;
            // free(temp->rightsib->rightsib);
            // free(temp->rightsib);
            // free(temp);
            node->child = NULL;
        }
        break;
        case 27: // <range_bound> MINUS <which_range_bound>
        {
            struct TreeNode * temp = node->child->rightsib;
            get_AST(temp);
            node->child->rightsib = temp->syn_addr;
            node->syn_addr = node->child;
            // free(temp);
            node->child = NULL;
        }
        break;
        case 28: // <range_bound> <which_range_bound>
        {
            get_AST(node->child);
            node->syn_addr = node->child->syn_addr;
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 29:
        {
            // <which_range_bound> ID
            node->syn_addr = node->child;
        }
        break;
        case 30:
        {
            // <which_range_bound> NUM
            node->syn_addr = node->child;
        }
        break;
        case 31:
        {
            // <statements> <statement> <statements>
            struct TreeNode* stmt = node->child;
            struct TreeNode* stmts1 = stmt->rightsib;
            get_AST(stmt);
            get_AST(stmts1);
            // printf("%s\t", stmt->syn_addr->label);
            stmt->syn_addr->rightsib = stmts1->syn_addr;
            if(stmts1->rightsib)
            {
                printf("%s\n", stmts1->rightsib->label);
            }
            else printf("\n");
            node->syn_addr = stmt->syn_addr;
            // free(stmts1);
            // free(stmt);
            node->child = NULL;
        }
        break;
        case 32:
        {
            // <statements> epsilon
            node->syn_addr = NULL;
            // // free(node->child);
        }
        break;
        case 33:
        {
            // <statement> <ioStmt>
            get_AST(node->child);
            node->syn_addr = node->child->syn_addr;
            // free(node->child);
        }
        break;
        case 34:
        {
            // <statement> <simpleStmt>
            get_AST(node->child);
            node->syn_addr = node->child->syn_addr;
            // free(node->child);
        }
        break;
        case 35:
        {
            // <statement> <declareStmt>
            get_AST(node->child);
            node->syn_addr = node->child->syn_addr;
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 36:
        {
            // <statement> <condionalStmt>
            get_AST(node->child);
            node->syn_addr = node->child->syn_addr;
            // free(node->child);
        }
        break;
        case 37:
        {
            // <statement> <iterativeStmt>
            get_AST(node->child);
            node->syn_addr = node->child->syn_addr;
            // free(node->child);
        }
        break;
        case 38:
        {
            // <ioStmt> GET_VALUE BO ID BC SEMICOL
            struct TreeNode ** children = (struct TreeNode **)malloc(5*sizeof(struct TreeNode *));
            int i = 0;
            struct TreeNode * temp = node->child;
            while(temp)
            {
                children[i++] = temp;
                temp = temp->rightsib;
            }
            children[2]->rightsib = NULL;
            node->syn_addr = make_node("GET_VALUE", &children[2], 1);
            // free(children[4]);
            // free(children[3]);
            // free(children[1]);
            // free(children[0]);
        }
        break;
        case 39:
        {
            // <ioStmt> PRINT BO <ioStmt_output> BC SEMICOL
            struct TreeNode ** children = (struct TreeNode **)malloc(5*sizeof(struct TreeNode *));
            int i = 0;
            struct TreeNode * temp = node->child;
            while(temp)
            {
                children[i++] = temp;
                temp = temp->rightsib;
            }
            get_AST(children[2]);
            node->syn_addr = make_node("PRINT", &children[2]->syn_addr, 1);
            // free(children[4]);
            // free(children[3]);
            // free(children[2]);
            // free(children[1]);
            // free(children[0]);
        }
        break;
        case 40:
        {
            // <ioStmt_output> ID <whichID>
            struct TreeNode* whichID = node->child->rightsib;
            get_AST(whichID);
            node->child->rightsib = whichID->syn_addr;
            node->syn_addr = node->child;
            // free(whichID);
        }
        break;
        case 41: //<whichID> SQBO <arr_ioStmt_output> SQBC
        {
            struct TreeNode * temp = node->child;
            get_AST(temp->rightsib);
            struct TreeNode * temp2 = make_node("ARRAY", &(temp->rightsib->syn_addr), 1);
            node->syn_addr = temp2;
            // free(temp->rightsib->rightsib);
            // free(temp->rightsib);
            // free(temp);
            node->child = NULL;
        }
        break;
        case 42: //<whichID> epsilon
        {
            node->syn_addr = NULL;
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 43: // <arr_ioStmt_output> ID
        {
            node->syn_addr = node->child;
        }
        break;
        case 44: // <arr_ioStmt_output> NUM
        {
            node->syn_addr = node->child;
        }
        break;
        case 45: // <arr_ioStmt_output> MINUS NUM
        {
            node->syn_addr = node->child;
        }
        break;
        case 46: // <ioStmt_output> TRUE
        {
            node->syn_addr = node->child;
        }
        break;
        case 47: // <ioStmt_output> FALSE
        {
            node->syn_addr = node->child;
        }
        break;
        case 48: // <ioStmt_output> MINUS <which_ioStmt_output>
        {
            struct TreeNode * temp = node->child->rightsib;
            get_AST(temp);
            node->child->rightsib = temp->syn_addr;
            node->syn_addr = node->child;
            // free(temp);
            node->child = NULL;
        }
        break;
        case 49: // <ioStmt_output> <which_ioStmt_output>
        {
            get_AST(node->child);
            node->syn_addr = node->child->syn_addr;
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 50: // <which_ioStmt_output> NUM
        {
            node->syn_addr = node->child;
            node->child = NULL;
        }
        break;
        case 51:
        {
            // <which_ioStmt_output> RNUM
            node->syn_addr = node->child;
        }
        break;
        case 52:
        {
            // <simpleStmt> <assignmentStmt>
            get_AST(node->child);
            node->syn_addr = node->child->syn_addr;
            // free(node->child);
        }
        break;
        case 53:
        {
            // <simpleStmt> <moduleReuseStmt>
            get_AST(node->child);
            node->syn_addr = node->child->syn_addr;
            // free(node->child);
        }
        break;
        case 54:
        {
            // <assignmentStmt> ID <whichStmt>
            struct TreeNode* whichStmt = node->child->rightsib;
            whichStmt->inh_addr = node->child;
            get_AST(whichStmt);
            node->syn_addr = whichStmt->syn_addr;
            // free(whichStmt);
        }
        break;
        case 55:
        {
            // <whichStmt> <lvalueIDStmt>
            struct TreeNode* lvalue = node->child;
            lvalue->inh_addr = node->inh_addr;
            get_AST(lvalue);
            node->syn_addr = lvalue->syn_addr;
            // free(lvalue);
        }
        break;
        case 56:
        {
            // <whichStmt> <lvalueARRStmt>
            struct TreeNode* lvalue = node->child;
            lvalue->inh_addr = node->inh_addr;
            get_AST(lvalue);
            node->syn_addr = lvalue->syn_addr;
            // free(lvalue);
        }
        break;
        case 57:
        {
            // <lvalueIDStmt> ASSIGNOP <toptier_expression> SEMICOL
            struct TreeNode* toptier = node->child->rightsib;
            get_AST(toptier);
            struct TreeNode* arr[] = {node->inh_addr, toptier->syn_addr};
            node->syn_addr = make_node("ASID", arr, 2);
            node->syn_addr->line_num = node->child->line_num;
            // free(toptier->rightsib);
            // free(toptier);
            // free(node->child);
        }
        break;
        case 58:
        {
            // <lvalueARRStmt> SQBO <index_expression> SQBC ASSIGNOP <toptier_expression> SEMICOL
            struct TreeNode* children[6];
            struct TreeNode* temp = node->child;
            int i = 0;
            while (temp) {
                children[i] = temp;
                temp = temp->rightsib;
                i++;
            }
            get_AST(children[1]);
            get_AST(children[4]);
            struct TreeNode* arr[] = {node->inh_addr, children[1]->syn_addr, children[4]->syn_addr};
            node->syn_addr = make_node("ASARR", arr, 3);
            node->syn_addr->line_num = children[3]->line_num;
            // for (int j = 0; j < 6; j++) 
                // free(children[j]);
        }
        break;
        case 59:
        {
            // <index_expression> <index_expression_term> <index_expression_temp>
            struct TreeNode * temp = node->child;
            get_AST(temp);
            temp->rightsib->inh_addr = temp->syn_addr;
            get_AST(temp->rightsib);
            node->syn_addr = temp->rightsib->syn_addr;
            // free(temp->rightsib);
            // free(temp);
            node->child = NULL;
            // if(!node->syn_addr)
            // {
            //     printf("NULL index expression\n");
            // }
        }
        break;
        case 60:
        {
            // <index_expression_temp> <op1> <index_expression_term> <index_expression_temp>
            struct TreeNode * child = node->child;
            get_AST(child);
            get_AST(child->rightsib);
            struct TreeNode * arr[] = {node->inh_addr, child->rightsib->syn_addr};
            struct TreeNode * temp = make_node(child->syn_addr->key, arr, 2);
            // if(!node->inh_addr)
            // {
            //     printf("Danger\n");
            // }
            child->rightsib->rightsib->inh_addr = temp;
            get_AST(child->rightsib->rightsib);
            node->syn_addr = child->rightsib->rightsib->syn_addr;
            // free(child->rightsib->rightsib);
            // free(child->rightsib);
            // free(child);
            node->child = NULL;
        }
        break;
        case 61:
        {
            // <index_expression_temp> epsilon
            node->syn_addr = node->inh_addr;
            // if(!node->syn_addr)
            // {
            //     printf("NULL index_expression_temp\n");
            // }
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 62:
        {
            // <index_expression_term> <index_expression_factor> <index_expression_term_temp>
            struct TreeNode * temp = node->child;
            get_AST(temp);
            temp->rightsib->inh_addr = temp->syn_addr;
            get_AST(temp->rightsib);
            node->syn_addr = temp->rightsib->syn_addr;
            // free(temp->rightsib);
            // free(temp);
            node->child = NULL;
        }
        break;
        case 63:
        {
            // <index_expression_term_temp> <op2> <index_expression_factor> <index_expression_term_temp>
            struct TreeNode * child = node->child;
            get_AST(child);
            get_AST(child->rightsib);
            struct TreeNode * arr[] = {node->inh_addr, child->rightsib->syn_addr};
            struct TreeNode * temp = make_node(child->syn_addr->key, arr, 2);
            child->rightsib->rightsib->inh_addr = temp;
            get_AST(child->rightsib->rightsib);
            node->syn_addr = child->rightsib->rightsib->syn_addr;
            // free(child->rightsib->rightsib);
            // free(child->rightsib);
            // free(child);
            node->child = NULL;
        }
        break;
        case 64:
        {
            // <index_expression_term_temp> epsilon
            node->syn_addr = node->inh_addr;
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 65:
        {
            // <index_expression_factor> <op1> <index_factor>
            struct TreeNode * temp, *child = node->child;
            get_AST(child);
            get_AST(child->rightsib);
            if(string_compare(child->syn_addr->key,"PLUS"))
            {
                temp = make_node("UNARY_PLUS", &(child->rightsib->syn_addr),1);
            }
            else
            {
                temp = make_node("UNARY_MINUS", &(child->rightsib->syn_addr),1);
            }
            node->syn_addr = temp;
            // free(child->rightsib);
            // free(child->syn_addr);
            // free(child);
            node->child = NULL;
        }
        break;
        case 66:
        {
            // <index_expression_factor> <index_factor>
            // get_AST(node->child);
            // node->syn_addr = node->child->syn_addr;
            // // free(node->child);
            // node->child = NULL;
            get_AST(node->child);
            node->syn_addr = node->child->syn_addr;
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 67:
        {
            // <index_factor> BO <index_expression> BC
            // struct TreeNode* index_expr = node->child->rightsib;
            // get_AST(index_expr);
            // if (node->inh_addr == NULL) 
            // {
            //     node->syn_addr = index_expr->syn_addr;
            // }
            // else 
            // {
            //     struct TreeNode* arr[] = {node->inh_addr, index_expr->syn_addr};
            //     node->syn_addr = make_node("L", arr, 2);
            // }
            // // free(node->child);
            // // free(index_expr->rightsib);
            // // free(index_expr);
            get_AST(node->child->rightsib);
            node->syn_addr = node->child->rightsib->syn_addr;
            // free(node->child->rightsib->rightsib);
            // free(node->child->rightsib);
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 68:
        {
            // <index_factor> ID
            // struct TreeNode* id = node->child;
            // if (node->inh_addr == NULL)
            // {
            //     node->syn_addr = id;
            // }
            // else 
            // {
            //     struct TreeNode* arr[] = {node->inh_addr, id};
            //     node->syn_addr = make_node("L", arr, 2);
            // }
            node->syn_addr = node->child;
            node->child = NULL;
        }
        break;
        case 69:
        {
            // <index_factor> NUM
            // struct TreeNode* num = node->child;
            // if (node->inh_addr == NULL)
            // {
            //     node->syn_addr = num;
            // }
            // else 
            // {
            //     struct TreeNode* arr[] = {node->inh_addr, num};
            //     node->syn_addr = make_node("L", arr, 2);
            // }
            node->syn_addr = node->child;
            node->child = NULL;
        }
        break;
        case 70:
        {
            // <moduleReuseStmt> <optional> USE MODULE ID WITH PARAMETERS <idList> SEMICOL
            struct TreeNode* children[8];
            struct TreeNode* temp = node->child;
            int i = 0;
            while (temp) {
                children[i] = temp;
                temp = temp->rightsib;
                i++;
            }
            get_AST(children[6]);
            get_AST(children[0]);
            struct TreeNode* arr[] = {children[0]->syn_addr, children[3], children[6]->syn_addr};
            node->syn_addr = make_node("MRS", arr, 3);
            node->syn_addr->line_num = children[1]->line_num;
            for (int j = 0; j < 8; j++) {
                // if (j != 3)
                    // free(children[j]);
            }
        }
        break;
        case 71:
        {
            // <optional> SQBO <idList> SQBC ASSIGNOP
            struct TreeNode* children[4];
            struct TreeNode* temp = node->child;
            int i = 0;
            while (temp) {
                children[i] = temp;
                temp = temp->rightsib;
                i++;
            }
            get_AST(children[1]);
            node->syn_addr = children[1]->syn_addr;
            // for (int j = 0; j < 4; j++)
                // free(children[j]);
        }
        break;
        case 72:
        {
            // <optional> epsilon
            node->syn_addr = make_node("IDLIST", NULL, 0);
            // free(node->child);
        }
        break;
        case 73:
        {
            // <idList> ID <idList_temp>
            struct TreeNode* id = node->child;
            struct TreeNode* idlist_temp = id->rightsib;
            get_AST(idlist_temp);
            id->rightsib = idlist_temp->syn_addr;
            node->syn_addr = make_node("IDLIST", &id, 1);
            // free(idlist_temp);
        }   
        break;
        case 74:
        {
            // <idList_temp> COMMA ID <idList_temp>
            struct TreeNode* id = node->child->rightsib;
            struct TreeNode* idList_temp1 = id->rightsib;
            get_AST(idList_temp1);
            id->rightsib = idList_temp1->syn_addr;
            node->syn_addr = id;
            // free(node->child);
            // free(idList_temp1);
        }
        break;
        case 75:
        {
            // <idList_temp> epsilon
            node->syn_addr = NULL;
            // free(node->child);
        }
        break;
        case 76: // <toptier_expression> <toptier_expression_term> <toptier_expression_temp>
        {
            struct TreeNode * temp = node->child;
            get_AST(temp);
            // if(!temp->syn_addr)
            // {
            //     printf("NULL toptier term\n");
            // }
            temp->rightsib->inh_addr = temp->syn_addr;
            get_AST(temp->rightsib);
            // if(!temp->rightsib->syn_addr)
            // {
            //     printf("NULL toptier\n");
            // }
            node->syn_addr = temp->rightsib->syn_addr;
            // free(temp->rightsib);
            // free(temp);
            node->child = NULL;
        }
        break;
        case 77: // <toptier_expression_temp> <logicalOp> <toptier_expression_term> <toptier_expression_temp>
        {
            struct TreeNode * temp = node->child;
            get_AST(temp);
            get_AST(temp->rightsib);
            struct TreeNode ** arr = (struct TreeNode **) malloc(2*sizeof(struct TreeNode *));
            arr[0] = node->inh_addr;
            arr[1] = temp->rightsib->syn_addr;
            struct TreeNode * new_node = make_node(temp->syn_addr->key, arr, 2);
            temp->rightsib->rightsib->inh_addr = new_node;
            get_AST(temp->rightsib->rightsib);
            node->syn_addr = temp->rightsib->rightsib->syn_addr;
            // free(temp->rightsib->rightsib);
            // free(temp->rightsib);
            // // free the syn address of <logicalOp> - Needs verification
            // free(temp->syn_addr);
            // free(temp);
            node->child = NULL;
            // free(arr[0]);
            // free(arr[1]);
            // free(arr);
        }
        break;
        case 78: // <toptier_expression_temp> epsilon
        {
            node->syn_addr = node->inh_addr;
            // if(!node->syn_addr)
            // {
            //     printf("NULL toptier temp\n");
            // }
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 79: // <toptier_expression_term> <toptier_expression_factor> <toptier_expression_term_temp>
        {
            struct TreeNode * temp = node->child;
            get_AST(temp);
            // if(!temp->syn_addr)
            // {
            //     printf("NULL tte factor\n");
            // }
            temp->rightsib->inh_addr = temp->syn_addr;
            get_AST(temp->rightsib);
            node->syn_addr = temp->rightsib->syn_addr;
            // free(temp->rightsib);
            // free(temp);
            node->child = NULL;
        }
        break;
        case 80: // <toptier_expression_term> TRUE <toptier_expression_term_temp>
        {
            struct TreeNode * temp = node->child;
            temp->rightsib->inh_addr = temp;
            get_AST(temp->rightsib);
            node->syn_addr = temp->rightsib->syn_addr;
            // free(temp->rightsib);
        }
        break;
        case 81: // <toptier_expression_term> FALSE <toptier_expression_term_temp>
        {
            struct TreeNode * temp = node->child;
            temp->rightsib->inh_addr = temp;
            get_AST(temp->rightsib);
            node->syn_addr = temp->rightsib->syn_addr;
            // free(temp->rightsib);
        }
        break;
        case 82: // <toptier_expression_term_temp> <relationalOp> <toptier_expression_factor> <toptier_expression_term_temp>
        {
            struct TreeNode * temp = node->child;
            get_AST(temp);
            get_AST(temp->rightsib);
            struct TreeNode ** arr = (struct TreeNode **) malloc(2*sizeof(struct TreeNode *));
            arr[0] = node->inh_addr;
            arr[1] = temp->rightsib->syn_addr;
            struct TreeNode * new_node = make_node(temp->syn_addr->key, arr, 2);
            temp->rightsib->rightsib->inh_addr = new_node;
            get_AST(temp->rightsib->rightsib);
            node->syn_addr = temp->rightsib->rightsib->syn_addr;
            // free(temp->rightsib->rightsib);
            // free(temp->rightsib);
            // // free the syn address of <relationalOp> - Needs verification
            // free(temp->syn_addr);
            // free(temp);
            node->child = NULL;
            // free(arr[0]);
            // free(arr[1]);
            // free(arr);
        }
        break;
        case 83: // <toptier_expression_term_temp> epsilon
        {
            node->syn_addr = node->inh_addr;
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 84: // <toptier_expression_factor> <term> <toptier_expression_factor_temp>
        {
            struct TreeNode * temp = node->child;
            get_AST(temp);
            temp->rightsib->inh_addr = temp->syn_addr;
            get_AST(temp->rightsib);
            node->syn_addr = temp->rightsib->syn_addr;
            // free(temp->rightsib);
            // free(temp);
            node->child = NULL;
        }
        break;
        case 85: // <toptier_expression_factor_temp> <op1> <term> <toptier_expression_factor_temp>
        {
            struct TreeNode * temp = node->child;
            get_AST(temp);
            get_AST(temp->rightsib);
            struct TreeNode ** arr = (struct TreeNode **) malloc(2*sizeof(struct TreeNode *));
            arr[0] = node->inh_addr;
            arr[1] = temp->rightsib->syn_addr;
            struct TreeNode * new_node = make_node(temp->syn_addr->key, arr, 2);
            temp->rightsib->rightsib->inh_addr = new_node;
            get_AST(temp->rightsib->rightsib);
            node->syn_addr = temp->rightsib->rightsib->syn_addr;
            // free(temp->rightsib->rightsib);
            // free(temp->rightsib);
            // // free the syn address of <op1> - Needs verification
            // free(temp->syn_addr);
            // free(temp);
            node->child = NULL;
            // free(arr[0]);
            // free(arr[1]);
            // free(arr);
        }
        break;
        case 86: // <toptier_expression_factor_temp> epsilon
        {
            node->syn_addr = node->inh_addr;
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 87: // <term> <factor> <term_temp>
        {
            struct TreeNode * temp = node->child;
            get_AST(temp);
            temp->rightsib->inh_addr = temp->syn_addr;
            get_AST(temp->rightsib);
            node->syn_addr = temp->rightsib->syn_addr;
            // free(temp->rightsib);
            // free(temp);
            node->child = NULL;
        }
        break;
        case 88: // <term_temp> <op2> <factor> <term_temp>
        {
            struct TreeNode * temp = node->child;
            get_AST(temp);
            get_AST(temp->rightsib);
            struct TreeNode ** arr = (struct TreeNode **) malloc(2*sizeof(struct TreeNode *));
            arr[0] = node->inh_addr;
            arr[1] = temp->rightsib->syn_addr;
            struct TreeNode * new_node = make_node(temp->syn_addr->key, arr, 2);
            temp->rightsib->rightsib->inh_addr = new_node;
            get_AST(temp->rightsib->rightsib);
            node->syn_addr = temp->rightsib->rightsib->syn_addr;
            // free(temp->rightsib->rightsib);
            // free(temp->rightsib);
            // // free the syn address of <op1> - Needs verification
            // free(temp->syn_addr);
            // free(temp);
            node->child = NULL;
            // free(arr[0]);
            // free(arr[1]);
            // free(arr);
        }
        break;
        case 89: // <term_temp> epsilon
        {
            node->syn_addr = node->inh_addr;
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 90: // <factor> <op1> <factor'>
        {
            struct TreeNode * temp, *child = node->child;
            get_AST(child);
            get_AST(child->rightsib);
            if(string_compare(child->syn_addr->key,"PLUS"))
            {
                temp = make_node("UNARY_PLUS", &(child->rightsib->syn_addr),1);
            }
            else
            {
                temp = make_node("UNARY_MINUS", &(child->rightsib->syn_addr),1);
            }
            node->syn_addr = temp;
            // free(child->rightsib);
            // free(child->syn_addr);
            // free(child);
            node->child = NULL;
        }
        break;
        case 91: // <factor> <factor'>
        {
            get_AST(node->child);
            node->syn_addr = node->child->syn_addr;
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 92: // <factor'> BO <toptier_expression> BC
        {
            get_AST(node->child->rightsib);
            node->syn_addr = node->child->rightsib->syn_addr;
            // free(node->child->rightsib->rightsib);
            // free(node->child->rightsib);
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 93: // <factor'> NUM
        {
            node->syn_addr = node->child;
            node->child = NULL;
        }
        break;
        case 94: //<factor'> RNUM
        {
            node->syn_addr = node->child;
            node->child = NULL;
        }
        break;
        case 95: // <factor'> ID <which_factor>
        {
            struct TreeNode * temp = node->child;
            get_AST(temp->rightsib);
            struct TreeNode ** arr = (struct TreeNode **) malloc(2*sizeof(struct TreeNode*));
            arr[0] = temp;
            arr[1] = temp->rightsib->syn_addr;
            struct TreeNode * temp2 = temp->rightsib;
            temp->rightsib = NULL;
            if(temp2->syn_addr == NULL)
            {
                node->syn_addr = make_node("FACTOR", &(temp),1);
            }
            else
            {
                // printf("Inside case 95\n");
                node->syn_addr = make_node("FACTOR", arr, 2);
            }
            // free(temp2);
            // // free(temp);
            node->child = NULL;
        }
        break;
        case 96: // <which_factor> SQBO <index_expression> SQBC
        {
            struct TreeNode * temp = node->child;
            get_AST(temp->rightsib);
            // if(!temp->rightsib->syn_addr)
            // {
            //     printf("NULL index expression in case 96\n");
            // }
            node->syn_addr = make_node("ARRAY", &(temp->rightsib->syn_addr),1);
            // free(temp->rightsib->rightsib);
            // free(temp->rightsib);
            // free(temp);
            node->child = NULL;
        }
        break;
        case 97: // <which_factor> epsilon
        {
            node->syn_addr = NULL;
            // free(node->child);
            node->child = NULL;
        }
        break;
        case 98: // <op1> PLUS
        {
            node->syn_addr = node->child;
        }
        break;
        case 99: // <op1> MINUS
        {
            node->syn_addr = node->child;
        }
        break;
        case 100:
        {
            // <op2> MUL
            node->syn_addr = node->child;
        }
        break;
        case 101:
        {
            // <op2> DIV
            node->syn_addr = node->child;
        }
        break;
        case 102:
        {
            // <logicalOp> AND
            node->syn_addr = node->child;
        }
        break;
        case 103:
        {
            // <logicalOp> OR
            node->syn_addr = node->child;
        }
        break;
        case 104:
        {
            // <relationalOp> LT
            node->syn_addr = node->child;
        }
        break;
        case 105:
        {
            // <relationalOp> LE
            node->syn_addr = node->child;
        }
        break;
        case 106:
        {
            // <relationalOp> GT
            node->syn_addr = node->child;
        }
        break;
        case 107:
        {
            // <relationalOp> GE
            node->syn_addr = node->child;
        }
        break;
        case 108:
        {
            // <relationalOp> EQ
            node->syn_addr = node->child;
        }
        break;
        case 109:
        {
            // <relationalOp> NE
            node->syn_addr = node->child;
        }
        break;
        case 110:
        {
            // <declareStmt> DECLARE <idList> COLON <dataType> SEMICOL
            struct TreeNode* children[5];
            struct TreeNode* temp = node->child;
            int i = 0;
            while (temp) {
                children[i] = temp;
                temp = temp->rightsib;
                i++;
            }
            get_AST(children[3]);
            get_AST(children[1]);
            struct TreeNode* arr[] = {children[1]->syn_addr, children[3]->syn_addr};
            node->syn_addr = make_node("DEC", arr, 2);
            // for (int j = 0; j < 5; j++)
                // free(children[j]);
        }
        break;
        case 111:
        {
            // <condionalStmt> SWITCH BO ID BC START <caseStmt> <default> END
            struct TreeNode* children[8];
            struct TreeNode* temp = node->child;
            int i = 0;
            while (temp) {
                children[i] = temp;
                temp = temp->rightsib;
                i++;
            }
            get_AST(children[5]);
            struct TreeNode * caseStmts = make_node("CASE_STMTS", &(children[5]->syn_addr),1);
            get_AST(children[6]);
            struct TreeNode* arr[] = {children[2], caseStmts, children[6]->syn_addr};
            node->syn_addr = make_node("SWITCH", arr, 3);
            for (int j = 0; j < 8; j++) {
                // if (j != 2)
                    // free(children[j]);
            }
        }
        break;
        case 112:
        {
            // <caseStmt> CASE <value> COLON <statements> BREAK SEMICOL <caseStmt_many>
            struct TreeNode* children[7];
            struct TreeNode* temp = node->child;
            int i = 0;
            while (temp) {
                children[i] = temp;
                temp = temp->rightsib;
                i++;
            }
            get_AST(children[1]);
            get_AST(children[3]);
            get_AST(children[6]);
            struct TreeNode * stmts;
            if(children[3]->syn_addr)
            {
                stmts = make_node("STATEMENTS", &children[3]->syn_addr, 1);
            }
            else stmts = make_node("STATEMENTS", NULL, 0);
            struct TreeNode* arr[] = {children[1]->syn_addr, stmts};
            struct TreeNode* case_node = make_node("CASE", arr, 2);
            case_node->scope_start = children[0]->line_num;
            case_node->scope_end = children[5]->line_num;
            case_node->rightsib = children[6]->syn_addr;
            // if(case_node->rightsib)
            // {
            //     printf("Case 112: %s\n", case_node->rightsib->label);
            // }
            // else
            // {
            //     printf("Case 112: NULL\n");
            // }
            node->syn_addr = case_node;
            // for (int j = 0; j < 7; j++)
                // free(children[j]);
        }
        break;
        case 113:
        {
            // <caseStmt_many> CASE <value> COLON <statements> BREAK SEMICOL <caseStmt_many>
            struct TreeNode* children[7];
            struct TreeNode* temp = node->child;
            int i = 0;
            while (temp) {
                children[i] = temp;
                temp = temp->rightsib;
                i++;
            }
            get_AST(children[1]);
            get_AST(children[3]);
            get_AST(children[6]);
            struct TreeNode * stmts;
            if(children[3]->syn_addr)
            {
                stmts = make_node("STATEMENTS", &children[3]->syn_addr, 1);
            }
            else stmts = make_node("STATEMENTS", NULL, 0);
            struct TreeNode* arr[] = {children[1]->syn_addr, stmts};
            struct TreeNode* case_node = make_node("CASE", arr, 2);
            case_node->scope_start = children[0]->line_num;
            case_node->scope_end = children[5]->line_num;
            case_node->rightsib = children[6]->syn_addr;
            node->syn_addr = case_node;
            // for (int j = 0; j < 7; j++)
                // free(children[j]);
        }
        break;
        case 114:
        {
            // <caseStmt_many> epsilon
            node->syn_addr = NULL;
            // free(node->child);
        }
        break;
        case 115:
        {
            // <value> NUM
            node->syn_addr = node->child;
        }
        break;
        case 116:
        {
            // <value> MINUS NUM
            node->child->child = node->child->rightsib;
            node->child->child->parent = node->child;
            node->child->rightsib = NULL;
            node->syn_addr = node->child;
        }
        break;
        case 117:
        {
            // <value> TRUE
            node->syn_addr = node->child;
        }
        break;
        case 118:
        {
            // <value> FALSE
            node->syn_addr = node->child;
        }
        break;
        case 119:
        {
            // <default> DEFAULT COLON <statements> BREAK SEMICOL
            struct TreeNode* children[5];
            struct TreeNode* temp = node->child;
            int i = 0;
            while (temp) {
                children[i] = temp;
                temp = temp->rightsib;
                i++;
            }
            get_AST(children[2]);
            struct TreeNode * stmts;
            if(children[2]->syn_addr)
            {
                stmts = make_node("STATEMENTS", &children[2]->syn_addr, 1);
            }
            else stmts = make_node("STATEMENTS", NULL, 0);
            node->syn_addr = make_node("DEFAULT", &stmts, 1);
            node->syn_addr->scope_start = children[0]->line_num;
            node->syn_addr->scope_end = children[4]->line_num;
            // for (int j = 0; j < 5; j++) 
                // free(children[j]);
        }
        break;
        case 120:
        {
            // <iterativeStmt> FOR BO ID IN <range_for_loop> BC START <statements> END
            struct TreeNode* children[9];
            struct TreeNode* temp = node->child;
            int i = 0;
            while (temp) {
                children[i] = temp;
                temp = temp->rightsib;
                i++;
            }
            get_AST(children[4]);
            get_AST(children[7]);
            struct TreeNode * stmts;
            if(children[7]->syn_addr)
            {
                stmts = make_node("STATEMENTS", &children[7]->syn_addr, 1);
            }
            else stmts = make_node("STATEMENTS", NULL, 0);
            node->syn_addr = make_node("DEFAULT", &stmts, 1);
            struct TreeNode* arr[] = {children[2], children[4]->syn_addr, stmts};
            node->syn_addr = make_node("FOR", arr, 3);
            node->syn_addr->scope_start = children[0]->line_num;
            node->syn_addr->scope_end = children[8]->line_num;
            for (int j = 0; j < 9; j++) {
                // if (j != 2)
                    // free(children[j]);
            }
        }
        break;
        case 121:
        {
            // <iterativeStmt> WHILE BO <toptier_expression> BC START <statements> END
            struct TreeNode* children[7];
            struct TreeNode* temp = node->child;
            int i = 0;
            while (temp) {
                children[i] = temp;
                temp = temp->rightsib;
                i++;
            }
            get_AST(children[2]);
            get_AST(children[5]);
            struct TreeNode * stmts;
            if(children[5]->syn_addr)
            {
                stmts = make_node("STATEMENTS", &children[5]->syn_addr, 1);
            }
            else stmts = make_node("STATEMENTS", NULL, 0);
            node->syn_addr = make_node("DEFAULT", &stmts, 1);
            struct TreeNode* arr[] = {children[2]->syn_addr, stmts};
            node->syn_addr = make_node("WHILE", arr, 2);
            node->syn_addr->line_num = children[0]->line_num;
            node->syn_addr->scope_start = children[0]->line_num;
            node->syn_addr->scope_end = children[6]->line_num;
            // for (int j = 0; j < 7; j++)
                // free(children[j]);
        }
        break;
        case 122:
        {
            // <range_for_loop> <which_range_for_loop> RANGEOP <which_range_for_loop>
            struct TreeNode* wrfloop1 = node->child;
            struct TreeNode* wrfloop2 = wrfloop1->rightsib->rightsib;
            get_AST(wrfloop1);
            get_AST(wrfloop2);
            struct TreeNode* temp_l = make_node("L", &wrfloop1->syn_addr, 1);
            struct TreeNode* temp_r = make_node("R", &wrfloop2->syn_addr, 1);
            struct TreeNode * arr[] = {temp_l, temp_r};
            struct TreeNode * range = make_node("RANGE", arr, 2);
            node->syn_addr = range;
            // free(wrfloop2);
            // free(wrfloop1->rightsib);
            // free(wrfloop1);
        }
        break;
        case 123:
        {
            // <which_range_for_loop> MINUS NUM
            node->syn_addr = node->child;
        }
        break;
        case 124:
        {
            // <which_range_for_loop> PLUS NUM
            node->syn_addr = node->child;
        }
        break;
        case 125:
        {
            // <which_range_for_loop> NUM
            node->syn_addr = node->child;
        }
        break;
        case 126:
        {
            // <default> epsilon
            node->syn_addr = make_node("DEFAULT", NULL, 0);
            // free(node->child);
        }
        break;
        case 127: // <range_bound> PLUS <which_range_bound>
        {
            struct TreeNode * temp = node->child->rightsib;
            get_AST(temp);
            node->child->rightsib = temp->syn_addr;
            node->syn_addr = node->child;
            // free(temp);
            node->child = NULL;
        }
        break;
        case 128: // <module_plist> <module_plist_val> <module_plist_temp>
        {
            struct TreeNode* val = node->child;
            struct TreeNode* temp = val->rightsib;
            get_AST(val);
            get_AST(temp);
            val->syn_addr->rightsib = temp->syn_addr;
            node->syn_addr = make_node("M_PLIST", &val->syn_addr, 1);
            // free(val);
            // free(temp);
        }
        break;
        case 129: // <module_plist_temp> COMMA <module_plist_val> <module_plist_temp>
        {
            struct TreeNode* children[3];
            struct TreeNode* temp = node->child;
            int i = 0;
            while (temp) {
                children[i] = temp;
                temp = temp->rightsib;
                i++;
            }
            get_AST(children[1]);
            get_AST(children[2]);
            children[1]->syn_addr->rightsib = children[2]->syn_addr;
            node->syn_addr = children[1]->syn_addr;
            for (int i = 0; i < 3; i++)
            {
                // free(children[i]);
            }
        }
        break;
        case 130: // <module_plist_temp> epsilon
        {
            node->syn_addr = NULL;
            // free(node->child);
        }
        break;
        case 131: // <module_plist_val> <val_val>
        {
            struct TreeNode* val_val = node->child;
            val_val->inh_addr = NULL;
            get_AST(val_val);
            node->syn_addr = val_val->syn_addr;
            // free(val_val);
        }
        break;
        case 132: // <module_plist_val> PLUS <val_val>
        {
            struct TreeNode* plus = node->child;
            struct TreeNode* val_val = plus->rightsib;
            val_val->inh_addr = plus;
            get_AST(val_val);
            // plus->rightsib = val_val->syn_addr;
            node->syn_addr = val_val->syn_addr;
            // free(val_val);
        }
        break;
        case 133: // <module_plist_val> MINUS <val_val>
        {
            struct TreeNode* minus = node->child;
            struct TreeNode* val_val = minus->rightsib;
            val_val->inh_addr = minus;
            get_AST(val_val);
            // minus->rightsib = val_val->syn_addr;
            node->syn_addr = val_val->syn_addr;
            // free(val_val);
        }
        break;
        case 134: // <module_plist_val> TRUE
        {
            node->syn_addr = node->child;
        }
        break;
        case 135: // <module_plist_val> FALSE
        {
            node->syn_addr = node->child;
        }
        break;
        case 136: // <val_val> NUM
        {
            if (node->inh_addr == NULL)
            {
                node->syn_addr = make_node("VAL_VAL", &node->child, 1);
            }
            else
            {
                struct TreeNode* op = node->inh_addr; // contains PLUS or MINUS
                struct TreeNode* arr[] = {op, node->child};
                node->syn_addr = make_node("VAL_VAL", arr, 2);
            }
        }
        break;
        case 137: // <val_val> RNUM
        {
            if (node->inh_addr == NULL)
            {
                node->syn_addr = make_node("VAL_VAL", &node->child, 1);
            }
            else
            {
                struct TreeNode* op = node->inh_addr; // contains PLUS or MINUS
                struct TreeNode* arr[] = {op, node->child};
                node->syn_addr = make_node("VAL_VAL", arr, 2);
            }
        }
        break;
        case 138: // <val_val> ID <whichID>
        {
            if (node->inh_addr == NULL)
            {
                struct TreeNode* id = node->child;
                struct TreeNode* whichID = id->rightsib;
                get_AST(whichID);
                id->rightsib = whichID->syn_addr;
                node->syn_addr = make_node("VAL_VAL", &id, 1);
                // free(whichID);
            }
            else
            {
                struct TreeNode* id = node->child;
                struct TreeNode* whichID = id->rightsib;
                get_AST(whichID);
                id->rightsib = whichID->syn_addr;
                struct TreeNode* op = node->inh_addr; // contains PLUS or MINUS
                // struct TreeNode* arr[] = {op, id};
                op->rightsib = id;
                node->syn_addr = make_node("VAL_VAL", &op, 1);
                // free(whichID);
            }
        }
        break;
    }
    // printf("Exiting rule %d\n",rule);
}
// int count = 100;
// void print_AST(struct TreeNode* node)
// {
//     // pre-order traversal
//     if (node == NULL)
//         return;
//     // if(!count)return;
//     // print current node
    
//     // label
//     if(node->label)
//     printf("%s\t", node->label);
//     else
//     {
//         printf("%s(%d)\t", node->key, node->line_num);
//     }

//     // parent
//     if(node->parent)
//     {
//         if(node->parent->label)
//         printf("%s\t\t\t", node->parent->label);
//         else 
//         {
//             printf("%s(%d)\t\t\t", node->parent->key, node->parent->line_num);
//         }
//     }
//     else printf("ROOT");

//     if (node->label)
//         printf("\t\t\t");
//     else
//         printf("%s\t\t\t", node->key);

//     printf("\n");
//     // iterate through children
//     // count--;
//     struct TreeNode* temp = node->child;
//     while (temp) {
//         print_AST(temp);
//         temp = temp->rightsib;
//     }
// }


void print_tab(int n) 
{
    if (n != 0)
    {
        for (int i = 0; i < n; i++)
            printf("\t");
    }
    else printf("\t\t\t");
}

void print_AST(struct TreeNode* node)
{
    

    if (node == NULL)
        return;

    // lexeme
    if(node->label)
    {
        // printf("%s", node->label);
    }
    else
    {
        int id_value = get_value("ID", 'T');
        int num_value = get_value("NUM", 'T');
        int rnum_value = get_value("RNUM", 'T');
        int type = node->sym.num;
        if (type == id_value)
        {
            printf("%s", node->property.lexeme);
        }
        else if (type == num_value)
        {
            printf("%d", node->property.value_int);
        }
        else if (type == rnum_value)
        {
            printf("%f", node->property.value_double);
        }
        else
        {
            printf("%s", node->property.lexeme);
        }
        // printf("%s(%d)\t", node->key, node->line_num);
    }
    print_tab(0);

    // current_node
    if (node->label != NULL)
    {
        printf("%s", node->label);
    }
    else
    {
        printf("%s", node->key);
    }
    print_tab(0);

    // line_no
    if (node->label)
    {

    }
    else
    {
        printf("%d", node->line_num);
    }
    print_tab(1);

    // token_name
    printf("----");
    print_tab(0);

    // parent
    if(node->parent)
    {
        if(node->parent->label)
        printf("%s", node->parent->label);
        else 
        {
            // printf("%s(%d)\t\t\t", node->parent->key, node->parent->line_num);
            printf("%s", node->parent->key);
        }
    }
    else printf("ROOT");
    print_tab(0);

    // is_leaf
    if (node->rightsib == NULL && node->child == NULL)
    {
        printf("y");
    }
    else
    {
        printf("n");
    }
    printf("\n");

/* irrelevant to submission
    if (node->label)
        printf("\t\t\t");
    else
        printf("%s\t\t\t", node->key);

    printf("\n");
*/
    // iterate through children
    struct TreeNode* temp = node->child;
    while (temp) {
        print_AST(temp);
        temp = temp->rightsib;
    }
}