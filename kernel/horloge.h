
#define CLOCKFREQ 100
#define QUARTZ 0x1234DD

void clock_settings(unsigned long *quartz, unsigned long *ticks);
unsigned long current_clock();
void wait_clock(unsigned long clock);
