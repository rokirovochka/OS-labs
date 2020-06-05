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
			if(pclose(ptr) == -1) {
				perror("pclose failed");
				return -1;
			}

			return -1;
		}
	}

	int status = pclose(ptr);
	
	if (WIFEXITED(status) == 0 || WEXITSTATUS(status) != 0) {
		fprintf(stderr,"Error with pclose status occured: status: %d", status);		
		return -1;
	}	
	return 0;
}
