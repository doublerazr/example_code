/* data_uncompress.c
 * Trevor Sharpe
 * February 4, 2016
 *
 * This program takes a compressed set of data on standard input and prints out
 * the uncompressed version by using getchar() and fread().*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	/* type is used to read the first character of each "segment", which informs the next process
	 * i is an iterator
	 * n is one less than the number of elements of the given type to be read in
	 * size is one less than the number of characters per string, if applicable
	 * readCheck is used to evaluate the number of elements successfully read
	 * and flag is used to print a space at the appropriate time*/
	int type, i, n, size, readCheck, flag;
	//buffers used to temporarilystore the elements being read
	char cbuf[257];
	int ibuf[256];
	double dbuf[256];
	void *buf;

	flag=0;
	//reads in the first command and checks it and every successive type read for EOF
	type=getchar();
	while (type!=EOF){
		//if type is n, prints a new line and reads next type at end of while loop
		if (type=='n'){
			printf("\n");
			flag=0;
		}
		//for other valid input, reads n and then differentiates between the number types and s for string
		else if (type=='s' || type=='i' || type=='d'){
			n=getchar();
			if (n==EOF){
				fprintf(stderr,"Input error: no size\n");
				return 1;
			}
			//if a string, reads the size variable, then creates a for loop for the number of strings to be read...
			if (type=='s'){
				for (i=0;i<n+1;i++){
					size=getchar();
					if (size==EOF){
						fprintf(stderr,"Input error: no string size\n");
						return 1;
					}
					//...and reads size+1 characters for each one, then prints each one out
					readCheck=fread(cbuf, 1, size+1, stdin);
					if (readCheck<size+1){
						fprintf(stderr, "Input error: not enough chars\n");
						return 1;
					}
					cbuf[size+1]='\0';
					if (flag==1) printf(" ");
					printf("%s", cbuf);
					flag=1;
				}
			}
			//otherwise for a number, sets the size of the element to 4 for integer or 8 for double...
			else {
				if (type=='i'){
					size=4;
					buf=ibuf;
				}
				else{
					size=8;
					buf=dbuf;
				}
				//...and reads in n+1 of that type before printing them out
				readCheck=fread(buf, size, n+1, stdin);
				if (readCheck<n+1){
					if (type=='i') fprintf(stderr, "Input error: not enough ints\n");
					if (type=='d') fprintf(stderr, "Input error: not enough doubles\n");
					return 1;
				}
				for (i=0;i<n+1;i++){
					if (flag==1) printf(" ");
					if (type=='i') printf("%d", ibuf[i]);
					else printf("%.10lg", dbuf[i]);
					flag=1;
				}
			}
		}
		else{
			fprintf(stderr,"Input error: bad type\n");
			return 1;
		}
		type=getchar(); //this is the reading of the next type referred to at the top (allows for EOF check to be used correctly)
	}
	return 0;
}
