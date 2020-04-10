#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
	time_t now;

	if (putenv("TZ=US/Pacific") == -1) {
		perror("Error with changing timezone.");
		return 1;
	}

	(void)time(&now);
	printf("%s\n", ctime(&now));

	return 0;
}


