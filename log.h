enum LogLevels {
	ERROR,
	WARN,
	INFO,
	DEBUG,
};

void init_logger(void);
void smm_log(int log_level, const char * fmt, ...);
