#include <time.h>

#define CLOCK_TYPE CLOCK_REALTIME

typedef struct timespec timespec;

//timeDiff function based on post by user "janneb" on Stack Overflow:
//http://stackoverflow.com/questions/13950290/clock-gettime-nanoseconds-calculation

//Calculate the time difference between start and end
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

//Set the start variable of timer t to the current time
void startTimer(timer* t) {
  clock_gettime(CLOCK_TYPE, &(t->start));
}

//Set the end variable of timer to the current time
void stopTimer(timer* t) {
  clock_gettime(CLOCK_TYPE, &(t->end));
  timespec diff = timeDiff(t->start, t->end);
  t->ms = (((double) diff.tv_sec) * 1000.0) + (((double) diff.tv_nsec) / 1000000.0);
}
