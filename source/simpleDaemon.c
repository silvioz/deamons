#include "../header/daemonize.h"

void main()
{
	printf("demonizing...\n");
	daemonize();
	printf("daemon");
	int i = 0;
	while(1){
		//log_message(LOG_FILE,"ALIVE");
		sleep(1); /* run */
		i++;
		if(i>5) break;
		//break;
	}
}