#include <termios.h>
#include "gtop.h"

static struct termios cfgOrigin; //static variable to store original terminal configuration

//function to re-enable original configuration
void disableRawMode() {
      tcsetattr(STDIN_FILENO, TCSAFLUSH, &cfgOrigin); //enforce original configuration
}
//function to enable raw mode for handling input
void enableRawMode() {
    tcgetattr(STDIN_FILENO, &cfgOrigin); //save original configuration
    atexit(disableRawMode);

    struct termios cfgRaw = cfgOrigin; //copy original configuration
    cfgRaw.c_lflag &= ~(ECHO | ICANON); //ignore enter and hide input
    cfgRaw.c_cc[VMIN] = 0; //do not wait for any input
    cfgRaw.c_cc[VTIME] = 0; //do not wait before passing the input

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &cfgRaw); //enforce raw configuration
}
//function to print interface
void printInterface(Process *list, int count, float cpuLoad, unsigned long ramUsed, unsigned long ramTotal, long uptime) {
    int h = uptime / 3600;
    int m = (uptime % 3600) / 60;
    int s = uptime % 60;
    printf("\033[H\033[2J"); //clears the screen
    //system("clear"); //clears the screen
    //prints the header
    printf("GTOP v1.0 - %d total tasks | enter 1 2 3 to change sort or q to exit\n", count);
    printf("CPU load: [%.1f%%] | RAM load: %lu MB / %lu MB | uptime: %dH:%dM:%dS\n", cpuLoad, ramUsed / 1024, ramTotal / 1024, h, m, s);
    printf("----------------------------------------------------------------------\n");
    printf("%-8s %-32s %-8s %-10s %-10s\n", "PID", "COMMAND", "STATE", "CPU%", "MEM%");
    printf("----------------------------------------------------------------------\n");
    //prints the process list
    for (int i = 0; i < 25; i++) {
        printf("%-8d %-32.32s %-8c %-10.1f %-10.1f\n", list[i].pid, list[i].name, list[i].state, list[i].cpuLoad, list[i].ramLoad);
    }
}
