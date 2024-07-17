#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct Flags
{
	uint8_t z : 1;
	uint8_t s : 1;
	uint8_t cy : 1;
	uint8_t p : 1;
	uint8_t ac : 1;
	uint8_t pad : 3;
} flags;

typedef struct emu8080
{
	uint8_t a : 1;
	uint8_t b : 1;
	uint8_t c : 1;
	uint8_t d : 1;
	uint8_t e : 1;
	uint8_t h : 1;
	uint8_t l : 1;
	uint16_t sp : 2;
	uint16_t pc : 2;
	uint8_t *memory;
	flags flags;
	uint8_t interrupt;
} emu8080;

void unimplementedInstruction()
{
	printf("Error: Unimplemented instruction");
	exit(1);
}

int parity(int num)
{
	int p = 1;
	while (num)
	{
		p = !p;
		num = num & (num - 1);
	}
	return p;
}

int emulate8080(emu8080 *emu)
{
	unsigned char *opcode = &emu->memory[emu->pc];

	switch (*opcode)
	{
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
		emu->c++;
		if (emu->c == 0)
		{
			emu->b++;
		}
		break;
	case 0x04:
		emu->b++;
		emu->flags.z = (emu->b == 0);
		emu->flags.s = ((emu->b & 0x80) != 0);
		emu->flags.p = parity(emu->b);
		break;
	case 0x05:
		emu->b--;
		emu->flags.z = (emu->b == 0);
		emu->flags.s = ((emu->b & 0x80) != 0);
		emu->flags.p = parity(emu->b);
		break;
	case 0x06:
	case 0x07:
	case 0x08:
	case 0x09:
	{
		uint16_t valH = (emu->h << 8) | (emu->l);
		uint16_t valRP = (emu->b << 8) | (emu->c);
		uint32_t add = (uint32_t)valH + (uint32_t)valRP;
		emu->flags.cy = (add > 0xFFFF);

		emu->h = (add & 0xff00) >> 8;
		emu->l = (add & 0x00ff);
		break;
	}
	case 0x0a:
	case 0x0b:
		emu->c--;
		if (emu->c == 0xff)
		{
			emu->b--;
		}
		break;
	case 0x0c:
		emu->c++;
		emu->flags.z = (emu->c == 0);
		emu->flags.s = ((emu->c & 0x80) != 0);
		emu->flags.p = parity(emu->b);
		break;
	case 0x0d:
		emu->c--;
		emu->flags.z = (emu->c == 0);
		emu->flags.s = ((emu->c & 0x80) != 0);
		emu->flags.p = parity(emu->c);
		break;

	case 0x0e:
	case 0x0f:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
		emu->e++;
		if (emu->e == 0)
		{
			emu->d++;
		}
		break;
	case 0x14:
		emu->d++;
		emu->flags.z = (emu->d == 0);
		emu->flags.s = ((emu->d & 0x80) != 0);
		emu->flags.p = parity(emu->d);
		break;
	case 0x15:
		emu->d--;
		emu->flags.z = (emu->d == 0);
		emu->flags.s = ((emu->d & 0x80) != 0);
		emu->flags.p = parity(emu->d);
		break;

	case 0x16:
	case 0x17:
	case 0x18:
	case 0x19:
	{
		uint16_t valH = (emu->h << 8) | (emu->l);
		uint16_t valRP = (emu->d << 8) | (emu->e);
		uint32_t add = (uint32_t)valH + (uint32_t)valRP;
		emu->flags.cy = (add > 0xFFFF);

		emu->h = (add & 0xff00) >> 8;
		emu->l = (add & 0x00ff);
		break;
	}
	case 0x1a:
	case 0x1b:
		emu->e--;
		if (emu->e == 0xff)
		{
			emu->d--;
		}
		break;
	case 0x1c:
		emu->e++;
		emu->flags.z = (emu->e == 0);
		emu->flags.s = ((emu->e & 0x80) != 0);
		emu->flags.p = parity(emu->e);
		break;
	case 0x1d:
		emu->e--;
		emu->flags.z = (emu->e == 0);
		emu->flags.s = ((emu->e & 0x80) != 0);
		emu->flags.p = parity(emu->e);
		break;
	case 0x1e:
	case 0x1f:
	case 0x20:
	case 0x21:
	case 0x22:
	case 0x23:
		emu->l++;
		if (emu->l == 0)
		{
			emu->h++;
		}
		break;
	case 0x24:
		emu->h++;
		emu->flags.z = (emu->h == 0);
		emu->flags.s = ((emu->h & 0x80) != 0);
		emu->flags.p = parity(emu->h);
		break;
	case 0x25:
		emu->h--;
		emu->flags.z = (emu->h == 0);
		emu->flags.s = ((emu->h & 0x80) != 0);
		emu->flags.p = parity(emu->h);
		break;
	case 0x26:
	case 0x27:
		// TODO
		break;
	case 0x28:
	case 0x29:
	{
		uint16_t valH = (emu->h << 8) | (emu->l);
		uint16_t valRP = (emu->h << 8) | (emu->l);
		uint32_t add = (uint32_t)valH + (uint32_t)valRP;
		emu->flags.cy = (add > 0xFFFF);

		emu->h = (add & 0xff00) >> 8;
		emu->l = (add & 0x00ff);
		break;
	}
	case 0x2a:
	case 0x2b:
		emu->l--;
		if (emu->l == 0xff)
		{
			emu->h--;
		}
		break;
	case 0x2c:
		emu->l++;
		emu->flags.z = (emu->l == 0);
		emu->flags.s = ((emu->l & 0x80) != 0);
		emu->flags.p = parity(emu->l);
		break;
	case 0x2d:
		emu->l--;
		emu->flags.z = (emu->l == 0);
		emu->flags.s = ((emu->l & 0x80) != 0);
		emu->flags.p = parity(emu->l);
		break;

	case 0x2e:
	case 0x2f:
	case 0x30:
	case 0x31:
	case 0x32:
	case 0x33:
		emu->sp++;
		break;
	case 0x34:
	{
		uint8_t add = ++emu->memory[(emu->h << 8) | (emu->l)];
		emu->flags.z = (add == 0);
		emu->flags.s = ((add & 0x80) != 0);
		emu->flags.p = parity(add);
		break;
	}
	case 0x35:
	{
		uint8_t add = --emu->memory[(emu->h << 8) | (emu->l)];
		emu->flags.z = (add == 0);
		emu->flags.s = ((add & 0x80) != 0);
		emu->flags.p = parity(add);
		break;
	}
	case 0x36:
	case 0x37:
		emu->flags.cy = 1;
		break;
	case 0x38:
	case 0x39:
	{
		uint16_t valH = (emu->h << 8) | (emu->l);
		uint16_t valRP = emu->sp;
		uint32_t add = (uint32_t)valH + (uint32_t)valRP;
		emu->flags.cy = (add > 0xFFFF);

		emu->h = (add & 0xff00) >> 8;
		emu->l = (add & 0x00ff);
		break;
	}
	case 0x3a:
	case 0x3b:
		emu->sp--;
		break;
	case 0x3c:
		emu->a++;
		emu->flags.z = (emu->a == 0);
		emu->flags.s = ((emu->a & 0x80) != 0);
		emu->flags.p = parity(emu->a);
		break;
	case 0x3d:
		emu->a--;
		emu->flags.z = (emu->a == 0);
		emu->flags.s = ((emu->a & 0x80) != 0);
		emu->flags.p = parity(emu->a);
		break;
	case 0x3e:
	case 0x3f:
	case 0x40:
	case 0x41:
	case 0x42:
	case 0x43:
	case 0x44:
	case 0x45:
	case 0x46:
	case 0x47:
	case 0x48:
	case 0x49:
	case 0x4a:
	case 0x4b:
	case 0x4c:
	case 0x4d:
	case 0x4e:
	case 0x4f:
	case 0x50:
	case 0x51:
	case 0x52:
	case 0x53:
	case 0x54:
	case 0x55:
	case 0x56:
	case 0x57:
	case 0x58:
	case 0x59:
	case 0x5a:
	case 0x5b:
	case 0x5c:
	case 0x5d:
	case 0x5e:
	case 0x5f:
	case 0x60:
	case 0x61:
	case 0x62:
	case 0x63:
	case 0x64:
	case 0x65:
	case 0x66:
	case 0x67:
	case 0x68:
	case 0x69:
	case 0x6a:
	case 0x6b:
	case 0x6c:
	case 0x6d:
	case 0x6e:
	case 0x6f:
	case 0x70:
	case 0x71:
	case 0x72:
	case 0x73:
	case 0x74:
	case 0x75:
	case 0x76:
	case 0x77:
	case 0x78:
	case 0x79:
	case 0x7a:
	case 0x7b:
	case 0x7c:
	case 0x7d:
	case 0x7e:
	case 0x7f:
	case 0x80:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)emu->b;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x81:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)emu->c;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x82:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)emu->d;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x83:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)emu->e;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x84:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)emu->h;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x85:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)emu->l;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x86:
	{
		uint8_t valM = emu->memory[(emu->h) << 8 | (emu->l)];
		uint16_t add = (uint16_t)emu->a + (uint16_t)valM;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x87:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)emu->a;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x88:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)emu->b + (uint16_t)emu->flags.cy;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x89:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)emu->c + (uint16_t)emu->flags.cy;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x8a:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)emu->d + (uint16_t)emu->flags.cy;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x8b:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)emu->e + (uint16_t)emu->flags.cy;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x8c:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)emu->h + (uint16_t)emu->flags.cy;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x8d:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)emu->l + (uint16_t)emu->flags.cy;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x8e:
	{
		uint8_t valM = emu->memory[(emu->h) << 8 | (emu->l)];
		uint16_t add = (uint16_t)emu->a + (uint16_t)valM + (uint16_t)emu->flags.cy;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x8f:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)emu->a + (uint16_t)emu->flags.cy;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0x90:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)emu->b;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x91:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)emu->c;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x92:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)emu->d;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x93:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)emu->e;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x94:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)emu->h;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x95:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)emu->l;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x96:
	{
		uint8_t valM = emu->memory[(emu->h << 8) | (emu->l)];
		uint16_t sub = (uint16_t)emu->a - (uint16_t)valM;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x97:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)emu->a;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x98:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)emu->b - (uint16_t)emu->flags.cy;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x99:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)emu->c - (uint16_t)emu->flags.cy;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x9a:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)emu->d - (uint16_t)emu->flags.cy;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x9b:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)emu->e - (uint16_t)emu->flags.cy;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x9c:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)emu->h - (uint16_t)emu->flags.cy;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x9d:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)emu->l - (uint16_t)emu->flags.cy;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x9e:
	{
		uint8_t valM = emu->memory[(emu->h << 8) | (emu->l)];
		uint16_t sub = (uint16_t)emu->a - (uint16_t)valM - (uint16_t)emu->flags.cy;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0x9f:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)emu->a - (uint16_t)emu->flags.cy;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0xa0:
	case 0xa1:
	case 0xa2:
	case 0xa3:
	case 0xa4:
	case 0xa5:
	case 0xa6:
	case 0xa7:
	case 0xa8:
	case 0xa9:
	case 0xaa:
	case 0xab:
	case 0xac:
	case 0xad:
	case 0xae:
	case 0xaf:
	case 0xb0:
	case 0xb1:
	case 0xb2:
	case 0xb3:
	case 0xb4:
	case 0xb5:
	case 0xb6:
	case 0xb7:
	case 0xb8:
	case 0xb9:
	case 0xba:
	case 0xbb:
	case 0xbc:
	case 0xbd:
	case 0xbe:
	case 0xbf:
	case 0xc0:
	case 0xc1:
	case 0xc2:
	case 0xc3:
	case 0xc4:
	case 0xc5:
	case 0xc6:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)opcode[1];
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0xc7:
	case 0xc8:
	case 0xc9:
	case 0xca:
	case 0xcb:
	case 0xcc:
	case 0xcd:
	case 0xce:
	{
		uint16_t add = (uint16_t)emu->a + (uint16_t)opcode[1] + (uint16_t)emu->flags.cy;
		emu->flags.z = ((add & 0xff) == 0);
		emu->flags.s = ((add & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((add > 0xff) ? 1 : 0);
		emu->flags.p = parity(add & 0xff);
		emu->a = (uint8_t)(add & 0xff);
		break;
	}
	case 0xcf:
	case 0xd0:
	case 0xd1:
	case 0xd2:
	case 0xd3:
	case 0xd4:
	case 0xd5:
	case 0xd6:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)opcode[1];
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0xd7:
	case 0xd8:
	case 0xd9:
	case 0xda:
	case 0xdb:
	case 0xdc:
	case 0xdd:
	case 0xde:
	{
		uint16_t sub = (uint16_t)emu->a - (uint16_t)opcode[1] - (uint16_t)emu->flags.cy;
		emu->flags.z = ((sub & 0xff) == 0);
		emu->flags.s = ((sub & 0x80) == 0 ? 0 : 1);
		emu->flags.cy = ((sub < 0) ? 1 : 0);
		emu->flags.p = parity(sub & 0xff);
		emu->a = (uint8_t)(sub & 0xff);
		break;
	}
	case 0xdf:
	case 0xe0:
	case 0xe1:
	case 0xe2:
	case 0xe3:
	case 0xe4:
	case 0xe5:
	case 0xe6:
	case 0xe7:
	case 0xe8:
	case 0xe9:
	case 0xea:
	case 0xeb:
	case 0xec:
	case 0xed:
	case 0xee:
	case 0xef:
	case 0xf0:
	case 0xf1:
	case 0xf2:
	case 0xf3:
	case 0xf4:
	case 0xf5:
	case 0xf6:
	case 0xf7:
	case 0xf8:
	case 0xf9:
	case 0xfa:
	case 0xfb:
	case 0xfc:
	case 0xfd:
	case 0xfe:
	case 0xff:
		unimplementedInstruction();
		break;
	}
}

int main()
{
	uint8_t a = 0xff;
	printf("%x", ++a);
}