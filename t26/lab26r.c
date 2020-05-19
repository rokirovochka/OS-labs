#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#define BUFFER_SIZE 256

int main(int argc, char **argv) {
	char buffer[1000];
	int n;

	while (1) {
		if ((n = read(STDIN_FILENO, buffer, BUFFER_SIZE)) <= 0) {
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
			perror("write failed");
			return -1;
		}
	}

	if (n == -1) {
		perror("read failed");
		return -1;
	}

	return 0;
}
