#include <stdio.h>
#include <stdlib.h>

int disassemble(unsigned char* buffer, int pc){
	unsigned char *code = &buffer[pc];
	printf("%04x ", *code);
	return 1;
}

int main(int argc, const char* argv[]){
	FILE* program = fopen(argv[1], "rb");

	if(!program){
		printf("Error opening file");
		exit(1);
	}

	fseek(program, 0L, SEEK_END);
	int size = ftell(program);
	fseek(program, 0L, SEEK_SET);

	unsigned char* buffer = (char*)malloc(size);
	fread(buffer, size, 1, program);
	fclose(program);

	int pc = 0;
	while(pc<size){
		pc+=disassemble(buffer, pc);
	}

	printf("%d", pc);
	return 0;
}