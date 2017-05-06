#include <stdio.h>
#include <stdlib.h>
#include "hmm.h"
#include <string.h>
int epoch;
double sumfora(double*** a,HMM MM,int x,int y,int countepoch)   //cal alpha*aij
{
	double sum=0;
	int i=0,j=0;
	//printf("%lf\n",a[0][5]);
	for(i=0;i<6;i++)      // check formula
	{
		sum+=(a[x][i][countepoch]*MM.transition[i][y]);
	}
	//printf("%.1000llf\n",sum);

	return sum;
}

double sumforb(double*** b,HMM MM,int x,int y,int countepoch,int* ob)   //cal alpha*aij
{
        double sum=0;
        int i=0,j=0;
        //printf("%lf\n",a[0][5]);
        for(j=0;j<6;j++)      // check formula     start here;
        {
                sum+=(MM.transition[y][j]*b[x+1][j][countepoch]*MM.observation[ob[(x+1)+50*countepoch]][j]);
        }
        //printf("%.1000llf\n",sum);

        return sum;
}



double cala(double*** a,int* ob,HMM MM)            // forward     // undebug   //countepoch= epochid
{
	int i=0,j=0,nowobsev=0,countepoch=0;
	for(countepoch=0;countepoch<epoch;countepoch++)
		for(i=0;i<6;i++)  //initial
		{
        		a[0][i][countepoch]=MM.initial[i]*MM.observation[ob[50*countepoch]][i];
		}

	
	for(countepoch=0;countepoch<epoch;countepoch++)
		for(i=1;i<50;i++)    //bug  start from i=??  end in i=??    //  i is the observation j=stage
		{
			for(j=0;j<6;j++)
			{
				a[i][j][countepoch]=sumfora(a,MM,i-1,j,countepoch) * MM.observation[ob[i+50*countepoch]][j];
			}

		}
	

	
	return 0;
}

double calb(double*** b,int* ob,HMM MM)            // backward     // undebug   //countepoch= epochid
{
        int i=0,j=0,nowobsev=0,countepoch=0;
        for(countepoch=0;countepoch<epoch;countepoch++)
                for(i=0;i<6;i++)  //initial
                {
                        b[49][i][countepoch]=1;
                }


        for(countepoch=0;countepoch<epoch;countepoch++)
                for(i=49;i>=1;i--)    //bug  start from i=??  end in i=??    //  i is the observation j=stage
                {
                        for(j=0;j<6;j++)
                        {
                                b[i-1][j][countepoch]+=sumforb(b,MM,i-1,j,countepoch,ob);
                        }

                }



        return 0;
}

double calr(double*** a,double*** b,double *** r)            // garma     // undebug   //countepoch= epochid
{
        int i=0,j=0,nowobsev=0,countepoch=0;
        for(countepoch=0;countepoch<epoch;countepoch++)
                for(i=0;i<50;i++)  //initial
                {
			for(j=0;j<6;j++)
			{
                        r[i][j][countepoch]= a[i][j][countepoch]* b[i][j][countepoch];
			}
                }
	
	double sum =0;
        for(countepoch=0;countepoch<epoch;countepoch++)
	{
                for(i=0;i<50;i++)    
                {
			sum=0;
                        for(j=0;j<6;j++)
                        {
                        	sum=sum+r[i][j][countepoch];   
                        }
			for(j=0;j<6;j++)
			{
				r[i][j][countepoch]/=sum;
			///	printf("%.100llf",r[i][j][countepoch]);
			}

                }
	}


        return 0;
}



double cale(double*** a,double*** b,double **** e,HMM MM,int *ob)            // epsilon     // undebug   //countepoch= epochid
{
        int i=0,j=0,k=0,nowobsev=0,countepoch=0;
        for(countepoch=0;countepoch<epoch;countepoch++)
        {        
		for(i=0;i<49;i++)  //initial
                {
                        for(j=0;j<6;j++)
                        {
				for(k=0;k<6;k++)
				{
                        		e[i][j][k][countepoch]= a[i][j][countepoch]*MM.transition[j][k]*MM.observation[ob[(i+1)+50*countepoch]][k]* b[i+1][k][countepoch];
                        	}
			}
                }
	}
	double sum=0;
	for(countepoch=0;countepoch<epoch;countepoch++)
        {
                for(i=0;i<49;i++)  //initial
                {
			sum=0;
                        for(j=0;j<6;j++)
                        {
                                for(k=0;k<6;k++)
                                {
                                        sum=sum+e[i][j][k][countepoch];
                                }
                        }
			for(j=0;j<6;j++)
                        {
                                for(k=0;k<6;k++)
                                {
                                        e[i][j][k][countepoch]/=sum;
                                }
                        }
			


                }
        }







/*
        long double sum =0;
        for(countepoch=0;countepoch<epoch;countepoch++)
        {
                for(i=0;i<50;i++)
                {
                        sum=0;
                        for(j=0;j<6;j++)
                        {
                                sum=sum+r[i][j][countepoch];
                        }
                        for(j=0;j<6;j++)
                        {
                                r[i][j][countepoch]/=sum;
                                printf("%.100llf",r[i][j][countepoch]);
                        }

                }
        }

*/
        return 0;
}

HMM calhmm(HMM MM,double*** r,double**** e,int* ob)
{
	HMM newmm;
	newmm=MM;                              // is this fail?
	int i=0,j=0,k=0,l=0,countepoch=0;
	double sum=0,sume=0,sumr=0,sum2r=0;
	for(i=0;i<6;i++)
	{
		sum=0;
		for(countepoch=0;countepoch<epoch;countepoch++)
		{
			sum+=r[0][i][countepoch];
		}
		sum/=epoch;
		newmm.initial[i]=sum;
	}
	
	for(i=0;i<6;i++)
	{
		for(j=0;j<6;j++)
		{
			sume=0;
			sumr=0;                                       //total garma is werid, may be promote;
			for(countepoch=0;countepoch<epoch;countepoch++)
			{
				for(k=0;k<49;k++)
				{
				sume=sume+e[k][i][j][countepoch];
				sumr=sumr+r[k][i][countepoch];		
				}			

			}
			newmm.transition[i][j]=(sume/sumr);
			
		}
	}
	
	for(i=0;i<6;i++)
	{
		
		for(j=0;j<6;j++)	//sumr and sum2r initital need to be check;
		{
			sumr=0;
			sum2r=0;
			for(countepoch=0;countepoch<epoch;countepoch++)
			{
				for(k=0;k<50;k++)
				{
					if(ob[k+50*countepoch]==i)
					{
						sumr=sumr+r[k][j][countepoch];
					}
					sum2r=sum2r+r[k][j][countepoch];
				}
			}
			newmm.observation[i][j]=(sumr/sum2r);
		
		}
		

	}	

	return newmm;
}



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




int main (int argc,char* argv[])
{
	HMM hmm_initial,MM,newmm;
	char* iter=argv[1];
	FILE *f=fopen(argv[3],"r");
	FILE *f2=fopen(argv[4],"w");
	int iternum=atoi(iter);
	int i=0,j=0,k=0;
	char seq[50];
	int obsevnum[500000]; // 0=A 1=B 2=C 3=D 4=E 5==F
	char obsev[500000];
	double*** alpha;  // 6 state   500000=t(obsev)
	double*** beta;
	double*** garma;
	double**** epsilon;
	//fscanf(f,"%s",seq);
	//printf("%s",seq);
	epoch=500000/50;
	alpha=(double***)malloc(50*sizeof(double **));
	beta=(double***)malloc(50*sizeof(double **));
	garma=(double***)malloc(50*sizeof(double**));
	epsilon=(double****)malloc(49*sizeof(double***));
	
	for(i=0;i<50;i++)
	{
		alpha[i]=(double**)malloc(6*sizeof(double*));
		beta[i]=(double**)malloc(6*sizeof(double*));
		garma[i]=(double**)malloc(6*sizeof(double*));
	}
	for(i=0;i<49;i++)
        {
                epsilon[i]=(double***)malloc(6*sizeof(double**));
        }

	
	
	
	

	for(i=0;i<50;i++)
        {
        	for(j=0;j<6;j++)
		{ 
	        	alpha[i][j]=(double*)malloc(epoch*sizeof(double));
               		beta[i][j]=(double*)malloc(epoch*sizeof(double));
        		garma[i][j]=(double*)malloc(epoch*sizeof(double));

		}
	}
	for(i=0;i<49;i++)
        {
                for(j=0;j<6;j++)
                {
                        epsilon[i][j]=(double**)malloc(6*sizeof(double*));

                }
        }

	
	for(i=0;i<49;i++)
	{
		for(j=0;j<6;j++)
		{
			for(k=0;k<6;k++)
			{
				epsilon[i][j][k]=(double*)malloc(epoch*sizeof(double));
			}
		}

	}



	for(i=0;i<50;i++)                      // assign big tabel for alpha beta
		for(j=0;j<6;j++)
		{
			for(k=0;k<epoch;k++)
			{
				alpha[i][j][k]=0;
				beta[i][j][k]=0;
				garma[i][j][k]=0;

			}
		}
		
	int l=0;
	for(i=0;i<49;i++)
        {
                for(j=0;j<6;j++)
                {
                        for(k=0;k<6;k++)
                        {
				for(l=0;l<epoch;l++) 
                                	epsilon[i][j][k][l]=0;
                        }
                }

        }



	



	i=0;	
	while(fscanf(f,"%s",seq)!=EOF)		// read file maybe wrong!!!! check
	{
		///if(seq[0]!=0)
		strcat(obsev,seq);
	}




	

	
	
	

	
	//loadHMM( &hmm_initial, "model_init.txt" );
		
	loadHMM(&MM,argv[2]);
	//dumpHMM( stderr, &hmm_initial );
	for(i=0;i<500000;i++)
	{
		obsevnum[i]=change(obsev[i]);
	}
	cala(alpha,obsevnum,MM);
	calb(beta,obsevnum,MM);
	calr(alpha,beta,garma);
	cale(alpha,beta,epsilon,MM,obsevnum);
	newmm=calhmm(MM,garma,epsilon,obsevnum);
	//printf("%lf",MM.initial[0]);
	dumpHMM( f2, &newmm );
	fclose(f2);
	FILE* f3;
	for(i=0;i<iternum-1;i++)
	{	
		//f3=fopen(argv[4],"r");
		loadHMM(&MM,argv[4]);
		//fclose(f3);		
		cala(alpha,obsevnum,MM);
        	calb(beta,obsevnum,MM);
        	calr(alpha,beta,garma);
        	cale(alpha,beta,epsilon,MM,obsevnum);
		newmm=calhmm(MM,garma,epsilon,obsevnum);
		f2=fopen(argv[4],"w");
		dumpHMM(f2,&newmm);
		fclose(f2);
		//printf("%lf",newmm.initial[0]);
		printf("complete %d iternum\n",i+1);
	}



	
	//printf("%f\n", log(1.5) ); // make sure the math library is included
	return 0;


}
