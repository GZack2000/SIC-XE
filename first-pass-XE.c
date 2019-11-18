#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//sprintf(varname,"%x",s)
int searchop(char s[10],FILE *f1)
{
	rewind(f1);
	printf("\nsearching for %s",s);	
	char x[20];
	fgets(x,20,f1);
	//printf("\nx:%s",x);
	char *p;
	p=strtok(x," \t");
	//printf("\nComparing with %s",p);
	while(feof(f1)==0)	
	{
		
		if(strcmp(p,s)==0)
		{	
			printf("\nFound");
			p=strtok(NULL," \t");
			p=strtok(NULL," \t");	
			printf("\nformat:%d",atoi(p));
			return atoi(p);
		}
		else
		{
			fgets(x,20,f1);
			//printf("\nx:%s",x);
			p=strtok(x," \t");
			//printf("\nComparing with %s",p);
		}
	}
	//printf("\nNot found");	
	return -1;
}
int searchsym(char s[10],FILE *f1)
{
	rewind(f1);
	//printf("\nsearching for %s",s);	
	char x[10];
	fgets(x,10,f1);
	//printf("\nx:%s",x);
	char *p;
	p=strtok(x," ");
	//printf("\nComparing with %s",x);
	while(feof(f1)==0)	
	{
		
		if(strcmp(x,s)==0)
		{	
			//printf("\nFound");	
			return 1;}
		else
		{
			fgets(x,10,f1);
			p=strtok(x," ");
		}
	}
	//printf("\nNot found");	
	return 0;
}
int token_count(char x[100])
{
	int c=0;
	char* token=strtok(x," \t\n");
	//printf("\nc:%dtoken:%s\n",c,token);
	while (token != NULL) 
	{   
		//printf("\nc:%dtoken:%s\n",c,token);		
		c++;
	      token = strtok(NULL, " \t\n"); 	
	} 
  
    return c; 
}
void main()
{
	FILE *in,*out,*symtab,*optab,*len;
	int locctr,dbg,start_addr,length,x;	
	char prefix[10],label[10],opcode[10],operand[10],instrn[30],icpy[30];
	in=fopen("input.txt","r");
	symtab=fopen("symtab.txt","w+");
	optab=fopen("optab.txt","r");
	out=fopen("tempfile.txt","w");
	len=fopen("length.txt","w");
	fgets(instrn,30,in);
	printf("\n%s",instrn);
	strcpy(icpy,instrn);
	char *p;
	x=token_count(instrn);
	printf("\nNo. of tokens:\t%d",x);
	p=strtok(icpy," \t");	
	if(x==1)
	{	
		strcpy(opcode,p);
		strcpy(label,"");
		strcpy(operand,"");	
	}
	else if(x==2)
	{
		strcpy(label,"");
		strcpy(opcode,p);
		p=strtok(NULL," \t\n");
		strcpy(operand,p);
	}			
	else if(x==3)
	{
		strcpy(label,p);
		p=strtok(NULL," \t");		
		strcpy(opcode,p);
		p=strtok(NULL," \t\n");
		strcpy(operand,p);
	}
	printf("\nLabel: %s\tOpcode:%s\tOperand:%s",label,opcode,operand);
	if(strcmp(opcode,"START")==0)
	{	
		locctr=strtol(operand,NULL,16);
		start_addr=locctr;
		fprintf(out,"%s %s %s\n",label,opcode,operand);
		fgets(instrn,30,in);
		printf("\n%s",instrn);
		strcpy(icpy,instrn);
		char *p;
		x=token_count(instrn);
		printf("\nNo. of tokens:\t%d",x);
		p=strtok(icpy," \t");	
		if(x==1)
		{	
			strcpy(opcode,p);
			strcpy(label,"");
			strcpy(operand,"");	
		}
		else if(x==2)
		{
			strcpy(label,"");
			strcpy(opcode,p);
			p=strtok(NULL," \t\n");
			strcpy(operand,p);
		}			
		else if(x==3)
		{
			strcpy(label,p);
			p=strtok(NULL," \t");		
			strcpy(opcode,p);
			p=strtok(NULL," \t\n");
			strcpy(operand,p);
		}
		printf("\nLabel: %s\tOpcode:%s\tOperand:%s",label,opcode,operand);
	}
	else
	{
		locctr=0X0;
		start_addr=0;
	}
	printf("\nlocctr:%X",locctr);
	while(strcmp(opcode,"END")!=0)	
	{
		strcpy(prefix,"");
		fprintf(out,"%X ",locctr);		
		if(strcmp(label,"")!=0) // if(label exists)
		{
			if(searchsym(label,symtab))//search symtab
			{
				printf("\n\nLabel already exists in Label field!!\n\n");
				exit(0);			
			}
			else
			{
				fprintf(symtab,"%s %d\n",label,locctr);
				printf("\nAdding %s to symtab",label);
			}
				
		}		
		//search opcode in OPTAB
		if(opcode[0]=='+')
		{
			printf("\nextracting + from opcode");
			strcpy(prefix,"+");
			char *w,temp[10];
			strcpy(temp,opcode);
			w=strtok(temp,"+");
			printf("\nw:%s",w);
			strcpy(opcode,w);
			//printf("\nOpcode: %s",opcode);
			locctr+=1;
		}
		x=searchop(opcode,optab);
		if(x!=-1)
		{	
			//printf("\nopcode");
			locctr+=x;
		}
		else if(strcmp(opcode,"WORD")==0)
			locctr+=0X3;
		else if(strcmp(opcode,"RESW")==0)
			locctr+=3*atoi(operand);
		else if(strcmp(opcode,"RESB")==0)
			locctr+=atoi(operand);
		else if(strcmp(opcode,"BYTE")==0)
		{
			int ol=strlen(operand)-1;
			//printf("\nBYTE %d",ol);
			if(operand[0]=='C')
				locctr+=0X1*ol-0X3;
			else if(operand[0]=='X')
			{
				dbg=ol-3;
				locctr+=0X1*(dbg/2);
				if(dbg%2!=0)
					locctr++;
			}
			else
				exit(0);
		}	
		else
		{
			printf("\nInvalid opcode!!\n\n");
			exit(0);			
		}
		//restoring + to opcode
		strcat(prefix,opcode);
		strcpy(opcode,prefix);
		//printf("\nRestored opcode:%s",opcode);
		printf("\nlocctr:%X",locctr);
		fprintf(out,"%s %s %s\n",label,opcode,operand);
		fgets(instrn,30,in);
		printf("\n%s",instrn);
		strcpy(icpy,instrn);
		char *p;
		x=token_count(instrn);
		printf("\nNo. of tokens:\t%d",x);
		p=strtok(icpy," \t\n");	
		if(x==1)
		{	
			strcpy(opcode,p);
			strcpy(label,"");
			strcpy(operand,"");	
		}
		else if(x==2)
		{
			strcpy(label,"");
			strcpy(opcode,p);
			p=strtok(NULL," \t\n");
			strcpy(operand,p);
		}			
		else if(x==3)
		{
			strcpy(label,p);
			p=strtok(NULL," \t");		
			strcpy(opcode,p);
			p=strtok(NULL," \t\n");
			strcpy(operand,p);
		}
		printf("\nLabel: %s\tOpcode:%s\tOperand:%s",label,opcode,operand);
		printf("\nopcode length:%d",strlen(opcode));
	}
	printf("\nWriting END to file\n");
 	fprintf(out,"%X %s %s %s\n",locctr,label,opcode,operand);//writing END to temp file
	length=locctr-start_addr;
	printf("\nLength of the program:	%d\n\n",length);
	fprintf(len,"%d",length);
	fclose(in);
	fclose(symtab);
	fclose(optab);
	fclose(out);
	fclose(len);	
}
