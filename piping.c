// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 


int writePipe(int readFlag,int writeFlag,char pipeData[100],int dataLength){
	close(readFlag);
	write(writeFlag,pipeData,dataLength); 
	close(writeFlag); 
	return 1;
}


int readPipe(int readFlag,int writeFlag,char containerVariable[100],int dataLength){
	close(writeFlag);
	read(readFlag,containerVariable,dataLength); 
	close(readFlag);
	return 1;
}

int main() 
{ 

	int fd1[2];
	int fd2[2];

	char fixed_str[] = "forgeeks.org"; 
	char input_str[100]; 
	pid_t p; 

	if (pipe(fd1)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
	if (pipe(fd2)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 

	scanf("%s", input_str); 
	p = fork(); 

	if (p < 0) 
	{ 
		fprintf(stderr, "fork Failed" ); 
		return 1; 
	} 

	// Parent process 
	else if (p > 0) 
	{ 
		char concat_str[100]; 
		writePipe(fd1[0],fd1[1],input_str, strlen(input_str)+1);
		wait(NULL);  
		readPipe(fd2[0],fd2[1],concat_str, 100); 
		printf("Concatenated string %s\n", concat_str); 
	} 

	// child process 
	else
	{ 
		char concat_str[100]; 
		readPipe(fd1[0],fd1[1],concat_str, 100); 

		int k = strlen(concat_str); 
		int i; 
		for (i=0; i<strlen(fixed_str); i++) 
			concat_str[k++] = fixed_str[i]; 

		concat_str[k] = '\0'; // string ends with '\0' 

		// Write concatenated string and close writing end 
		writePipe(fd2[0],fd2[1], concat_str, strlen(concat_str)+1); 
		exit(0); 
	} 
} 
