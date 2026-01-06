#include <unistd.h>
#include "gtop.h"

int main() {
    Process *list = calloc(MAX_PROCESSES, sizeof(Process));
    int count = 0;
    long uptime;
    float cpuLoad;
    unsigned long ramUsed, cpuTotalStart, cpuTotalEnd, cpuIdleStart, cpuIdleEnd, cpuTotalDelta, ramTotal;
    getRamTotal(&ramTotal); //get total ram
    while(1){
        //system stats block
        getSystemStats(&ramUsed, &cpuTotalStart, &cpuIdleStart, &ramTotal);
        sleep(1); //waits a second in order to have delta (variation in time)
        getSystemStats(&ramUsed, &cpuTotalEnd, &cpuIdleEnd, &ramTotal);
        getUptime(&uptime);
        calculateSystStats(cpuTotalStart, cpuTotalEnd, cpuIdleStart, cpuIdleEnd, &cpuTotalDelta, &cpuLoad);
        //process stat block
        getProcessList(list, &count);
        calculateProcStats(list, count, &ramTotal, &cpuTotalDelta);
        qsort(list, count, sizeof(Process), compareCpu); //sorting through function pointer
        //prints system info and task list
        printInterface(list, count, cpuLoad, ramUsed, ramTotal, uptime);
    }
    free(list);
    return 0;
}