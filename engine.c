#include "gtop.h"

//block of functions to manage sorting with qsort, these are passe by function pointer in main
//subfunction for cpu load
int compareCpu(const void* a, const void* b) {
    Process* p1 = (Process*)a;
    Process* p2 = (Process*)b;
    //check if task is active or unactive
    if(p1->isActive > p2->isActive) {
        return -1;
    }
    else if(p1->isActive < p2->isActive) {
        return 1;
    }
    else { //if they are both then order them by cpu load
        if(p1->cpuLoad > p2->cpuLoad) {
            return -1;
        }
        else if(p1->cpuLoad < p2->cpuLoad) {
            return 1;
        }
        else {
            return 0;
        }
    }
}
//subfunction for ram use
int compareRam(const void* a, const void* b) {
    Process* p1 = (Process*)a;
    Process* p2 = (Process*)b;
    //same as aboe but with ram
    if(p1->isActive > p2->isActive) {
        return -1;
    }
    else if(p1->isActive < p2->isActive) {
        return 1;
    }
    else {
        if(p1->ramLoad > p2->ramLoad) {
            return -1;
        }
        else if(p1->ramLoad < p2->ramLoad) {
            return 1;
        }
        else {
            return 0;
        }
    }
}
//subfunction for pid
int comparePid(const void* a, const void* b) {
    Process* p1 = (Process*)a;
    Process* p2 = (Process*)b;
    //same as above but with pid
    if(p1->isActive > p2->isActive) {
        return -1;
    }
    else if(p1->isActive < p2->isActive) {
        return 1;
    }
    else {
        if(p1->pid > p2->pid) {
            return -1;
        }
        else if(p1->pid < p2->pid) {
            return 1;
        }
        else {
            return 0;
        }
    }
}
//function to calculate cpu use by system
void calculateSystStats(unsigned long total_cpu_start, unsigned long total_cpu_end, unsigned long idle_cpu_start, unsigned long idle_cpu_end, unsigned long *delta_total_cpu, float *cpu_load) {    
*delta_total_cpu = total_cpu_end - total_cpu_start;
unsigned long delta_idle = idle_cpu_end - idle_cpu_start;
    if (*delta_total_cpu > 0) { //if system is too fast this avoids division by 0
        *cpu_load = (((float)(*delta_total_cpu - delta_idle)) / (float)(*delta_total_cpu)) * 100;
    }
}
//function to calculate cpu and ram use by tasks
void calculateProcStats(Process *list, int count, unsigned long *total_ram, unsigned long *delta_total_cpu) {
    for(int i = 0; i < count; i++) {
        //calculates delta and cpu use by task
        unsigned long current_time = list[i].timeUser + list[i].timeSyst;
        unsigned long delta_cpu = current_time - list[i].timeOld; //cpu load will be weird on first cycle but correct later on
        list[i].cpuLoad = (delta_cpu / (float)(*delta_total_cpu)) * 100.0;
        //calculates ram used by task
        list[i].ramLoad = ((list[i].rss) / (float)(*total_ram)) * 100.0;
    }
}
