#include <time.h>

#define CLOCK_TYPE CLOCK_REALTIME

typedef struct timespec timespec;

timespec timeDiff(timespec start, timespec end) {
  timespec temp;
  if (end.tv_nsec < start.tv_nsec) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  }
  else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  return temp;
}

typedef struct {
  timespec start;
  timespec end;
  double ms;
} timer;

void startTimer(timer* t) {
  clock_gettime(CLOCK_TYPE, &(t->start));
}

void stopTimer(timer* t) {
  clock_gettime(CLOCK_TYPE, &(t->end));
  timespec diff = timeDiff(t->start, t->end);
  t->ms = (((double) diff.tv_sec) * 1000.0) + (((double) diff.tv_nsec) / 1000000.0);
}
