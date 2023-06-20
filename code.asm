extern printf, scanf, exit
section .data
int: db "%d", 0
real: db "%lf", 0
bool: db "%d", 0
str: db "%s",10, 0
str_no_new_line: db "%s", 0
section .text
;Push all registers
%macro push_all 0
push RAX
push RBX
push RCX
push RDX
push RSP
push RBP
push RSI
push RDI
%endmacro

;Pop all registers
%macro pop_all 0
pop RDI
pop RSI
pop RBP
pop RSP
pop RDX
pop RCX
pop RBX
pop RAX
%endmacro

%macro print_str 1
section .data
%%str db %1,0
section .text
push_all
mov RDI, str
mov RSI, %%str
mov RAX,0
align_16_rsp
call printf
remove_align_16_rsp
pop_all
%endmacro

%macro print_str_no_new_line 1
section .data
%%str_no_new_line db %1,0
section .text
push_all
mov RDI, str_no_new_line
mov RSI, %%str
mov RAX,0
align_16_rsp
call printf
remove_align_16_rsp
pop_all
%endmacro

%macro align_16_rsp 0 
push RBX
push RDX
push RAX
mov RDX, 0
mov RAX, RSP
mov RBX, 16
idiv RBX
mov RBX, RDX
pop RAX
pop RDX
sub RSP, RBX
%endmacro
%macro remove_align_16_rsp 0
add RSP, RBX
pop RBX
%endmacro
global main
main:enter 216, 0
push_all
mov RAX, 5
mov [RBP - 48], RAX
pop_all
push_all
mov RAX, [RBP - 48]
mov [RBP - 24], RAX
pop_all
push_all
mov RAX, 9
mov [RBP - 56], RAX
pop_all
push_all
mov RAX, [RBP - 56]
mov [RBP - 32], RAX
pop_all
push_all
mov RDI, int
mov RDX, RBP
sub RDX, 0
xor RAX, RAX
mov [RDX], RAX
mov RSI, RDX
mov RAX, 0
align_16_rsp
call scanf
remove_align_16_rsp
pop_all
push_all
mov RDI, int
mov RDX, RBP
sub RDX, 8
xor RAX, RAX
mov [RDX], RAX
mov RSI, RDX
mov RAX, 0
align_16_rsp
call scanf
remove_align_16_rsp
pop_all
push_all
mov RAX, [RBP - 0]
mov [RBP - 64], RAX
pop_all
push_all
mov RAX, [RBP - 8]
mov [RBP - 72], RAX
pop_all
push_all
mov RAX, [RBP - 32]
mov [RBP - 80], RAX
pop_all
push_all
mov RAX, [RBP - 72]
mov RBX, [RBP - 80]
mul RBX
mov [RBP - 88], RAX
pop_all
push_all
mov RAX, [RBP - 64]
mov RBX, [RBP - 88]
add RAX, RBX
mov [RBP - 96], RAX
pop_all
push_all
mov RAX, [RBP - 24]
mov [RBP - 104], RAX
pop_all
push_all
mov RAX, [RBP - 32]
mov [RBP - 112], RAX
pop_all
push_all
mov RAX, [RBP - 104]
mov RBX, [RBP - 112]
sub RAX, RBX
mov [RBP - 120], RAX
pop_all
push_all
mov RAX, [RBP - 8]
mov [RBP - 128], RAX
pop_all
push_all
mov RAX, [RBP - 120]
mov RBX, [RBP - 128]
mul RBX
mov [RBP - 136], RAX
pop_all
push_all
mov RAX, [RBP - 96]
mov RBX, [RBP - 136]
add RAX, RBX
mov [RBP - 144], RAX
pop_all
push_all
mov RAX, [RBP - 24]
mov [RBP - 152], RAX
pop_all
push_all
mov RAX, 2
mov [RBP - 160], RAX
pop_all
push_all
mov RAX, [RBP - 152]
mov RBX, [RBP - 160]
mul RBX
mov [RBP - 168], RAX
pop_all
push_all
mov RAX, [RBP - 144]
mov RBX, [RBP - 168]
add RAX, RBX
mov [RBP - 176], RAX
pop_all
push_all
mov RAX, [RBP - 32]
mov [RBP - 184], RAX
pop_all
push_all
mov RAX, [RBP - 0]
mov [RBP - 192], RAX
pop_all
push_all
mov RAX, [RBP - 184]
mov RBX, [RBP - 192]
mul RBX
mov [RBP - 200], RAX
pop_all
push_all
mov RAX, [RBP - 176]
mov RBX, [RBP - 200]
sub RAX, RBX
mov [RBP - 208], RAX
pop_all
push_all
mov RAX, [RBP - 208]
mov [RBP - 16], RAX
pop_all
push_all
print_str ""
mov RDI, int
mov RSI, [RBP - 16]
xor RAX, RAX
call printf
pop_all
call exit
