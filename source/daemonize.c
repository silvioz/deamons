#include "../header/daemonize.h"

void log_message(char* filename,char* message)
{
    FILE *logfile;
	logfile=fopen(filename,"a");
	if(!logfile) return;
	fprintf(logfile,"%s\n",message);
	fclose(logfile);
}

void signal_handler(int sig)
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
	if(getppid()==1) return; //already a daemon 
	i=fork();
	if (i<0) {
		printf("Can't fork this process\n");
		exit(1); // fork error 
	}
	if (i>0){
		printf("process forked, exiting\n");
		exit(0); // parent exits 
	}
	// child (daemon) continues 
	int a = setsid(); // obtain a new process group 
	printf("SSID setted: %d\n",a);
	/*for (i=getdtablesize();i>=0;--i) close(i); // close all descriptors
	i=open("/dev/null",O_RDWR); dup(i); dup(i); // handle standart I/O //*/
	umask(027); // set newly created file permissions 
	chdir(RUNNING_DIR); // change running directory 
	printf("Acquiring lock file\n");
	lfp=open(LOCK_FILE,O_RDWR|O_CREAT,0640);
	if (lfp<0){
		printf("Can't acquire lock file %s (code: %d)\n",LOCK_FILE, lfp); 
		exit(1); // can not open 
	}
	int lockRes = lockf(lfp,F_TLOCK,0);//flock(lfp,LOCK_EX); //LOCK_NB LOCK_EX
	if (lockRes<0){ 
		printf("Can't lock file %s (code: %d) \n",LOCK_FILE, lockRes);
		exit(0); // can not lock 
	}
	printf("Lock file %s acquired (open: %d, lock: %d)\n",LOCK_FILE,lfp,lockRes);
	// first instance continues 
	sprintf(str,"%d\n",getpid());
	write(lfp,str,strlen(str)); // record pid to lockfile

	printf("Closed descriptor");
	for (i=getdtablesize();i>=0;--i){
        if(i != lfp)
            close(i); /* close all descriptors */
    }
	i=open("/dev/null",O_RDWR); dup(i); dup(i); /* handle standart I/O */

	signal(SIGCHLD,SIG_IGN); // ignore child 
	signal(SIGTSTP,SIG_IGN); // ignore tty signals 
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGHUP,signal_handler); // catch hangup signal 
	signal(SIGTERM,signal_handler); // catch kill signal 

	
}