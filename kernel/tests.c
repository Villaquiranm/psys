#include <debug.h>
#include "processus.h"
#include "tests.h"

#define TESTS_NUMBER 3
void (*functions[TESTS_NUMBER])(void) = {test1, test2, test3};

void execute_tests() {
  for (int i = 0; i < TESTS_NUMBER; i++) {
    printf("Executing test %d\n", i+1);
    (functions[i])();
  }
}

//-----------------------------------------------------------------------------

#define DUMMY_VAL 78

int dummy1(void *arg) {
  printf("1");
  assert((int) arg == DUMMY_VAL);
  return 3;
}

int dummy2(void *arg) {
  printf(" 5");
  assert((int) arg == DUMMY_VAL + 1);
  return 4;
}

void test1() {
  int pid1;
  int r;
  int rval;

  pid1 = start(dummy1, "dummy1", 4000, 192, (void *) DUMMY_VAL);
  assert(pid1 > 0);
  printf(" 2");

  r = waitpid(pid1, &rval);
  assert(r == pid1);
  assert(rval == 3);
  printf(" 3");

  pid1 = start(dummy2, "dummy2", 4000, 100, (void *) (DUMMY_VAL + 1));
  assert(pid1 > 0);
  printf(" 4");

  r = waitpid(pid1, &rval);
  assert(r == pid1);
  assert(rval == 4);
  printf(" 6.\n");
}

//-----------------------------------------------------------------------------

int procExit(void *args) {
  printf(" 5");
  exitFunction((int) args);
  assert(0);
  return 0;
}

int procKill(void *args) {
  printf(" X");
  return (int)args;
}

void test2() {
  int rval;
  int r;
  int pid1;
  int val = 45;

  printf("1");
  pid1 = start(procKill, "procKill", 4000, 100, (void *) val);
  assert(pid1 > 0);
  printf(" 2");

  r = kill(pid1);
  assert(r == 0);
  printf(" 3");

  r = waitpid(pid1, &rval);
  assert(rval == 0);
  assert(r == pid1);
  printf(" 4");

  pid1 = start(procExit, "procExit", 4000, 192, (void *) val);
  assert(pid1 > 0);
  printf(" 6");

  r = waitpid(pid1, &rval);
  assert(rval == val);
  assert(r == pid1);
  assert(waitpid(getpid(), &rval) < 0);
  printf(" 7.\n");
}

//-----------------------------------------------------------------------------

int prio4(void *arg) {
  /* arg = priority of this proc. */
  int r;

  assert(getprio(getpid()) == (int) arg);
  printf("1");
  r = chprio(getpid(), 64);
  assert(r == (int) arg);
  printf(" 3");
  return 0;
}

int prio5(void *arg) {
  /* Arg = priority of this proc. */
  int r;

  assert(getprio(getpid()) == (int) arg);
  printf(" 7");
  r = chprio(getpid(), 64);
  assert(r == (int)arg);
  printf("error: I should have been killed\n");
  assert(0);
  return 0;
}

void test3() {
  int pid1;
  int p = 192;
  int r;

  assert(getprio(getpid()) == 128);
  pid1 = start(prio4, "prio4", 4000, p, (void *) p);
  assert(pid1 > 0);
  printf(" 2");
  r = chprio(getpid(), 32);
  assert(r == 128);
  printf(" 4");
  r = chprio(getpid(), 128);
  assert(r == 32);
  printf(" 5");
  assert(waitpid(pid1, 0) == pid1);
  printf(" 6");

  assert(getprio(getpid()) == 128);
  pid1 = start(prio5, "prio5", 4000, p, (void *) p);
  assert(pid1 > 0);
  printf(" 8");
  r = kill(pid1);
  assert(r == 0);
  assert(waitpid(pid1, 0) == pid1);
  printf(" 9");
  r = chprio(getpid(), 32);
  assert(r == 128);
  printf(" 10");
  r = chprio(getpid(), 128);
  assert(r == 32);
  printf(" 11.\n");
}

//-----------------------------------------------------------------------------
