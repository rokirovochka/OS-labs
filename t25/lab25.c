#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#define BUFFER_SIZE 512


int main(int argc, char **argv) {

	int fd[2];
	pid_t pidRecv, pidSend;
	if (pipe(fd) == -1) {
		perror("pipe failed");
		return -1;
	}

	pidSend = fork();
	if (pidSend == 0) {
		if (close(fd[1]) == -1) {
			perror("close failed");
			return -1;
		}
		char *lines[2] = { "First Line\n", "Second Line\n" };
		for (int i = 0; i < 2; i++) {
			if (write(fd[0], lines[i], strlen(lines[i])) == -1) {
				if (close(fd[0]) == -1) {
					perror("close failed");
					return -1;
				}
				perror("write failed");
				return -1;
			}
		}
		if (close(fd[0]) == -1) {
			perror("close failed");
			return -1;
		}
		return 0;
	}
	else if (pidSend == -1) {
		if (close(fd[0]) == -1 || close(fd[1]) == -1) {
			perror("close failed");
			return -1;
		}
		perror("fork failed");
		return -1;
	}


	pidRecv = fork();
	if (pidRecv == 0) {
		if (close(fd[0]) == -1) {
			perror("close failed");
			return -1;
		}
		char buffer[BUFFER_SIZE];
		int n;
		while (1) {
			if ((n = read(fd[1], buffer, BUFFER_SIZE)) <= 0) {
				if (errno == EINTR) {
					errno = 0;
					continue;
				}
				break;
			}
			for (int i = 0; i < n; i++) {
				buffer[i] = toupper(buffer[i]);
			}
			if (write(STDOUT_FILENO, buffer, n) == -1) {
				if (close(fd[1]) == -1) {
					perror("close failed");
					return -1;
				}
				perror("write failed");
				return -1;
			}
		}

		if (n == -1) {
			perror("read failed");
		}
		if (close(fd[1]) == -1) {
			perror("close failed");
			return -1;
		}
		return n;
	}
	else if (pidRecv == -1) {
		if (close(fd[0]) == -1 || close(fd[1]) == -1) {
			perror("close failed");
			return -1;
		}
		perror("fork failed");
		return -1;
	}
	
	if (close(fd[0]) == -1 || close(fd[1]) == -1) {
		perror("close failed");
		return -1;
	}
	if (wait(NULL) == -1) {
		perror("wait failed");
	}
	if (wait(NULL) == -1) {
		perror("wait failed");
	}

	return 0;
}
