#include "gtop.h"

//function to print interface
void printInterface(Process *list, int count, float cpuLoad, unsigned long ramUsed, unsigned long ramTotal, long uptime) {
    int h = uptime / 3600;
    int m = (uptime % 3600) / 60;
    int s = uptime % 60;
    system("clear"); //clears the screen
    //prints the header
    printf("GTOP v1.0 - %d total tasks\n", count);
    printf("CPU load: [%.1f%%] | RAM load: %lu MB / %lu MB | uptime: %dH:%dM:%dS\n", cpuLoad, ramUsed / 1024, ramTotal / 1024, h, m, s);
    printf("----------------------------------------------------------------------\n");
    printf("%-8s %-32s %-8s %-10s %-10s\n", "PID", "COMMAND", "STATE", "CPU%", "MEM%");
    printf("----------------------------------------------------------------------\n");
    //prints the process list
    for (int i = 0; i < 25; i++) {
        printf("%-8d %-32.32s %-8c %-10.1f %-10.1f\n", list[i].pid, list[i].name, list[i].state, list[i].cpuLoad, list[i].ramLoad);
    }
}
