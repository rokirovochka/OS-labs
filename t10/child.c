#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv){
	printf("child pid = %d\n", getpid());
	if(argc < 2) return -1;
	int var;
	scanf("%d", &var);
	printf("%s is child argument\n", argv[1]);
	return argc;
}
