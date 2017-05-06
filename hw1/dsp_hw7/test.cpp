#include <stdio.h>
#include <stdlib.h>
#include "hmm.h"
#include <string.h>
int epoch;

typedef struct node{
	int num;
	double prob;
}node;

int change(char c)                // change the observation to int
{
        switch(c)
        {
                case 'A':
                        return 0;
                case 'B':
                        return 1;
                case 'C':
                        return 2;
                case 'D':
                        return 3;
                case 'E':
                        return 4;
                case 'F':
                        return 5;
        }
return -1;

}

double maxthe(double*** theta,int x,int y,int z,HMM* mm)
{
	double max=0,candi[6];
	int i=0,j=0,k=0;
	for(i=0;i<6;i++)
	{
		candi[i]=theta[x][y][i]*mm[x].transition[i][z];
	}	
	
	for(i=0;i<6;i++)
	{
		if(max<candi[i])
		{
			max=candi[i];
		}
	}
	return max;
}

double findMax(double *** theta,int x)
{

	int i=0,j=0,k=0;
	double max=0;
	
		for(j=0;j<6;j++)
		{
			if(max<theta[x][49][j])
			{
				max=theta[x][49][j];
			}
		}

	return max;



}


node viterbi(HMM* mm,int* ob,int epochnum)     // cal the viterbi model and output the biggest prob model // epoch is for every sample
{
	int number=-1,i=0,j=0,countepoch=0,k=0;
	double P[5];
	double *** theta;
	node node1;
	theta=(double***)malloc(sizeof(double)*5);   //5model 5theta		

	
	for(i=0;i<5;i++)
	{
		theta[i]=(double**)malloc(50*sizeof(double*));
	}
	for(i=0;i<5;i++)
        {	
		for(j=0;j<=50;j++)
		{
                	theta[i][j]=(double*)malloc(6*sizeof(double));
        	}
	}

	for(i=0;i<5;i++)
	{
		for(j=0;j<50;j++)	
		{
			for(k=0;k<6;k++)
			{
				theta[i][j][k]=0;
			}
		}
	}	
	
	for(i=0;i<5;i++)	//initial the theta;
	{
		for(j=0;j<6;j++)
		{
			theta[i][0][j]=mm[i].initial[j]*mm[i].observation[ob[epochnum*50+0]][j];
		}
	}
	

	
	for(i=0;i<5;i++)
	{
		for(j=0;j<49;j++)
		{
			for(k=0;k<6;k++)
			{
				theta[i][j+1][k]=maxthe(theta,i,j,k,mm)*mm[i].observation[ob[(j+1)+epochnum*50]][k];
			}
		}
	
	}
		

	
	for(i=0;i<5;i++)
	{
		for(j=0;j<6;j++)
		{
			P[i]=findMax(theta,i);
		}
	}
	double max=0;
	int count=0;
	for(i=0;i<5;i++)
	{
		if(max<P[i])
		{
			count=i;
			max=P[i];
		}
	}	

	number=count;
	//node1=(node *)malloc(sizeof(node));
	node1.num=count;
	node1.prob=max;
	return node1; 

}
char* judge(node node1)
{
	char *s;
	s=(char*)malloc(13*sizeof(char));
	switch(node1.num)
	{
		case 0:
			strcat(s,"model_01.txt");
			return s;
		case 1:
			strcat(s,"model_02.txt");
                        return s;
		case 2:
			strcat(s,"model_03.txt");
                        return s;
		case 3:
			strcat(s,"model_04.txt");
                        return s;
		case 4:
			strcat(s,"model_05.txt");
                        return s;
	
	
	
	}
strcat(s,"fail");
return s;


}

int main(int argc,char* argv[])
{
	FILE *f=fopen(argv[1],"r");
	FILE *f2=fopen(argv[2],"r");
	FILE *f3;
	HMM mm[5];
	char modelname[20];
	char obsev[125000];
	int obsevnum[125000];
	int epochnum=0;
	char seq[50];
	int id=0;
	int i=0;
	char str[20];
	char str2[20];
	double temp=0;
	FILE *f4;
	double count=0;
	node node1;
	epoch=125000/50;
	while(fscanf(f,"%s",modelname)!=EOF)
	{
		loadHMM(&mm[i],modelname);
		i++;
	}
	
	
	while(fscanf(f2,"%s",seq)!=EOF)          // read file maybe wrong!!!! check
        {
                ///if(seq[0]!=0)
                strcat(obsev,seq);
        }
	for(i=0;i<125000;i++)
        {
                obsevnum[i]=change(obsev[i]);
        }
	printf("pass\n");
	f3=fopen(argv[3],"a"); 
	
	for(epochnum=0;epochnum<epoch;epochnum++)
	{
		node1=viterbi(mm,obsevnum,epochnum);
		fprintf(f3,"%s %e\n",judge(node1),node1.prob);
		
	}	
	fclose(f3);


	if(strcmp(argv[2],"testing_data1.txt")==0)
	{	
		
		f3=fopen("testing_answer.txt","r");
		f4=fopen("result1.txt","r");
	
		if(f3!=NULL)
		{
			while(fscanf(f4,"%s %lf",str,&temp)!=EOF)
			{
				fscanf(f3,"%s",str2);
				if(strcmp(str,str2)==0)
				{
					count++;
				}
			}


			fclose(f4);
			fclose(f3);


			f4=fopen("acc.txt","w");
			fprintf(f4,"%lf",count/epoch);
			fclose(f4);

		}
	}
return 0;
}
