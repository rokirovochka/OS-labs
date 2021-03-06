#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#define BUFFER_SIZE 256
#define min(x,y) (x<y?x:y)

typedef struct Line {
	int len;
	int offset;
}Line;

Line* buildTable(int fdesc, int *curLine) {
	int maxLines = 256;
	Line* lines = (Line*)malloc(sizeof(Line) * maxLines);
	if (lines == NULL) {
		perror("malloc failed");
		return NULL;
	}
	*curLine = 0;
	lines[0].offset = 0;
	lines[0].len = 0;

	char c;
	char recvBuffer[BUFFER_SIZE];
	int n;
	while (1) {
		if ((n = read(fdesc, recvBuffer, sizeof(char) * BUFFER_SIZE)) <= 0) {
			if (errno == EINTR) {
				errno = 0;
				continue;
			}
			break;
		}

		for (int i = 0; i < n; i++) {
			c = recvBuffer[i];
			if (*curLine + 1 == maxLines) {
				maxLines += 256;
				Line *newLines = realloc(lines, maxLines);
				if (newLines == NULL) {
					perror("realloc failed");
					free(lines);
					return NULL;
				}
				lines = newLines;
			}
			lines[*curLine].len++;
			if (c == '\n') {
				lines[(*curLine) + 1].offset = lines[*curLine].offset + lines[*curLine].len;
				lines[(*curLine) + 1].len = 0;
				(*curLine)++;
			}
		}
	}
	return lines;
}

void printTable(Line* lines, int linesCnt) {
	for (int i = 0; i < linesCnt; i++) {
		printf("len = %d, offset = %d\n", lines[i].len, lines[i].offset);
	}
}


int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "usage: ./lab5.exe filename\n");
		return -1;
	}

	int fdesc = open(argv[1], O_RDONLY);
	if (fdesc == -1) {
		perror(argv[1]);
		return -1;
	}
	int linesCnt = 0;
	Line* lines = buildTable(fdesc, &linesCnt);
	if (lines == NULL) {
		if (close(fdesc)) {
			perror("closing file failed");
			return -1;
		}
		return -1;
	}
	printf("lines = %d\n", linesCnt);
	printTable(lines, linesCnt);

	char recvBuffer[BUFFER_SIZE];
	int idx = 0;
	do {
		printf("enter line number: ");
		int v = scanf("%d", &idx);
		if (v != 1 || idx < 0) {
			fprintf(stderr, "incorrect input\n");
			fflush(stdin);
			continue;
		}
		if (idx > linesCnt) {
			fprintf(stderr, "number should be <= %d\n", linesCnt);
			continue;
		}
		if (idx == 0) {
			printf("stopped\n");
			break;
		}
		if (lseek(fdesc, lines[idx - 1].offset, SEEK_SET) == -1) {
			perror("lseek failed");
			free(lines);
			if (close(fdesc)) {
				perror("closing file failed");
				return -1;
			}
			return -1;
		}
		int n;
		int rest = lines[idx - 1].len;

		while (1) {
			if ((n = read(fdesc, recvBuffer, min(BUFFER_SIZE, rest))) <= 0) {
				if (errno == EINTR) {
					errno = 0;
					continue;
				}
				break;
			}

			for (int i = 0; i < n; i++) {
				printf("%c", recvBuffer[i]);
			}
			rest -= n;
		}
	} while (1);

	free(lines);
	if (close(fdesc)) {
		perror("closing file failed");
		return -1;
	}
	return 0;
}

