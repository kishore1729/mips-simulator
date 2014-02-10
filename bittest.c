#include <stdio.h>

#define CAT_MASK 0xE0000000
#define CAT1_OPCODE_MASK 0x1C000000
#define CAT2N3_OPCODE_MASK 0x00070000
#define IMME_MASK 0x0000FFFF
#define RS_MASK 0x1F000000
#define RT_MASK 0x00F80000
#define RD_MASK 0x0000F800
#define CAT1_RT_MASK 0x001F0000
#define CAT1_BASE_MASK 0x03E00000
#define CAT1_RT_OFFSET 16
#define CAT1_BASE_OFFSET 21
#define CAT_OFFSET 28
#define CAT1_OPCODE_OFFSET 26
#define CAT2N3_OPCODE_OFFSET 16
#define RS_OFFSET 24
#define RT_OFFSET 19
#define RD_OFFSET 11

int genInstrStr(unsigned int iword, char retStr[]){
	int catValue = (iword & CAT_MASK)>>CAT_OFFSET;
	int opcode =0;
	int bufoffset =0;
	int rs,rd,rt,imm_val=0;
	switch (catValue){
		case 0x0: // Category 1
			opcode = (iword & CAT1_OPCODE_MASK)>>CAT1_OPCODE_OFFSET;
			switch (opcode){
				case 0x0: //J instruction
					imm_val = (iword & 0x03FFFFFF)<<2;
					snprintf(retStr, 32, "J #%d", imm_val);
					break;
				case 0x2: //BEQ instruction
					rs = (iword & CAT1_BASE_MASK)>>CAT1_BASE_OFFSET;
					rt = (iword & CAT1_RT_MASK)>>CAT1_RT_OFFSET;
					imm_val = (iword & IMME_MASK)<<2;
					snprintf(retStr, 32, "BEQ R%d, R%d, #%d", rs, rt, imm_val);
					break;
				case 0x4: //BGTZ instruction
					rs = (iword & CAT1_BASE_MASK)>>CAT1_BASE_OFFSET;
					imm_val = (iword & IMME_MASK)<<2;
					snprintf(retStr, 32, "BGTZ R%d, #%d", rs, imm_val);
					break;
				case 0x5: //BREAK instruction
					snprintf(retStr, 32, "BREAK");
					return 1;
					break;
				case 0x6: //SW instruction
					rs = (iword & CAT1_BASE_MASK)>>CAT1_BASE_OFFSET;
					rt = (iword & CAT1_RT_MASK)>>CAT1_RT_OFFSET;
					imm_val = (iword & IMME_MASK);
					snprintf(retStr, 32, "SW R%d, %d(R%d)", rt, imm_val, rs);
					break;
				case 0x7: //LW instruction
					rs = (iword & CAT1_BASE_MASK)>>CAT1_BASE_OFFSET;
					rt = (iword & CAT1_RT_MASK)>>CAT1_RT_OFFSET;
					imm_val = (iword & IMME_MASK);
					snprintf(retStr, 32, "LW R%d, %d(R%d)", rt, imm_val, rs);
					break;
				default: //error opcode
					printf("Unimplemented opcode in category 1 used. This opcode is reserved.");
					break;
			}
			break;
		case 0xc: //Category 2
			opcode = (iword & CAT2N3_OPCODE_MASK)>>CAT2N3_OPCODE_OFFSET;
			switch (opcode){
				case 0x0: //ADD instruction
					bufoffset = snprintf(retStr, 32, "ADD ");
					break;
				case 0x1: //SUB instruction
					bufoffset = snprintf(retStr, 32, "SUB ");
					break;
				case 0x2: //MUL instruction
					bufoffset = snprintf(retStr, 32, "MUL ");
					break;
				case 0x3: //AND instruction
					bufoffset = snprintf(retStr, 32, "AND ");
					break;
				case 0x4: //OR instruction
					bufoffset = snprintf(retStr, 32, "OR ");
					break;
				case 0x5: //XOR instruction
					bufoffset = snprintf(retStr, 32, "XOR ");
					break;
				case 0x6: //NOR instruction
					bufoffset = snprintf(retStr, 32, "NOR ");
					break;
				default: //error opcode
					printf("Unimplemented opcode in category 2 used. This opcode is reserved.");
					break;
			}
			rs = (iword & RS_MASK)>>RS_OFFSET;
			rt = (iword & RT_MASK)>>RT_OFFSET;
			rd = (iword & RD_MASK)>>RD_OFFSET;
			snprintf(retStr+bufoffset, 32-bufoffset, "R%d, R%d, R%d", rd, rs, rt);
			break;
		case 0xe: //Category 3
			opcode = (iword & CAT2N3_OPCODE_MASK)>>CAT2N3_OPCODE_OFFSET;
			switch (opcode){
				case 0x0: //ADDI instruction
					bufoffset = snprintf(retStr, 32, "ADDI ");
					break;
				case 0x1: //ANDI instruction
					bufoffset = snprintf(retStr, 32, "ANDI ");
					break;
				case 0x2: //ORI instruction
					bufoffset = snprintf(retStr, 32, "ORI ");
					break;
				case 0x3: //XORI instruction
					bufoffset = snprintf(retStr, 32, "XORI ");
					break;
				default: //error opcode
					printf("Unimplemented opcode in category 3 used. This opcode is reserved.");
					break;
			}
			rs = (iword & RS_MASK)>>RS_OFFSET;
			rt = (iword & RT_MASK)>>RT_OFFSET;
			imm_val = (iword & IMME_MASK);
			snprintf(retStr+bufoffset, 32-bufoffset, "R%d, R%d, #%d", rt, rs, imm_val);
			break;
		default: //error handling
			printf("Unimplemented opcode category used. This opcode is reserved.");
			break;
	}
	return 0; // regular instruction	
}

int main(int argc, char* argv[])
{
  FILE *infile;
  FILE *disfile;
  FILE *simfile;
  FILE *interfile;
  char c;
  unsigned int instrword = 0;
  int shiftby=0;
  int prgMem_address = 128;
  unsigned char break_reached = 0; 
  int dataMem_address =0;
  char retStr[32]; //19 would be enough, but 32 is a nice number :D
  int dataMem[32]; //Assumed max possible data limit
  int *endOfValidData = dataMem;
  
  if(argc != 2){
	printf("Incorrect input arguments");
	return 1;
  }
  
  infile = fopen(argv[1], "r");
  if (!infile) {
	printf("Invalid file name / file open error");
	return 1;
  }
  disfile = fopen("disassembly.txt", "w");
  if (!disfile) {
	printf("Invalid file name / file open error");
	return 1;
  }
  simfile = fopen("simulation.txt", "w");
  if (!simfile) {
	printf("Invalid file name / file open error");
	return 1;
  }
  interfile = fopen("binaryCodes.txt", "w");
  if (!interfile) {
	printf("Invalid file name / file open error");
	return 1;
  }
  
  printf("Starting Disassembly\n");
  while ((c = getc(infile)) != EOF){
      if ('0' == c || '1' == c){
		  instrword <<= shiftby;
		  instrword |= (c=='0'? 0x00000000 : 0x00000001);
		  fputc(c, disfile);
		  shiftby=1;
		  continue;
	  }
	  else if('\r' == c) continue;
	  else if('\n' == c) {
		  if(break_reached == 0){ //break not yet reached, program memeory active
			break_reached = genInstrStr(instrword, retStr);
			fprintf(disfile, "\t%d\t%s\n", prgMem_address, retStr);
			fprintf(interfile, "%d\n", instrword);
		  }
		  else { //break reached now in data memory
			if(dataMem_address == 0) dataMem_address = prgMem_address;
			fprintf(disfile, "\t%d\t%d\n", prgMem_address, instrword);
			*endOfValidData = instrword;
			endOfValidData++;
		  }
		  prgMem_address+=4;
		  instrword = 0;
		  shiftby =0;
	  }      
  }
  fclose(infile);
  fclose(disfile);
  fclose(interfile);
  printf("Starting Simulation\n");
  disfile = fopen("disassembly.txt", "r");
  if (!disfile) {
	printf("Invalid file name / file open error");
	return 1;
  }
  interfile = fopen("binaryCodes.txt", "r");
  if (!interfile) {
	printf("Invalid file name / file open error");
	return 1;
  }
  fprintf(simfile,"Data\n");
  while(&dataMem[shiftby] != endOfValidData){
	  if(shiftby%8 ==0) fprintf(simfile,"%d:\t",dataMem_address+4*shiftby);
	  if((shiftby+1)%8==0) fprintf(simfile,"%d\n",dataMem[shiftby++]);
	  else fprintf(simfile,"%d\t",dataMem[shiftby++]);
  }
  fclose(simfile);
  return 0;
}
