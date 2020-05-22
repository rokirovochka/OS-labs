#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv){
	if(argc < 2) {
		printf("Usage: %s [params] \n",argv[0]);
		return -1;
	}
	pid_t pid = fork();
	if (pid == -1) {
		perror("fork failed");
		return -1;
	}
	if (pid) {
		int status;
		do {
			sleep(10);
			pid_t chId = waitpid(pid, &status, 0);
			if (chId == -1) {
				perror("waitpid failed");
				return -1;
			}
			if(WIFSIGNALED(status)) {
				printf("child process terminated due to the signal receipt of a signal\n");
				return -1;
			}
		} while (!WIFEXITED(status));
		printf("child returned with: %d\n", WEXITSTATUS(status));

	} else {		
		execvp(argv[1], &argv[1]);
		perror("exec failed");
		return -1;
	}

	return 0;
}
