
/*#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

#define RUNNING_DIR	"/tmp"
#define LOCK_FILE	"exampled.lock"
#define LOG_FILE	"exampled.log"

void log_message(filename,message)
char *filename;
char *message;
{
FILE *logfile;
	logfile=fopen(filename,"a");
	if(!logfile) return;
	fprintf(logfile,"%s\n",message);
	fclose(logfile);
}

void signal_handler(sig)
int sig;
{
	switch(sig) {
	case SIGHUP:
		log_message(LOG_FILE,"hangup signal catched");
		break;
	case SIGTERM:
		log_message(LOG_FILE,"terminate signal catched");
		exit(0);
		break;
	}
}

void daemonize()
{
int i,lfp;
char str[10];
	if(getppid()==1) return; /* already a daemon */
	i=fork();
	if (i<0) exit(1); /* fork error */
	if (i>0) exit(0); /* parent exits */
	/* child (daemon) continues */
	setsid(); /* obtain a new process group */
	
	umask(027); /* set newly created file permissions */
    
	chdir(RUNNING_DIR); /* change running directory */
    
	lfp=open(LOCK_FILE,O_RDWR|O_CREAT,0640);
    
	if (lfp<0) exit(1); /* can not open */
    
	if (lockf(lfp,F_TLOCK,0)<0) exit(0); /* can not lock */

    
	/* first instance continues */
	sprintf(str,"%d\n",getpid());
	write(lfp,str,strlen(str)); /* record pid to lockfile */
	signal(SIGCHLD,SIG_IGN); /* ignore child */
	signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGHUP,signal_handler); /* catch hangup signal */
	signal(SIGTERM,signal_handler); /* catch kill signal */
    for (i=getdtablesize();i>=0;--i){
        if(i != lfp)
            close(i); /* close all descriptors */
    }
	i=open("/dev/null",O_RDWR); dup(i); dup(i); /* handle standart I/O */
    
}

void main()
{
	daemonize();
	int i = 0;
	while(1){
		//log_message(LOG_FILE,"ALIVE");
		sleep(1); /* run */
		i++;
		if(i>15) break;
		//break;
	}
}