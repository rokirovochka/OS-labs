#include <stdio.h>
#define BUFFER_SIZE 256

int main(int argc, char **argv) {
	if(argc < 2) {
		fprintf(stderr, "usage: %s filename\n",argv[0]);
		return -1;
	}

	FILE *fin, *fout;
	char line[BUFFER_SIZE];
	if ((fin = fopen(argv[1], "r")) == NULL) {
		perror("fopen failed");
		return -1;
	}
	fout = popen("wc -l", "w");
	if(fout == NULL) {
		perror("popen failed");
		return -1;
	}
	while (fgets(line, BUFFER_SIZE, fin) != NULL) {
		if (line[0] == '\n') {
			fputs(line, fout);
		}
	}
	fclose(fin);
	if(pclose(fout) == -1) {
		perror("pclose failed");
		return -1;
	}

	return 0;
}
