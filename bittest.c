#include <stdio.h>

#define CAT_MASK 0xE0000000
#define CAT1_OPCODE_MASK 0x1C000000
#define CAT2N3_OPCODE_MASK 0x00070000
#define IMME_MASK 0x0000FFFF
#define RS_MASK 0x1F000000
#define RT_MASK 0x00F80000
#define RD_MASK 0x0000F800
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
					retStr[bufoffset++] = 'J';
					retStr[bufoffset++] = ' ';
					retStr[bufoffset++] = '#';
					imm_val = (iword & 0x03FFFFFF)<<2;
					while(imm_val/10 > 0){retStr[bufoffset++] = '0'+(imm_val%10);imm_val /= 10;}
					retStr[bufoffset++] = '0'+imm_val;
					break;
				case 0x2: //BEQ instruction
					retStr[bufoffset++] = 'B';
					retStr[bufoffset++] = 'E';
					retStr[bufoffset++] = 'Q';
					retStr[bufoffset++] = ' ';
					break;
				case 0x4: //BGTZ instruction
					retStr[bufoffset++] = 'B';
					retStr[bufoffset++] = 'G';
					retStr[bufoffset++] = 'T';
					retStr[bufoffset++] = 'Z';
					retStr[bufoffset++] = ' ';
					break;
				case 0x5: //BREAK instruction
					retStr[bufoffset++] = 'B';
					retStr[bufoffset++] = 'R';
					retStr[bufoffset++] = 'E';
					retStr[bufoffset++] = 'A';
					retStr[bufoffset++] = 'K';
					retStr[bufoffset++] = '\0';
					return 1;
					break;
				case 0x6: //SW instruction
					retStr[bufoffset++] = 'S';
					retStr[bufoffset++] = 'W';
					retStr[bufoffset++] = ' ';
					break;
				case 0x7: //LW instruction
					retStr[bufoffset++] = 'L';
					retStr[bufoffset++] = 'W';
					retStr[bufoffset++] = ' ';
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
					retStr[bufoffset++] = 'A';
					retStr[bufoffset++] = 'D';
					retStr[bufoffset++] = 'D';
					retStr[bufoffset++] = ' ';
					break;
				case 0x1: //SUB instruction
					retStr[bufoffset++] = 'S';
					retStr[bufoffset++] = 'U';
					retStr[bufoffset++] = 'B';
					retStr[bufoffset++] = ' ';
					break;
				case 0x2: //MUL instruction
					retStr[bufoffset++] = 'M';
					retStr[bufoffset++] = 'U';
					retStr[bufoffset++] = 'L';
					retStr[bufoffset++] = ' ';
					break;
				case 0x3: //AND instruction
					retStr[bufoffset++] = 'A';
					retStr[bufoffset++] = 'N';
					retStr[bufoffset++] = 'D';
					retStr[bufoffset++] = ' ';
					break;
				case 0x4: //OR instruction
					retStr[bufoffset++] = 'O';
					retStr[bufoffset++] = 'R';
					retStr[bufoffset++] = ' ';
					break;
				case 0x5: //XOR instruction
					retStr[bufoffset++] = 'X';
					retStr[bufoffset++] = 'O';
					retStr[bufoffset++] = 'R';
					retStr[bufoffset++] = ' ';
					break;
				case 0x6: //NOR instruction
					retStr[bufoffset++] = 'N';
					retStr[bufoffset++] = 'O';
					retStr[bufoffset++] = 'R';
					retStr[bufoffset++] = ' ';
					break;
				default: //error opcode
					printf("Unimplemented opcode in category 2 used. This opcode is reserved.");
					break;
			}
			rs = (iword & RS_MASK)>>RS_OFFSET;
			rt = (iword & RT_MASK)>>RT_OFFSET;
			rd = (iword & RD_MASK)>>RD_OFFSET;
			retStr[bufoffset++] = 'R';
			if(rd/10 > 0){retStr[bufoffset++] = '0'+(int)(rd/10);rd %= 10;}
			retStr[bufoffset++] = '0'+rd;
			retStr[bufoffset++] = ',';
			retStr[bufoffset++] = ' ';
			retStr[bufoffset++] = 'R';
			if(rs/10 > 0){retStr[bufoffset++] = '0'+(int)(rs/10);rs %= 10;}
			retStr[bufoffset++] = '0'+rs;
			retStr[bufoffset++] = ',';
			retStr[bufoffset++] = ' ';
			retStr[bufoffset++] = 'R';
			if(rt/10 > 0){retStr[bufoffset++] = '0'+(int)(rt/10);rt %= 10;}
			retStr[bufoffset++] = '0'+rt;
			break;
		case 0xe: //Category 3
			opcode = (iword & CAT2N3_OPCODE_MASK)>>CAT2N3_OPCODE_OFFSET;
			switch (opcode){
				case 0x0: //ADDI instruction
					retStr[bufoffset++] = 'A';
					retStr[bufoffset++] = 'D';
					retStr[bufoffset++] = 'D';
					retStr[bufoffset++] = 'I';
					retStr[bufoffset++] = ' ';
					break;
				case 0x1: //ANDI instruction
					retStr[bufoffset++] = 'A';
					retStr[bufoffset++] = 'N';
					retStr[bufoffset++] = 'D';
					retStr[bufoffset++] = 'I';
					retStr[bufoffset++] = ' ';
					break;
				case 0x2: //ORI instruction
					retStr[bufoffset++] = 'O';
					retStr[bufoffset++] = 'R';
					retStr[bufoffset++] = 'I';
					retStr[bufoffset++] = ' ';
					break;
				case 0x3: //XORI instruction
					retStr[bufoffset++] = 'X';
					retStr[bufoffset++] = 'O';
					retStr[bufoffset++] = 'R';
					retStr[bufoffset++] = 'I';
					retStr[bufoffset++] = ' ';
					break;
				default: //error opcode
					printf("Unimplemented opcode in category 3 used. This opcode is reserved.");
					break;
			}
			rs = (iword & RS_MASK)>>RS_OFFSET;
			rt = (iword & RT_MASK)>>RT_OFFSET;
			imm_val = (iword & IMME_MASK);
			retStr[bufoffset++] = 'R';
			if(rt/10 > 0){retStr[bufoffset++] = '0'+(int)(rt/10);rt %= 10;}
			retStr[bufoffset++] = '0'+rt;
			retStr[bufoffset++] = ',';
			retStr[bufoffset++] = ' ';
			retStr[bufoffset++] = 'R';
			if(rs/10 > 0){retStr[bufoffset++] = '0'+(int)(rs/10);rs %= 10;}
			retStr[bufoffset++] = '0'+rs;
			retStr[bufoffset++] = ',';
			retStr[bufoffset++] = ' ';
			retStr[bufoffset++] = '#';
			while(imm_val/10 > 0){retStr[bufoffset++] = '0'+(int)(imm_val/10);imm_val %= 10;}
			retStr[bufoffset++] = '0'+imm_val;
			break;
		default: //error handling
			printf("Unimplemented opcode category used. This opcode is reserved.");
			break;
	}
	retStr[bufoffset++] = '\0';
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
		  }
		  prgMem_address+=4;
		  printf("%X\n",instrword);
		  instrword = 0;
		  shiftby =0;
	  }      
  }
  fclose(infile);
  fclose(disfile);
  fclose(simfile);
  fclose(interfile);
  return 0;
}
