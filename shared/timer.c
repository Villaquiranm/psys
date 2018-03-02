#include "timer.h"

int numberInterruptions;

void clock_settings(unsigned long *quartz, unsigned long *ticks){

  quartz = QUARTZ;
  //TODO vérifier si on doit faire %256
  ticks = QUARTZ/CLOCKFREQ;
}

unsigned long current_clock(){

	outb(0x20, 0x20);
  numberInterruptions++;
}

void wait_clock(unsigned long clock){

  processus.etat = ENDORMI;
  processus.reveille = numberInterruptions + clock;

  //TODO DECOMMENTER APRÈS C'EST FAIT
  //ordonnance();
}
