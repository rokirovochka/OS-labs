#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {
	pid_t p = fork();
	if(p == -1) {
		perror("fork failed");
		return -1;
	}
	
	if(p) {
		pid_t childId = wait(NULL);
		if(childId == -1) {
			perror("wait failed");
			return -1;
		}
		printf("parent process\n");
	} else {
		execlp("cat", "cat", "file.txt", NULL);
		perror("error");
		return -1;
	}
	return 0;
}