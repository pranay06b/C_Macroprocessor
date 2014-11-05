#include<stdio.h>
#include<string.h>
   FILE *fp,*fw;
    int countdef=0;
    int c,expanding=0;
    char lbl[20],opc[20],opr[20];
char name[10][10]={"","","","","","","","","",""},arg[10][100]={"","","","","","","","","",""},def[10][4096]={"","","","","","","","","",""};
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
       	printf("\n@@@@@ %d@@@@@",counter);
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
			       	printf("\n@@@@@ %d@@@@@",counter);
				nestmacro(counter,countdef);
				// write_to_file(1,counter);
			   	printf("\nok\n");	
					continue; 	//Not to read same line again
			}
                 write_to_file(0,0);
                 check_endof_macro=strcmp(opc,"MEND"); 
                 if(check_endof_macro!=0)
                 {
                 //printf("%d",countdef);
                strcat(def[countdef], lbl);
                strcat(def[countdef], " ");
                strcat(def[countdef], opc);
                strcat(def[countdef], " ");
                strcat(def[countdef], opr);
                strcat(def[countdef], " ");
                strcat(def[countdef], "\n"); 
                     }        
            }
            	    countdef++;
           
        }
    }
	//printf("\ncountdef%d",countdef);
	test=search("RBUFF");
	printf("a%d",test);
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
int write_to_file(check_Expand,counter)
{
	if(check_Expand==1)
	{
		fprintf(fw, ";");
		fprintf(fw, lbl);
		fprintf(fw, " ");
		fprintf(fw, opc);
		fprintf(fw, " ");	
		fprintf(fw, opr);
		fprintf(fw, "\n");
		//printf("\n****\n%s\n", def[counter]);
		fprintf(fw, def[counter]);
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
