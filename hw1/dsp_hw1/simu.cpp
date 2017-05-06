#include <stdio.h>
#include <stdlib.h>


int main()
{
	FILE *a,*b;
	double out;
	a=fopen("acc.txt","r");
	b=fopen("output.txt","a");
	fscanf(a,"%lf",&out);
	
	fprintf(b,"%lf\n",out);
	fclose(a);
	fclose(b);
	




}
