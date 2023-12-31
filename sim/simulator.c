#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


#define MAX_LINE 8 // maximum line length in memin.txt.
#define MAX_LABEL 50 // the maximum lable size is 50 and .
#define MAX_MEMO_LINES 512 // maximum number of lines in the memory.
#define NUM_OF_REG 16 // number of registers.

typedef struct Format
{
	int opcode; // 8 bits
	int rd; // 4 bits
	int rs; // 4 bits
	int rt; // 4 bits
	int imm; // 12 bits.
}Format;

// Variables Definitions:
int PC = 0; // program counter
int counter_cycles = 0; // counts the number of cycles.
int memory[MAX_MEMO_LINES]; // memory size
int register_arr[NUM_OF_REG] = { 0 }; //  16 registers 
char memin_arr[MAX_MEMO_LINES][MAX_LINE] = { "00000000" }; // represents memin as a matrix
Format inst = { 0 }; // instruction 
Format* ptr_inst = &inst; // instruction's address

// Functions Decleration:
int fetch(FILE* memin);
void build_instruction(int PC, Format* ptr_inst);
int execute(Format* ptr_inst);
void open_files(char* argv[], FILE** memin, FILE** memout, FILE** trace, FILE** cycles, FILE** regout);
void close_files(FILE** memin, FILE** memout, FILE** trace, FILE** cycles, FILE** regout);





void open_files(char* argv[], FILE** memin, FILE** memout, FILE** trace, FILE** cycles, FILE** regout)
{
	*memin = fopen(argv[1], "r");
	if (memin == NULL) {
		printf("Error: memin file was not opened");
		exit(1);
	}

	*memout = fopen(argv[2], "w");
	if (memout == NULL) {
		printf("Error: memout file was not opened");
		exit(2);
	}

	*regout = fopen(argv[3], "w");
	if (regout == NULL) {
		printf("Error: regout file was not opened");
		exit(3);
	}

	*trace = fopen(argv[4], "w");
	if (trace == NULL) {
		printf("Error: Cannot open file\n");
	    exit(4);
	}

	*cycles = fopen(argv[5], "w");
	if (cycles == NULL) {
		printf("Error: Cannot open file\n");
		exit(5);
	}

}

void close_files(FILE** memin, FILE** memout, FILE** trace, FILE** cycles, FILE** regout)
{
	//close all the files
	fclose(*memin);
	fclose(*memout);
	fclose(*regout);
	fclose(*cycles);
	fclose(*trace);

}

int main(int argc, char* argv[])
{
	FILE* memin = NULL;
	FILE* memout = NULL;
	FILE* trace = NULL;
	FILE* cycles = NULL;
	FILE* regout = NULL;
	char temp[MAX_LINE + 1] = { 0 };
	int test_execution = 0; //Jump Indicator

	open_files( argv, &memin,  &memout,  &trace,  &cycles, &regout);
	
	int eof = fetch(memin);// read memin 

	while (PC >= 0 && PC < MAX_MEMO_LINES && PC < eof)
	{
		//********    TRACE   ********\
		//initilize the trace file. alocating 3 digits for the PC, prints INST, prints the rest of regs in 8 digits (hexa) format.
		memcpy(temp, memin_arr[PC], 8);
		temp[MAX_LINE] = '\0';
		build_instruction(PC, ptr_inst);
		fprintf(trace, "%08X %s 00000000 %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X %08X\n",PC, temp,\
			register_arr[1], register_arr[2], register_arr[3], register_arr[4], register_arr[5], register_arr[6],register_arr[7],\
			register_arr[8], register_arr[9], register_arr[10], register_arr[11], register_arr[12], register_arr[13], register_arr[14], register_arr[15]);
		test_execution = execute(ptr_inst);
		counter_cycles++;
		if (test_execution)
			continue; // the execution did a jump to  another place so we dont need to promote PC
		PC++;
	}
	
	/**********    CYCLES   ************/
	fprintf(cycles, "%d", counter_cycles);//prints the cycles counter

	/**********    REGOUT  ************/ 
	for (int i = 2; i < NUM_OF_REG; i++) //prints the registers list all the regs without imm & zero 
		fprintf(regout, "%08X\n", register_arr[i]);

	/**********     MEMOUT   ************/
	for (int i = 0; i < MAX_MEMO_LINES; i++)//prints the memout data
	{
		fprintf(memout, "%08X\n", memory[i]);
	}

	close_files(&memin, &memout, &trace, &cycles, &regout);
	
	return 0;
}



// This function gets the assembler code from Memin replicate it to "memin_array" and "memory" arrays 
int fetch(FILE *txt_file)
{
	char line[MAX_LINE + 1];
	char temp;
	int i = 0;

	for (i = 0; i < MAX_MEMO_LINES; i++) // sets all the lines in array        
	{
		fgets(line, MAX_LINE + 1, txt_file);
		if (feof(txt_file))
			return i - 1;
		strncpy(memin_arr[i], line, MAX_LINE); // initilize our memory structure base
		memory[i] = strtoul(line, NULL, 16); // copy the data into the memory array
		temp = getc(txt_file); // remove '\n' from the end of the line
	}
	return i;
}

// This function builds the components of the commands (op,rt,rd,...) 
void build_instruction(int PC, Format* ptr_inst)
{
	int num;
	char op_code[3] = { memin_arr[PC][0],memin_arr[PC][1],'\0' }; // op_code will get 2 hexa digits
	ptr_inst->opcode = strtoul(op_code, NULL, 16); // convert the opcode to decimal number

	char rd[2] = { memin_arr[PC][2],'\0' };// rd will get 2 hexa digits
	ptr_inst->rd = strtoul(rd, NULL, 16); // convert the rd to decimal number

	char rs[2] = { memin_arr[PC][3],'\0' };// rs will get 2 hexa digits
	ptr_inst->rs = strtoul(rs, NULL, 16); // convert the rs to decimal number

	char rt[2] = { memin_arr[PC][4],'\0' };// rt will get 2 hexa digits
	ptr_inst->rt = strtoul(rt, NULL, 16); // convert the rt to decimal number

	char imm[4] = { memin_arr[PC][5],memin_arr[PC][6],memin_arr[PC][7],'\0' };// imm will get 3 hexa digits
	ptr_inst->imm = strtoul(imm, NULL, 16); // convert the imm to decimal number

	// check if rd / rs / rt is imm, if yes - load the imm to the Reg Array
	if (ptr_inst->rs == 1) { // rs is immidiate
		num = strtoul(imm, NULL, 16); // convert the imm to decimal and sign extend it
		/*if (imm[0] == 'F') { // check if imm is a negative number
			num |= 0xfffff000;
		}*/
		ptr_inst->imm = num;
		register_arr[ptr_inst->rs] = ptr_inst->imm;
	}

	else if (ptr_inst->rt == 1) { // rt is immidiate
		num = strtoul(imm, NULL, 16); // convert the imm to decimal and sign extend it
		/*if (imm[0] == 'F') { // check if imm is a negative number
			num |= 0xfffff000;
		}*/
		ptr_inst->imm = num;
		register_arr[ptr_inst->rt] = ptr_inst->imm;
	}
	else if (ptr_inst->rd == 1) { // rd is immidiate
		num = strtoul(imm, NULL, 16); // convert the imm to decimal and sign extend it
		/*if (imm[0] == 'F') { // check if imm is a negative number
			num |= 0xfffff000;
		}*/
		ptr_inst->imm = num;
		register_arr[ptr_inst->rd] = ptr_inst->imm;
	}
	else {//the instruction did not use imm, reset imm to 0
		register_arr[1] = 0;
	}
}

// This function identifies the opdode and executes the instruction (registers, memory, PC...) 
int execute(Format* ptr_inst)
{
	switch (ptr_inst->opcode)
	{
	case 0x0: // add
		register_arr[ptr_inst->rd] = register_arr[ptr_inst->rs] + register_arr[ptr_inst->rt];
		return 0;

	case 0x1: //sub
		register_arr[ptr_inst->rd] = register_arr[ptr_inst->rs] - register_arr[ptr_inst->rt];
		return 0;

	case 0x2: //and
		register_arr[ptr_inst->rd] = register_arr[ptr_inst->rs] & register_arr[ptr_inst->rt];
		return 0;

	case 0x3: //or
		register_arr[ptr_inst->rd] = register_arr[ptr_inst->rs] | register_arr[ptr_inst->rt];
		return 0;

	case 0x4: //sll
		register_arr[ptr_inst->rd] = register_arr[ptr_inst->rs] << register_arr[ptr_inst->rt];
		return 0;

	case 0x5: //sra
		register_arr[ptr_inst->rd] = register_arr[ptr_inst->rs] >> register_arr[ptr_inst->rt];
		return 0;

	case 0x6: //srl
		register_arr[ptr_inst->rd] = register_arr[ptr_inst->rs] >> register_arr[ptr_inst->rt];
		register_arr[ptr_inst->rd] = 0x7 && register_arr[ptr_inst->rd];
		return 0;

	case 0x7: //beq
		if (register_arr[ptr_inst->rs] == register_arr[ptr_inst->rt]) {
			PC = register_arr[ptr_inst->rd];
			return 1;
		}return 0;

	case 0x8: //bne 
		if (register_arr[ptr_inst->rs] != register_arr[ptr_inst->rt]) {
			PC = register_arr[ptr_inst->rd];
			return 1;
		}return 0;

	case 0x9: //blt
		if (register_arr[ptr_inst->rs] < register_arr[ptr_inst->rt]) {
			PC = register_arr[ptr_inst->rd];
			return 1;
		}return 0;

	case 0xA: //bgt
		if (register_arr[ptr_inst->rs] > register_arr[ptr_inst->rt]) {
			PC = register_arr[ptr_inst->rd];
			return 1;
		}return 0;

	case 0xB: //ble
		if (register_arr[ptr_inst->rs] <= register_arr[ptr_inst->rt]) {
			PC = register_arr[ptr_inst->rd];
			return 1;
		}return 0;

	case 0xC: //bge
		if (register_arr[ptr_inst->rs] >= register_arr[ptr_inst->rt]) {
			PC = register_arr[ptr_inst->rd];
			return 1;
		}return 0;

	case 0xD: //jal
		register_arr[15] = PC + 1;
		PC = register_arr[ptr_inst->rd];
		return 1;

	case 0xE: //lw
		register_arr[ptr_inst->rd] = memory[(register_arr[ptr_inst->rs] + register_arr[ptr_inst->rt]) % MAX_MEMO_LINES]; //considering the size limitation
		return 0;

	case 0xF: //sw
		memory[(register_arr[ptr_inst->rs] + register_arr[ptr_inst->rt]) % MAX_MEMO_LINES] = register_arr[ptr_inst->rd];
		return 0;

	case 0x13: //halt
		PC = -2;//will stop the loop after the itaration
		return 1;
	}
}
