enum LogLevels {
	ERROR,
	WARN,
	INFO,
	DEBUG,
};

void init_logger();
void smm_log(int log_level, const char * fmt, ...);
