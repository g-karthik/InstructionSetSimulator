#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 1000
#define MAX_LEN 50

typedef struct Instruction
{
        char *instruction;
        int line;
}Instruction;
Instruction instr[MAX];

typedef struct Label
{
        char *label;
        int next_pc;
}Label;
Label lab[MAX];

int no_of_labels = 0;

int regmem[10];
int diff_for_cmp;
int pc = 0;
int ctr = 0;  //the number of instructions

void ReadFile(FILE *fp)
{
        char *buffer = (char*)malloc(sizeof(char)*MAX_LEN);
        int length = 0;
        char ch;
        ch = getc(fp);
        while((ch != EOF))
        {

                if(ch=='\n')
                {
                        instr[ctr].instruction = malloc(MAX_LEN*sizeof(char));
                        strcpy(instr[ctr].instruction,buffer);
                        ctr++;
                        length = 0;
                        buffer = (char*)malloc(sizeof(char)*MAX_LEN);
                        ch = getc(fp);
                        continue;
                }
                buffer[length] = ch;
                length++;
                ch = getc(fp);
        }
}


Instruction* fetch(int pc_local)      //the return pointer needs to be defined in the caller space
{
        Instruction *inst = malloc(sizeof(Instruction));
        (*inst).instruction = malloc(MAX_LEN*sizeof(char));
        strcpy((*inst).instruction,instr[pc_local].instruction);
        (*inst).line = pc_local;
        return inst;
}

int decode(Instruction* inst)   //the returned value is the opcode; if no operation in the instruction (example, label), then return -1
{
        char op[4];
        op[0] = (*inst).instruction[0];
        op[1] = (*inst).instruction[1];
        op[2] = (*inst).instruction[2];
        op[3] = '\0';


        char *possible_op[6];
        int i;
        for(i=0;i<6;i++)
        {
                possible_op[i] = malloc(4*sizeof(char));
        }
        strcpy(possible_op[0],"mov");
        strcpy(possible_op[1],"jmp");
        strcpy(possible_op[2],"mul");
        strcpy(possible_op[3],"add");
        strcpy(possible_op[4],"cmp");
        strcpy(possible_op[5],"jle");

        for(i=0;i<6;i++)
        {
                if(!strcmp(possible_op[i],op))
                {
                        return i;
                }
        }
        //At this stage, the function must be running only if it is a label

        lab[no_of_labels].label = malloc(4*sizeof(char));
        strcpy(lab[no_of_labels].label,op);
        lab[no_of_labels].next_pc = ((*inst).line) + 1;

        no_of_labels++;

        return -1;

}


void execute(Instruction *inst,int opcode)
{
        if(opcode==-1)  //the instruction is a label: continue with next instruction
        {
		pc++;
                return;
        }
        else
        {
                if(opcode==1)   //jmp instruction
                {
                        char templabel[4];
                        templabel[0] = (*inst).instruction[4];
                        templabel[1] = (*inst).instruction[5];
                        templabel[2] = (*inst).instruction[6];
                        templabel[3] = '\0';
                        int i;
                        for(i=0;i<no_of_labels;i++)
                        {
                                if(!strcmp(templabel,lab[i].label))
                                {
                                        pc = lab[i].next_pc;    //pc should be a global variable
                                        return;
                                }
                        }
                        printf("Error! Undefined label reference: %s\n", templabel);
                        printf("Exiting...\n");
                        exit(1);
                }
                if(opcode==0||opcode==2||opcode==3||opcode==4)
                {
                        int regno;
                        int no;
                        int regno2;
                        int no2;
                        if((*inst).instruction[4]=='R')
                        {
                                int flag;
                                regno = (*inst).instruction[5] - 48;
                                if((*inst).instruction[7]=='R')
                                {
                                        regno2 = (*inst).instruction[8] - 48;
                                        flag = 0;
                                }
                                else
                                {
                                        no2 = (*inst).instruction[7] - 48;
                                        flag = 1;
                                }

                                if(flag==0)
                                {
                                        if(opcode==0)
                                        {
                                                regmem[regno2] = regmem[regno];
                                        }
                                        if(opcode==2)
                                        {
                                                regmem[regno2] = regmem[regno2]*regmem[regno];
                                        }
                                        if(opcode==3)
                                        {
                                                regmem[regno2] = regmem[regno2] + regmem[regno];
                                        }
                                        if(opcode==4)
                                        {
                                                diff_for_cmp = regmem[regno2]-regmem[regno];
                                        }
                                }
                                if(flag==1)
                                {
                                        if(opcode==0||opcode==2||opcode==3)
                                        {
                                                printf("Syntax Error! Exiting...");
                                                exit(1);
                                        }
                                        if(opcode==4)
                                        {
                                                diff_for_cmp = no2 - regmem[regno];
                                        }
                                }

                        }
                        else
                        {
                                no = (*inst).instruction[4] - 48;
                                if((*inst).instruction[6]!='R'&&opcode!=4)
                                {       printf("Syntax Error! Exiting...\n");
                                        exit(1);
                                }
                                if((*inst).instruction[6]!='R'&&opcode==4)
                                {
                                        no2 = (*inst).instruction[6] - 48;
                                        diff_for_cmp = no2 - no;
                                }
                                if((*inst).instruction[6]=='R')
                                {
                                        regno2 = (*inst).instruction[7] - 48;

                                        if(opcode==0)
                                        {
                                                regmem[regno2] = no;
                                        }
                                        if(opcode==2)
                                        {
                                                regmem[regno2] *= no;
                                        }
                                        if(opcode==3)
                                        {
                                                regmem[regno2] += no;
                                        }
                                        if(opcode==4)
                                        {
                                                diff_for_cmp = regmem[regno2] - no;
                                        }
                                }
                        }

                }

                if(opcode==5)
                {
                        if(diff_for_cmp<=0)
                        {
                                char templabel[4];
                                templabel[0] = (*inst).instruction[4];
                                templabel[1] = (*inst).instruction[5];
                                templabel[2] = (*inst).instruction[6];
                                templabel[3] = '\0';
                                int i;
                                for(i=0;i<no_of_labels;i++)
                                {
                                        if(!strcmp(templabel,lab[i].label))
                                        {
                                                pc = lab[i].next_pc;    //pc should be a global variable
                                                return;
                                        }
                                }
                                printf("Error! Undefined label reference: %s\n", templabel);
                                printf("Exiting...\n");
                                exit(1);
                        }
                }
        }

        pc++;
}

int main(int argc, char* argv[])
{
	FILE* fp;
	fp = fopen(argv[1], "r");
	int opcode,i;
	ReadFile(fp);
	Instruction* inst_main;

	for(i=0;i<ctr;i++)
	{
		inst_main = fetch(i);
		opcode = decode(inst_main);
	}

	while(pc<ctr)
	{
		inst_main = fetch(pc);
		opcode = decode(inst_main);
		execute(inst_main,opcode);
		
		printf("Current State of Register File:\n");
		for(i=0;i<10;i++)
		{
			printf("R[%d] = %d\n",i,regmem[i]);
		}
	}

	fclose(fp);

}




