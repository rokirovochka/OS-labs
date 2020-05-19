#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char **argv) {
	char *lines[2] ={ "First Line\n","Second Line\n",};
	FILE *ptr = popen("./receiver", "w");
	if(ptr == NULL) {
		perror("popen failed");
		return -1;
	}

	for(int i=0; i<2; i++) {
		if(fprintf(ptr, "%s", lines[i]) != strlen(lines[i])) {
			perror("fprintf failed");
			pclose(ptr);
			return -1;
		}
	}
	pclose(ptr);
	return 0;
}
