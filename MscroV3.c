#include<stdio.h>
#include<string.h>
#include<stddef.h>.
#include "str_rep.h"
   FILE *fp,*fw;
    int countdef=0;
    int c,expanding=0;
    char lbl[20],opc[20],opr[20];
	char name[10][10]={"","","","","","","","","",""},arg[10][100]={"","","","","","","","","",""},def[30][4096]={"","","","","","","","","",""};
	char unique_Label='A';


	
int main (){
 	int test;
    int counter=99;
    int j,check_endof_macro;
    check_endof_macro=1;
    j=0;
    fp = fopen("sic_xecode.c","r");
    fw = fopen("expanded.c","w");
    while(1){
        j++;
        if( feof(fp) ){break ;
        }
        fscanf(fp,"%s %s %s",lbl,opc,opr);	
        counter=search(opc);
        if(counter!=99)
        {
       	//printf("\n@@@@@ %d@@@@@",counter);
	 write_to_file(1,counter);
   	printf("\nok\n");	
		continue; 	//Not to read same line again
        }
        write_to_file(0,0);
        if(strcmp(opc,"MACRO")==0){  
            strcat(arg[countdef], opr);
            strcat(name[countdef], lbl);
		check_endof_macro=1;
            while(check_endof_macro)     
               {          
                 fscanf(fp,"%s %s %s",lbl,opc,opr);
		 counter=search(opc);
			if(counter!=99)
			{
		//	       	printf("\n@@@@@ %d@@@@@",counter);
				nestmacro(counter,countdef);
				// write_to_file(1,counter);
			   	printf("\nok\n");	
					continue; 	//Not to read same line again
			}
                 write_to_file(0,0);
                 check_endof_macro=strcmp(opc,"MEND"); 
                 if(check_endof_macro!=0)
                 {

              write_to_deftab(countdef);
                     }        
            }
            	    countdef++;
           
        }
    }
	//printf("\ncountdef%d",countdef);
	test=search("RBUFF");

    fclose(fp);
}













int search(char *macro_name)
{
	int i;

	for(i=0;i<countdef;i++)
	{
		if(strcmp(macro_name,name[i])==0)
		{
		//	printf("\nccc%d\n",i);
			return i;
		}
	}
	return 99;
}
char *split_Argtab(char str[128],int count_Split)
{
	
	char * pch=" ";

 // printf ("Splitting string \"%s\" into tokens:\n",str);
  if(count_Split>0)
  {
  	 pch = strtok (str,",");
  	 
  while (pch != NULL && count_Split>1)
  {
   
    pch = strtok (NULL, ",");
    count_Split--;
  }	
  }
 //printf("%s\n",pch );
  return pch;

}
/*
int count_Argtab(char a[30])
{
	int count_arg=0,i=0;
	/*char *t;
	t=st;
	//*s='\0';
	printf("%s\n",t);
	while(*t!='a')
	{
		if(*t==',')
			count_arg++;
			printf("check\n");		
		t++;
	}*/
		/*
		printf("%s\n",a );
	int len=strlen(a)-1;
	printf("%c\n",a[0] );
	while(i!=3)
	{
		if(!strcmp(',',a[i]))
			count_arg++;
		i++;
	}
	return count_arg;
}*/

int write_to_file(check_Expand,counter)
{
	char operands[10][128]={"","","","","","","","","",""};
	char arg_operands[10][128]={"","","","","","","","","",""};
	char * pch;
	char *split_opnd=" ";
	char *token;
	int icount=0,len=0,ii=0,count_arg=0,i,count_Split;
	char *opnd_Replace;
	char dummy[128]={""};
	char def_buffer[4096];
	if(check_Expand==1)
	{
		fprintf(fw, ";");
		fprintf(fw, lbl);
		fprintf(fw, " ");
		fprintf(fw, opc);
		fprintf(fw, " ");	
		fprintf(fw, opr);
		fprintf(fw, "\n");
		//printf("%s",split_Argtab(arg[counter],2));
		//Splitting Operand
		len=strlen(opr)-1;
		while(ii!=len)
		{
			if(opr[ii]==',')
				count_arg++;
			ii++;
		}
		
		/*for ( i = 0; i <count_arg ; i++)
		{
			count_Split=i+1;
			if(count_Split>0)
			  {
				  	 split_opnd = strtok (opr,",");
				  	 
				  while (split_opnd != NULL && count_Split>1)
				  {
				   
				    split_opnd = strtok (NULL, ",");
				    count_Split--;
				  }	
			  };
			  
			  strcat(operands[i],split_opnd);
		}*/
			  
		for (i =0; i <=count_arg ; i++)
		{
			/* code */
			strcpy(dummy,opr);
			//printf("#%s\n\n",split_Argtab(dummy,i+1));
			strcat(operands[i],split_Argtab(dummy,i+1));

		}
		//Splitted Opnd
		count_arg=0;
		ii=0;
		//Splitting Arguments
		len=strlen(arg[counter])-1;
		while(ii!=len)
		{
			if(arg[counter][ii]==',')
				count_arg++;
			ii++;
		}

		for (i =0; i <=count_arg ; i++)
		{
			/* code */
			strcpy(dummy,arg[counter]);
			//printf("#%s\n\n",split_Argtab(dummy,i+1));
			strcat(arg_operands[i],split_Argtab(dummy,i+1));
			
		}
		//Splitting arg done
		//Copying argtab
		i=0;
		for (i =0; i <=count_arg ; i++)
		{
			/* code */
			//printf("%s\n",arg_operands[0] );
			//opnd_Replace = strstr (def[counter],arg_operands[0] );
			//strncpy (opnd_Replace,"sam",3);
			strcat(def_buffer,replace_str2(def[counter],arg_operands[i],operands[i]));
			def[counter][0]='\0';
			strcat(def[counter],def_buffer);
			def_buffer[0]='\0';
			printf("%s\n",def[counter] );
		}
		
		//copying done
		fprintf(fw, def[counter]);
		count_arg=0;//to count no. of again for different macro
		return 2;
	}
	fprintf(fw, lbl);
	fprintf(fw, " ");
	fprintf(fw, opc);
	fprintf(fw, " ");	
	fprintf(fw, opr);
	fprintf(fw, "\n");
	return 1;
}
int nestmacro(int counter,int macroCounter)
{
	 strcat( def[macroCounter],def[counter]);
}
int write_to_deftab(int deftab_Counter)
{
	     		char label[40]=" ";
	     		if(strcmp("NULL",lbl)!=0)
	     		{
	     			//strcat(label,'$');
	     			label[0]='$';
	     			label[1]=unique_Label;
	     			unique_Label = unique_Label + 1;
	     			strcat(label,lbl);
	     			strcat(def[deftab_Counter], label);
	     		}
	     		else
	     		{
	     			strcat(def[deftab_Counter], lbl);

	     		}
                strcat(def[deftab_Counter], " ");
                strcat(def[deftab_Counter], opc);
                strcat(def[deftab_Counter], " ");
                strcat(def[deftab_Counter], opr);
                strcat(def[deftab_Counter], "          ");
                strcat(def[deftab_Counter], "\n"); 
}
