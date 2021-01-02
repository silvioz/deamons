#ifndef _daemonize
#define _daemonize

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>


#define RUNNING_DIR	"/home/silvio/"
#define LOCK_FILE	"/home/silvio/logPrivateDaemon/exampled.lock"
#define LOG_FILE	"/home/silvio/logPrivateDaemon/exampled.log"

void log_message(char* filename,char* message);

void signal_handler(int sig);

void daemonize();

#endif