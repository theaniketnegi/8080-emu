#include <stdio.h>
#include <stdlib.h>

int disassemble(unsigned char* buffer, int pc){
	unsigned char *code = &buffer[pc];
	printf("%04x ", *code);
	printf("%04x ", pc);
	int size = 1;

	switch(*code){
		case 0x00:
			printf("NOP");
			break;
		case 0x01:
			printf("LXI B,#%02x%02x", code[2], code[1]);
			size=3;
			break;
		case 0x02:
			printf("STAX B");
			break;
		case 0x03:
			printf("INX B");
			break;
		case 0x04:
			printf("INR B");
			break;
		case 0x05:
			printf("DCR B");
			break;
		case 0x06:
			printf("MVI B,#%02x", code[1]);
			size=2;
			break;
		case 0x07:
			printf("RLC");
			break;
		case 0x08:
			printf("NOP");
			break;
		case 0x09:
			printf("DAD B");
			break;
		case 0x0A:
			printf("LDAX B");
			break;
		case 0x0B:
			printf("DCX B");
			break;
		case 0x0C:
			printf("INR C");
			break;
		case 0x0D:
			printf("DCR C");
			break;
		case 0x0E:
			printf("MVI C,#%02x", code[1]);
			size=2;
			break;
		case 0x0F:
			printf("RRC");
			break;
		case 0x10:
			printf("NOP");
			break;
		case 0x11:
			printf("LXI D,#%02x%02x", code[2], code[1]);
			size=3;
			break;
		case 0x12:
			printf("STAX D");
			break;
		case 0x13:
			printf("INX D");
			break;
		case 0x14:
			printf("INR D");
			break;
		case 0x15:
			printf("DCR D");
			break;
		case 0x16:
			printf("MVI D,#%02x", code[1]);
			break;
		case 0x17:
			printf("RAL");
			break;
		case 0x18:
			printf("NOP");
			break;
		case 0x19:
			printf("DAD D");
			break;
		case 0x1A:
			printf("LDAX D");
			break;
		case 0x1B:
			printf("DCX D");
			break;
		case 0x1C:
			printf("INR E");
			break;
		case 0x1D:
			printf("DCR E");
			break;
		case 0x1E:
			printf("MVI E,#%02x", code[1]);
			size=2;
			break;
		case 0x1F:
			printf("RAR");
			break;
		case 0x20:
			printf("RIM");
			break;
		case 0x21:
			printf("LXI H,#%02x%02x", code[2], code[1]);
			size=3;
			break;
		case 0x22:
			printf("SHLD #%02x%02x", code[2], code[1]);
			size=3;
			break;
		case 0x23:
			printf("INX H");
			break;
		case 0x24:
			printf("INR H");
			break;
		case 0x25:
			printf("DCR H");
			break;
		case 0x26:
			printf("MVI H,#%02x", code[1]);
			size=2;
			break;
		case 0x27:
			printf("DAA");
			break;
		case 0x28:
			printf("NOP");
			break;
		case 0x29:
			printf("DAD H");
			break;
		case 0x2A:
			printf("LHLD #%02x%02x", code[2], code[1]);
			size=3;
			break;
		case 0x2B:
			printf("DCX H");
			break;
		case 0x2C:
			printf("INR L");
			break;
		case 0x2D:
			printf("DCR L");
			break;
		case 0x2E:
			printf("MVI L,#%02x", code[1]);
			break;
		case 0x2F:
			printf("CMA");
			break;
		case 0x30:
			printf("SIM");
			break;
		case 0x31:
			printf("LXI SP,#%02x%02x", code[2], code[1]);
			size=3;
			break;
		case 0x32:
			printf("STA #%02x%02x", code[2], code[1]);
			size=3;
			break;
		case 0x33:
			printf("INX SP");
			break;
		case 0x34:
			printf("INR M");
			break;
		case 0x35:
			printf("DCR M");
			break;
		case 0x36:
			printf("MVI M,#%02x", code[1]);
			size=2;
			break;
		case 0x37:
			printf("STC");
			break;
		case 0x38:
			printf("NOP");
			break;
		case 0x39:
			printf("DAD SP");
			break;
		case 0x3A:
			printf("LDA #%02x%02x", code[2], code[1]);
			size=3;
			break;
		case 0x3B:
			printf("DCX SP");
			break;
		case 0x3C:
			printf("INR A");
			break;
		case 0x3D:
			printf("DCR A");
			break;
	}
	printf("\n");
	return size;
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
	return 0;
}