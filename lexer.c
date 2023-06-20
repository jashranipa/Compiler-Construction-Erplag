/*  Group 10
    ID:	2019B5A70688P	Name:	Abhijith Kannan
    ID:	2020A7PS0003P	Name:	Anushka Bhattacharjee
    ID:	2020A7PS1687P	Name:	Khushi Shah
    ID:	2020A7PS0148P	Name:	Deep Pandya
    ID:	2020A7PS0119P	Name:	Jash Ranipa*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"lexer.h"
// #include"setup.h"

FILE* fp;
int buffer_size;
char* buffer[2];
int curr, next;
int begin, forward;
int flag_begin, flag_forward;
int line_num;
FILE* lexical_errors;

void getStream() {
    /* SWITCH BUFFER INDICES */
    curr = 1 - curr; next = 1 - next;

    /* READ DATA INTO NEXT */
    for (int i = 0; i < buffer_size; i++) {
        char c = (char) fgetc(fp);
        if (c == EOF) {
            buffer[next][i] = '$';
            fclose(fp);
            break;
        }
        buffer[next][i] = c;
    }
}

char* copy_lexeme() {
    char* lexeme;
    lexeme = (char*) malloc(20 * sizeof(char));
    for (int i = 0; i < 20; i++) {
        lexeme[i] = 0;
    }
    if (flag_forward == 0) {
        //both ptrs in curr
        int len = forward - begin;
        if (len > 20) {
            fprintf(lexical_errors, "ID length is too long, at line number: %d\n", line_num);
            // printf("begin: %d, forward: %d, forward_flag: %d\n", begin, forward, flag_forward);
            return NULL;
        }
        for (int i = 0; i < len; i++) {
            char c = buffer[curr][begin+i];
            lexeme[i] = c;
        }
    }
    else {
        //both ptrs in diff buffer
        int len = (buffer_size - begin) + (forward);
        if (len > 20) {
            fprintf(lexical_errors, "ID length is too long, at line number: %d\n", line_num);
            // printf("begin: %d, forward: %d, forward_flag: %d\n", begin, forward, flag_forward);
            return NULL;
        }
        int curr_count = buffer_size - begin;
        for (int i = 0; i < len; i++) {
            char c;
            if (begin + i < buffer_size) {
                c = buffer[curr][begin+i];
            }
            else {
                c = buffer[next][i-curr_count];
            }
            lexeme[i] = c;
        }
    }
    return lexeme;
}

unsigned int copy_int_value() {
    char digits[11];
    for (int i = 0; i < 11; i++) {
        digits[i] = 0;
    }
    if (flag_forward == 0) {
        // both ptrs in same buffer
        int len = forward - begin;
        if (len > 10) {
            fprintf(lexical_errors, "length of NUM is too long, same buffer, at line no: %d\n", line_num);
            return 0;
        }
        for (int i = 0; i < len; i++) {
            char c = buffer[curr][begin+i];
            digits[i] = c;
        }
    }
    else {
        // fwd is in next, begin in curr
        int len = (buffer_size - begin) + forward;
        if (len > 10) {
            fprintf(lexical_errors, "length of NUM is too long, at line no: %d\n", line_num);
            return 0;
        }
        int curr_count = buffer_size - begin;
        for (int i = 0; i < len; i++) {
            char c;
            if (begin + i < buffer_size) {
                c = buffer[curr][begin+i];
            }
            else {
                c = buffer[next][i-curr_count];
            }
            digits[i] = c;
        }
    }
    unsigned int ans = (unsigned int) atoi(digits);
    return ans;
}

double copy_double_value_simple() {
    double ans = 0.0;
    int i = begin;
    int flag_i = 0;

    char c = '0';
    
    while (c != '.') {
        if (i == buffer_size) {
            flag_i = 1;
            i = 0;
        }
        if (flag_i == 0) {
            c = buffer[curr][i];
        }
        else {
            c = buffer[next][i];
        }
        i++;
    }

    int save_fwd = forward;
    int save_flag = flag_forward;
    
    if (i == 0) {
        forward = buffer_size - 1;
        flag_forward = 0;
    }
    else {
        forward = i-1;
        flag_forward = flag_i;
    }

    unsigned int mantissa = copy_int_value();
    forward = save_fwd;
    flag_forward = save_flag;

    ans += (double) mantissa;
    double term = 0.1;

    int len = 0;
    if (flag_i == flag_forward) {
        len = forward - i;
    }
    else {
        len = (buffer_size - i) + forward;
    }

    for (int count = 0; count < len; count++) {
        if (i == buffer_size) {
            flag_i = 1;
            i = 0;
        }
        if (flag_i == 1) {
            c = buffer[next][i];
        }
        else {
            c = buffer[curr][i];
        }
        int digit = c - '0';
        ans += term * digit;
        term /= 10;
        i++;
    }
    return ans;
}

double copy_double_value_with_e() {
    double ans = 0.0;
    int i = begin;
    int flag_i = 0;

    char c = '0';
    
    while (c != 'E' && c != 'e') {
        if (i == buffer_size) {
            flag_i = 1;
            i = 0;
        }
        if (flag_i == 0) {
            c = buffer[curr][i];
        }
        else {
            c = buffer[next][i];
        }
        i++;
    }
    int save_fwd = forward;
    int save_flag = flag_forward;
    
    if (i == 0) {
        forward = buffer_size - 1;
        flag_forward = 0;
    }
    else {
        forward = i-1;
        flag_forward = flag_i;
    }

    double simple = copy_double_value_simple();
    ans += simple;

    forward = save_fwd;
    flag_forward = save_flag;

    if (i == buffer_size) {
        flag_i = 1;
        i = 0;
    }

    // find len of exp
    int len = 0;
    if (flag_i == flag_forward) {
        len = forward - i;
    }
    else {
        len = (buffer_size - i) + forward;
    }

    if (flag_i == 0) {
        c = buffer[curr][i];
    }
    else {
        c = buffer[next][i];
    }
    i++;
    if (i == buffer_size) {
        flag_i = 1;
        i = 0;
    }

    int sign = 0;
    if (c == '-') {
        sign = 2;
    }
    else if (c == '+') {
        sign = 1;
    }

    if (len == 0) {
        fprintf(lexical_errors, "Exponent has 0 digits, at line_num: %d\n", line_num);
        return 0;
    }
    else if (len > 2 && sign == 0) {
        fprintf(lexical_errors, "Exponent has >2 digits, at line_num: %d\n", line_num);
        return 0;
    }
    else if (len > 3 && sign != 0) {
        fprintf(lexical_errors, "Exponent has >2 digits, at line_num: %d\n", line_num);
        return 0;
    }

    int exp = 0;
    if (sign == 0) {
        if (len == 2) {
            exp += (c - '0') * 10;
            if (flag_i == 0) {
                c = buffer[curr][i];
            }
            else {
                c = buffer[next][i];
            }
            exp += c - '0';
        }
        else {
            exp = c - '0';
        }
    }
    else {
        if (len == 2) {
            if (flag_i == 0) {
                c = buffer[curr][i];
            }
            else {
                c = buffer[next][i];
            }
            exp = c - '0';
        }
        else {
            for (int j = 0; j < 2; j++) {
                if (flag_i == 0) {
                    c = buffer[curr][i];
                }
                else {
                    c = buffer[next][i];
                }
                if (i == buffer_size) {
                    i = 0;
                    flag_i = 1;
                }
                i++;
                if (j == 0) 
                    exp += (c - '0') * 10;
                else
                    exp += (c - '0');
            }
        }
    }
    for (int j = 0; j < exp; j++) {
        if (sign == 0 || sign == 1) {
            ans *= 10;
        }
        else {
            ans /= 10;
        }
    }
    return ans;
}

tk getNextToken() {
    int state = 1;
    tk token = (tk) malloc(sizeof(struct token_info));
    for (int i = 0; i < 20; i++)
    {
        token->property.lexeme[i] = 0;
    }
    while (1) {
        if (flag_begin == 1 && flag_forward == 1) {
            getStream();
            flag_forward = 0;
            flag_begin = 0;
        }

        char c;            
        if (flag_forward == 1) {
            c = buffer[next][forward];
        }
        else {
            c = buffer[curr][forward];
        }
        //printf("\n %c %d\n", c, line_num);
        //if(c=='\n')printf("Please %d\n", state);
        switch (state) {            
            case 1:
                if (c == '+') {
                    state = 2;
                }
                if (c == '-') {
                    state=3;
                }
                if(c == '*'){
                    state=4;
                }
                if(c == '/'){
                    state=5;
                }
                if(c == '<'){
                    state=6;
                }
                if(c == '>'){
                    state=8;
                }
                if(c == '='){
                    state=10;
                }
                if(c == '!'){
                    state=12;
                }
                if(c == ':'){
                    state=18;
                }
                if(c == '.'){
                    state=19;
                }
                if(c == ';'){
                    state=21;
                }
                if(c == ','){
                    state=22;
                }
                if(c == '['){
                    state=24;
                }
                if(c == ']'){
                    state=25;
                }
                if(c == '('){
                    state=26;
                }
                if(c == ')'){
                    state=27;
                }
                if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')|| (c == '_')){
                    state=29;
                }
                if(c >= '0' && c <= '9'){
                    state=30;
                }
                if(c == 32 || c == 9){
                    //blank or tab
                    state=36;
                }
                if(c == 10){
                    //line feed
                    //printf("This\n");
                    state=37;
                }
                if(c == '$'){
                    state=41;
                }
            break;
            
            case 2:
                // token->type = PLUS;
                token-> type = get_value("PLUS", 'T');
                token->line_num = line_num;
                token->property.no_value = '+';
                token->property.lexeme[0] = '+';
                begin = forward;
                flag_begin = flag_forward;
                return token;
            break;
            
            case 3:
                // token->type = MINUS;
                token->type = get_value("MINUS", 'T');
                token->line_num = line_num;
                token->property.no_value = '-';
                token->property.lexeme[0] = '-';
                begin = forward;
                flag_begin = flag_forward;
                return token;
            break;
            
            case 4:
                if (c == '*') {
                    state = 28;
                }
                else {
                    // token->type = MUL;
                    token->type = get_value("MUL", 'T');
                    token->line_num = line_num;
                    token->property.no_value = '*';
                    token->property.lexeme[0] = '*';
                    begin = forward;
                    flag_begin = flag_forward;
                    return token;
                }
            break;
            
            case 5:
                // token->type = DIV;
                token->type = get_value("DIV", 'T');
                token->line_num = line_num;
                token->property.no_value = '/';
                token->property.lexeme[0] = '/';
                begin = forward;
                flag_begin = flag_forward;
                return token;
            break;
            
            case 6:
                if (c == '<') {
                    state = 14;
                }
                else if (c == '=') {
                    state = 7;
                }
                else {
                    // token->type = LT;
                    token->type = get_value("LT", 'T');
                    token->line_num = line_num;
                    token->property.no_value = '<';
                    token->property.lexeme[0] = '<';
                    begin = forward;
                    flag_begin = flag_forward;
                    return token;
                }
            break;
            
            case 7:
                // token->type = LE;
                token->type = get_value("LE", 'T');
                token->line_num = line_num;
                token->property.no_value = '=';
                token->property.lexeme[0] = '<';
                token->property.lexeme[1] = '=';
                begin = forward;
                flag_begin = flag_forward;
                return token;
                
            break;
            
            case 8:
                if(c == '>'){
                    state = 15;
                }
                else if(c == '='){
                    state = 9;
                }
                else{
                    // token->type = GT;
                    token->type = get_value("GT", 'T');
                    token->line_num = line_num;
                    token->property.no_value = '>';
                    token->property.lexeme[0] = '>';
                    begin = forward;
                    flag_begin = flag_forward;
                    return token;
                }

            break;
            
            case 9:
                // token->type = GE;
                token->type = get_value("GE", 'T');
                token->line_num = line_num;
                token->property.no_value = '=';
                token->property.lexeme[0] = '>';
                token->property.lexeme[1] = '=';
                begin = forward;
                flag_begin = flag_forward;
                return token;

            break;
            
            case 10:
                if (c == '=') {
                    state = 11;
                }
                else {
                    fprintf(lexical_errors, "Lexical error at line no. %d\n", line_num);
                    // printf("state num: %d\n", state);
                    begin = forward;
                    flag_begin = flag_forward;
                    state = 1;
                }

            break;

            case 11:
                // token->type = EQ;
                token->type = get_value("EQ", 'T');
                token->line_num = line_num;
                token->property.no_value = '=';
                token->property.lexeme[0] = '=';
                token->property.lexeme[1] = '=';
                begin = forward;
                flag_begin = flag_forward;
                return token;


            break;

            case 12:
                if (c == '=') {
                    state = 13;
                }
                else {
                    fprintf(lexical_errors, "Lexical error at line no. %d\n", line_num);
                    // printf("state num: %d\n", state);
                    begin = forward;
                    flag_begin = flag_forward;
                    state = 1;
                }
            break;
            
            case 13:
                // token->type = NE;
                token->type = get_value("NE", 'T');
                token->line_num = line_num;
                token->property.no_value = '=';
                token->property.lexeme[0] = '!';
                token->property.lexeme[1] = '=';
                begin = forward;
                flag_begin = flag_forward;
                return token;
            break;
            
            case 14:
                if (c == '<') {
                    state = 16;
                }
                else {
                    // token->type = DEF;
                    token->type = get_value("DEF", 'T');
                    token->line_num = line_num;
                    token->property.no_value = '<';
                    token->property.lexeme[0] = '<';
                    token->property.lexeme[1] = '<';
                    begin = forward;
                    flag_begin = flag_forward;
                    return token;
                }
            break;
            
            case 15:
                if (c == '>') {
                    state = 17;
                }
                else {
                    // token->type = ENDDEF;
                    token->type = get_value("ENDDEF", 'T');
                    token->line_num = line_num;
                    token->property.no_value = '>';
                    token->property.lexeme[0] = '>';
                    token->property.lexeme[1] = '>';
                    begin = forward;
                    flag_begin = flag_forward;
                    return token;
                }
            break;
            
            case 16:
                // token->type = DRIVERDEF;
                token->type = get_value("DRIVERDEF", 'T');
                token->line_num = line_num;
                token->property.no_value = '<';
                token->property.lexeme[0] = '<';
                token->property.lexeme[1] = '<';
                token->property.lexeme[2] = '<';
                begin = forward;
                flag_begin = flag_forward;
                return token;
            break;
            
            case 17:
                // token->type = DRIVERENDDEF;
                token->type = get_value("DRIVERENDDEF", 'T');
                token->line_num = line_num;
                token->property.no_value = '>';
                token->property.lexeme[0] = '>';
                token->property.lexeme[1] = '>';
                token->property.lexeme[2] = '>';
                begin = forward;
                flag_begin = flag_forward;
                return token;
            break;
            
            case 18:
                if (c == '=') {
                    state = 23;
                }
                else {
                    // token->type = COLON;
                    token->type = get_value("COLON", 'T');
                    token->line_num = line_num;
                    token->property.no_value = ':';
                    token->property.lexeme[0] = ':';
                    begin = forward;
                    flag_begin = flag_forward;
                    return token;
                }
            break;
            
            case 19:
            	//printf("%c",c);
                if (c == '.') {
                    state = 20;
                }
                else {
                    fprintf(lexical_errors, "Lexical error at line no. %d\n", line_num);
                    // printf("state num: %d\n", state);
                    begin = forward;
                    flag_begin = flag_forward;
                    state = 1;
                }
            break;
            
            case 20:
                // token->type = RANGEOP;
                token->type = get_value("RANGEOP", 'T');
                token->line_num = line_num;
                token->property.no_value = '.';
                token->property.lexeme[0] = '.';
                token->property.lexeme[1] = '.';
                begin = forward;
                flag_begin = flag_forward;
                return token;
            break;

            case 21:
                // token->type = SEMICOL;
                token->type = get_value("SEMICOL", 'T');
                token->line_num = line_num;
                token->property.no_value = ';';
                token->property.lexeme[0] = ';';
                begin = forward;
                flag_begin = flag_forward;
                return token;
            break;

            case 22:
                // token->type = COMMA;
                token->type = get_value("COMMA", 'T');
                token->line_num = line_num;
                token->property.no_value = ',';
                token->property.lexeme[0] = ',';
                begin = forward;
                flag_begin = flag_forward;
                return token;
            break;
            
            case 23:
                // token->type = ASSIGNOP;
                token->type = get_value("ASSIGNOP", 'T');
                token->line_num = line_num;
                token->property.no_value = '=';
                token->property.lexeme[0] = ':';
                token->property.lexeme[1] = '=';
                begin = forward;
                flag_begin = flag_forward;
                return token;
            break;
            
            case 24:
                // token->type = SQBO;
                token->type = get_value("SQBO", 'T');
                token->line_num = line_num;
                token->property.no_value = '[';
                token->property.lexeme[0] = '[';
                begin = forward;
                flag_begin = flag_forward;
                return token;
            break;
            
            case 25:
                // token->type = SQBC;
                token->type = get_value("SQBC", 'T');
                token->line_num = line_num;
                token->property.no_value = ']';
                token->property.lexeme[0] = ']';
                begin = forward;
                flag_begin = flag_forward;
                return token;
            break;
            
            case 26:
                // token->type = BO;
                token->type = get_value("BO", 'T');
                token->line_num = line_num;
                token->property.no_value = '(';
                token->property.lexeme[0] = '(';
                begin = forward;
                flag_begin = flag_forward;
                return token;
            break;
            
            case 27:
                // token->type = BC;
                token->type = get_value("BC", 'T');
                token->line_num = line_num;
                token->property.no_value = ')';
                token->property.lexeme[0] = ')';
                begin = forward;
                flag_begin = flag_forward;
                return token;
            break;
            
            case 28:
                if (c != '*' && c != '$' && c!='\n') {
                    state = 28;
                }
                else if (c == '*') {
                    state = 38;
                }
                else if(c=='\n') {
                    line_num++;
                    //printf("In state 28");
                }               
            break;
            
            case 29:
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_' || (c >= '0' && c <= '9')) {
                    state = 29;
                }
                else {
                    if(buffer[curr][begin]=='\n')
                        begin++;
                    // only implemented ID for now, make hash table search for key
                    char* temp_lexeme = copy_lexeme();

                    if (temp_lexeme == NULL)
                    {
                        //printf("NULL DETECTED!!\n");
                        begin = forward;
                        flag_begin = flag_forward;
                        state = 1;
                        break;
                    }

                    int type = get_value(temp_lexeme, 'T');
                    int and_type = get_value("AND", 'T');
                    int or_type = get_value("OR", 'T');
                    int num_type = get_value("NUM", 'T');
                    int rnum_type = get_value("RNUM", 'T');

                    if (type == and_type) {
                        token->type = and_type;
                    }
                    else if (type == or_type) {
                        token->type = or_type;
                    }
                    else {
                        int all_lower = 1;
                        int i = 0;
                        while (temp_lexeme[i] != 0) {
                            if ((temp_lexeme[i] >= 'A' && temp_lexeme[i] <= 'Z') || (temp_lexeme[i] >= '0' && temp_lexeme[i] <= '9')) {
                                all_lower = 0;
                                break;
                            }
                            i++;
                        }
                        for (int i = 0; i < 20; i++) {
                            token->property.lexeme[i] = temp_lexeme[i];
                        }
                        if (all_lower) {
                            char upper_case[20];
                            for (int i = 0; i < 20; i++) {
                                upper_case[i] = 0;
                                if (temp_lexeme[i] != '_' && temp_lexeme[i] != 0) {
                                    upper_case[i] = temp_lexeme[i] - 'a' + 'A';
                                }
                                else if (temp_lexeme[i] == '_') {
                                    upper_case[i] = temp_lexeme[i];
                                }
                            }
                            type = get_value(upper_case, 'T');
                            if (type == -1 || type == num_type || type == rnum_type) {
                                // submit an ID
                                token->type = get_value("ID", 'T');
                                for (int i = 0; i < 20; i++) {
                                    token->property.lexeme[i] = temp_lexeme[i];
                                }
                            }
                            else {
                                token->type = type;
                            }
                        }
                        else {
                            // submit an ID
                            token->type = get_value("ID", 'T');
                        }
                    }

                    token->line_num = line_num;                    
                    begin = forward;
                    flag_begin = flag_forward;
                    // free(temp_lexeme);
                    return token;
                }
            break;
            
            case 30:
                if (c >= '0' && c <= '9') {
                    state = 30;
                }
                else if (c == '.') {
                    state = 31;
                }
                else {
                    // value_int has to be loaded
                    // token->type = NUM;
                    token->type = get_value("NUM", 'T');
                    token->line_num = line_num;
                    token->property.value_int = copy_int_value();
                    begin = forward;
                    flag_begin = flag_forward;
                    return token;
                }
            break;
            
            case 31:
                if (c == '.') {
                    state = 40;
                }
                else if (c >= '0' && c <= '9') {
                    state = 32;
                }
                else {
                    fprintf(lexical_errors, "Lexical error at line no. %d\n", line_num);
                    // printf("state num: %d\n", state);
                    begin = forward;
                    flag_begin = flag_forward;
                    state = 1;
                }
            break;

            case 32:
                if (c == 'E' || c == 'e') {
                    state = 33;
                }
                else if (c >= '0' && c <= '9') {
                    state = 32;
                }
                else {
                    int len = 0;
                    if (flag_forward == 0) {
                        len = forward - begin;
                    }
                    else {
                        len = (buffer_size - begin) + forward;
                    }
                    // token->type = RNUM;
                    token->type = get_value("RNUM", 'T');
                    token->line_num = line_num;
                    token->property.value_double = copy_double_value_simple();
                    begin = forward;
                    flag_begin = flag_forward;
                    return token;   
                }
            break;
            
            case 33:
                if (c == '+' || c == '-') {
                    state = 34;
                }
                else if (c >= '0' && c <= '9') {
                    state = 35;
                }
                else {
                    fprintf(lexical_errors, "Lexical error at line no. %d\n", line_num);
                    // printf("state num: %d\n", state);
                    begin = forward;
                    flag_begin = flag_forward;
                    state = 1;
                }
            break;
            
            case 34:
                if (c >= '0' && c <= '9') {
                    state = 35;
                }
                else {
                    fprintf(lexical_errors, "Lexical error at line no. %d\n", line_num);
                    // printf("state num: %d\n", state);
                    begin = forward;
                    flag_begin = flag_forward;
                    state = 1;
                }
            break;
            
            case 35:
                if (c >= '0' && c <= '9') {
                    state = 35;
                }
                else {
                    // RNUM value has to be loaded still
                    // token->type = RNUM;
                    token->type = get_value("RNUM", 'T');
                    token->line_num = line_num;
                    token->property.value_double = copy_double_value_with_e();
                    begin = forward;
                    flag_begin = flag_forward;
                    return token; 
                }
            break;
            
            case 36:
                if (c == 32 || c == 9) {
                    // ascii for blank or tab space
                    state = 36;
                }
                else {
                    begin = forward;
                    flag_begin = flag_forward;
                    forward--;
                    state = 1;
                }
            break;
            
            case 37:
                line_num++;
                //printf("In state 37");
                forward--;
                // included to adjust /r 'carriage return'
                begin = forward;
                flag_begin = flag_forward;
                state = 1;
            break;
            
            case 38:
                if (c == '*') {
                    state = 39;
                }
                else if(c=='\n') {
                    line_num++;
                    //printf("In state 38");
                }
                else {
                    // printf("Lexical error at line no. %d\n", line_num);
                    // printf("state num: %d\n", state);
                    // begin = forward;
                    // flag_begin = flag_forward;
                    // state = 1;
                    state = 28;
                }
            break;
            
            case 39:
                begin = forward--;
                flag_begin = flag_forward;
                state = 1;
            break;
            
            case 40:
                // retraction
                forward-=2;
                unsigned int value_int = copy_int_value();
                begin = forward;
                flag_begin = flag_forward;
                // token->type = NUM;
                token->type = get_value("NUM", 'T');
                token->line_num = line_num;
                token->property.value_int = value_int;
                return token;
            break;

            case 41:
                // token->type = $;
                // fclose(lexical_errors);
                token->type = get_value("$", 'T');
                token->line_num = line_num;
                token->property.no_value = '$';
                begin = forward;
                flag_begin = flag_forward;
                return token; 
            break;
        }
        forward++;
        if (forward == buffer_size) {
            flag_forward = 1;
            forward = 0;
        }
    }
    return token;
}

void setup_lexer(FILE* a_fp, int a_buffer_size) {
    fp = a_fp;
    buffer_size = a_buffer_size;
    curr = 0; next = 1;
    begin = 0; forward = 0;
    flag_begin = 0; flag_forward = 0;
    line_num = 1;

    /* INITIALIZE TWIN BUFFERS */
    char* curr_buffer = (char*) malloc(buffer_size * sizeof(char));
    char* next_buffer = (char*) malloc(buffer_size * sizeof(char));
    buffer[0] = curr_buffer;
    buffer[1] = next_buffer;
    
    getStream();
    getStream();

    lexical_errors = fopen("lexical_errors.txt", "w");
}

void removeComments(char* testcase_file, char* clean_file) {
    FILE* fp;
    fp = fopen(testcase_file, "r");
    FILE* newfile;
    newfile=fopen(clean_file,"w");
    char curr;
    int state=1;
    //flag set when the first * is found
    //flag reset if no consecutive
    //if * found and flag=1, then comment start

    curr=fgetc(fp);
    while(curr!=EOF) {
        switch(state) {
            case 1: 
                if(curr=='*') {
                    state=2;
                    break;
                }
                fputc(curr,newfile);
                break;
            case 2:
                if(curr=='*') {
                    state=3;
                    break;
                }
                fputc('*',newfile);
                fputc(curr,newfile);
                state=1;
                break;
            case 3:
                if(curr=='*') {
                    state=4;
                    break;
                }
                if(curr=='\n') 
                    fputc(curr,newfile);
                break;
            case 4:
                if(curr=='*') {
                    state=1;
                    break;
                }
                if(curr=='\n') {
                    fputc(curr,newfile);
                }
                state=3;
                break;

        }
        curr=fgetc(fp);
    }
    
    if (state == 2) {
        fputc('*',newfile);
    }
    fclose(fp);
    fclose(newfile);
    return;
}
