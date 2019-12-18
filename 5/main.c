#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INSTRUCTION_BUFSIZE 256
#define INSTRUCTION_LENGTH 6

#define PARAM1 mode1 ? src[pos + 1] : src[src[pos + 1]]
#define PARAM2 mode2 ? src[pos + 2] : src[src[pos + 2]]

int run_intcode(int *src, const int len, const int input);
void parse_instruction(const int instruction, int *opcode, int *mode1, int *mode2, int *mode3);
int load_program(int **src);

int main(int argc, char *argv[])
{
	int *src;
	int len = load_program(&src);

	run_intcode(src, len, 5);
	return 0;
}

int run_intcode(int *src, const int len, const int input)
{
	int pos = 0;
	while (pos < len)
	{
		int opcode, mode1, mode2, mode3;
		parse_instruction(src[pos], &opcode, &mode1, &mode2, &mode3);
		switch (opcode)
		{
			case 1: ; // add
				int sum = 0;
				sum += mode1 ? src[pos + 1] : src[src[pos + 1]];
				sum += mode2 ? src[pos + 2] : src[src[pos + 2]];
				src[src[pos + 3]] = sum;
				pos += 4;
				break;
			case 2: ; // multiply
				int prod = 1;
				prod *= mode1 ? src[pos + 1] : src[src[pos + 1]];
				prod *= mode2 ? src[pos + 2] : src[src[pos + 2]];
				src[src[pos + 3]] = prod;
				pos += 4;
				break;
			case 3: // write
				src[src[pos + 1]] = input;
				pos += 2;
				break;
			case 4: // read
				printf("%d\n", mode1 ? src[pos + 1] : src[src[pos + 1]]);
				pos += 2;
				break;
			case 5: // jump if true
				if (mode1 ? src[pos + 1] : src[src[pos + 1]])
					pos = mode2 ? src[pos + 2] : src[src[pos + 2]];
				else
					pos += 3;
				break;
			case 6: // jump if false
				if (!(mode1 ? src[pos + 1] : src[src[pos + 1]]))
					pos = mode2 ? src[pos + 2] : src[src[pos + 2]];
				else
					pos += 3;
				break;
			case 7: ; // less than
				// TODO fix these macros to make this work and not be ugly
				// but i have to go take a final now
				// i will probably never open this file again unless i need it for a future challenge
				int _param1 = PARAM1;
				int _param2 = PARAM2;
				if (_param1 < _param2)
					src[src[pos + 3]] = 1;
				else
					src[src[pos + 3]] = 0;
				pos += 4;
				break;
			case 8: ; // equals
				int param1 = PARAM1;
				int param2 = PARAM2;
				if (param1 == param2)
					src[src[pos + 3]] = 1;
				else
					src[src[pos + 3]] = 0;
				pos += 4;
				break;
			case 99:
				pos = len; // to break out of our while loop
				break;
		}
	}
}

int load_program(int **src)
{
	int bufsize = INSTRUCTION_BUFSIZE;
	*src = malloc(bufsize * sizeof(int));
	char c;
	int src_len = 0;
	while (c != EOF)
	{
		char buf[6];
		int i = 0;
		while ((c = getchar()) != ',' && c != EOF)
			buf[i++] = c;
		buf[i] = '\0';
		(*src)[src_len++] = atoi(buf);
	}

	return src_len;
}

void parse_instruction(const int instruction, int *opcode, int *mode1, int *mode2, int *mode3)
{
	*opcode = instruction % 100;
	*mode1 = (instruction / 100) % 10;
	*mode2 = (instruction / 1000) % 10;
	*mode3 = (instruction / 10000) % 10;
}
