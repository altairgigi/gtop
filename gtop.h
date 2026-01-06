#ifndef GTOP_H 
#define GTOP_H 
//libraries necessary to all the files
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//constant for max processes
#define MAX_PROCESSES 4096
//single process struct
typedef struct {
    int pid;
    char name[256];
    char state;
    unsigned long timeUser;
    unsigned long timeSyst;
    unsigned long timeOld;
    long rss;
    float ramLoad;
    float cpuLoad;
    int isActive;
} Process;
//prototypes for fetcher.c
void getProcessList(Process *list, int* count);
int searchPid(Process *list, int *count, int pid);
void getUptime(long *uptime);
void getRamTotal(unsigned long* ramTotal);
void getSystemStats(unsigned long* ramUsed, unsigned long* cpuTotal, unsigned long* cpuIdle, unsigned long* ramTotal);
//prototypes for engine.c 
int compareCpu(const void* a, const void* b);
int compareRam(const void* a, const void* b);
int comparePid(const void* a, const void* b);
void calculateSystStats(unsigned long cpuTotalStart, unsigned long cpuTotalEnd, unsigned long cpuIdleStart, unsigned long cpuIdleEnd, unsigned long *cpuTotalDelta, float *cpuLoad);
void calculateProcStats(Process *list, int count, unsigned long *ramTotal, unsigned long *cpuTotalDelta);
//prototypes for display.c
void disableRawMode();
void enableRawMode();
void printInterface(Process *list, int count, float cpuLoad, unsigned long ramUsed, unsigned long ramTotal, long uptime);

#endif