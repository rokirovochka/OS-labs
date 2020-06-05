#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define BUFFER_SIZE 256

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "usage: %s filename\n", argv[0]);
		return -1;
	}
	char command[BUFFER_SIZE] = "cat ";
	sprintf(command, "cat %s | grep ^$ | wc -l", argv[1]);

	FILE* pin = popen(command, "r");
	if (pin == NULL) {
		perror("popen failed");
	}
	int answer;
	fscanf(pin, "%d", &answer);
	
	int status = pclose(pin);
	
	if (WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0) {
		fprintf(stderr,"Error with pclose status occured: status: %d", status);	
		return -1;
	}	
	
	printf("result: %d\n", answer);
	return 0;
}
