#include<stdio.h>
#include<stdarg.h>

#include "log.h"

static FILE *logfile = NULL;

void init_logger() {

	static int nevercallmeagain = 0;

	if (nevercallmeagain)
		return;

	if (!logfile) {
		logfile = fopen("smm.log", "w");
	}

	if (!logfile) {
		fprintf(stderr, "WARN: couldn't open smm.log for logging.\n");
	} else {
		fprintf(logfile, "INFO:  initialized logging process.\nINFO:  Aloha!\n");
		nevercallmeagain = 1;
	}

}

void smm_log(int log_level, const char * fmt, ...) {

	va_list ap;

	if (log_level == ERROR)
		fprintf(logfile, "ERROR: ");
	else if (log_level == WARN)
		fprintf(logfile, "WARN:  ");
	else if (log_level == INFO)
		fprintf(logfile, "INFO:  ");
	else if (log_level == DEBUG)
		fprintf(logfile, "DEBUG: ");

	va_start(ap, fmt);
	vfprintf(logfile, fmt, ap);
	va_end(ap);

	fprintf(logfile, "\n");

}


