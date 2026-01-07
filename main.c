#include "gtop.h"

int main() {
    Process *list = calloc(MAX_PROCESSES, sizeof(Process)); //creates list and allocates cleaned memory
    int (*sortMode) (const void*, const void*) = compareCpu; //function pointer to direct sorting by user input
    int count = 0, execute = 1;
    long uptime;
    float cpuLoad;
    unsigned long ramUsed, cpuTotalStart, cpuTotalEnd, cpuIdleStart, cpuIdleEnd, cpuTotalDelta, ramTotal;
    enableRawMode(); //enables the raw mode, prepares to disable it as soon as programm exits
    getRamTotal(&ramTotal); //gets total ram
    while(execute == 1){
        //system stats block
        getSystemStats(&ramUsed, &cpuTotalStart, &cpuIdleStart, &ramTotal);
        sleep(1); //waits a second in order to have delta (variation in time)
        getSystemStats(&ramUsed, &cpuTotalEnd, &cpuIdleEnd, &ramTotal);
        getUptime(&uptime);
        calculateSystStats(cpuTotalStart, cpuTotalEnd, cpuIdleStart, cpuIdleEnd, &cpuTotalDelta, &cpuLoad);
        //process stat block
        getProcessList(list, &count);
        calculateProcStats(list, count, &ramTotal, &cpuTotalDelta);
        qsort(list, count, sizeof(Process), sortMode); //sorting through function pointer
        //prints system info and task list
        printInterface(list, count, cpuLoad, ramUsed, ramTotal, uptime);
        char input = '\0';
        if(read(STDIN_FILENO, &input, 1) == 1) { //checks for input and switches sorting function
            switch(input) {
            case '1': 
                sortMode = compareCpu; //descending order by cpu load
                break;
            case '2':
                sortMode = compareRam; //descending order by ram load
                break;
            case '3':
                sortMode = comparePid; //ascending order by pid
                break;
            case 'q':
                execute = 0; //exit the program
                break;
            default:
                break;
            }
        }
    }
    free(list); //frees memory allocated through calloc
    return 0;
}