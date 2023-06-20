# write make on the WSL
# then write ./compiler to execute
$(CC)= gcc

compiler: parser.o setup.o stackADT.o treeADT_1.o driver.o lexer.o hashtableADT.o AST.o symbolTable.o gen_ir.o code_gen.o
	$(CC) 	 parser.o setup.o stackADT.o treeADT_1.o driver.o lexer.o hashtableADT.o AST.o symbolTable.o gen_ir.o code_gen.o -o compiler
driver.o: driver.c code_gen.h
	$(CC) -c driver.c
code_gen.o: code_gen.c code_gen.h
	$(CC) -c code_gen.c
gen_ir.o: gen_ir.c gen_ir.h
	$(CC) -c gen_ir.c
symbolTable.o: symbolTable.c symbolTable.h
	$(CC) -c symbolTable.c
AST.o: AST.c AST.h
	$(CC) -c AST.c
lexer.o: lexer.c lexer.h
	$(CC) -c lexer.c
hashtableADT.o: hashtableADT.c hashtableADT.h
	$(CC) -c hashtableADT.c
parser.o: parser.c parser.h
	$(CC) -c parser.c
setup.o: setup.c parserDef.h
	$(CC) -c setup.c
stackADT.o: stackADT.c stackADT.h
	$(CC) -c stackADT.c
treeADT_1.o: treeADT_1.c treeADT_1.h
	$(CC) -c treeADT_1.c

clean:
	rm *.o compiler