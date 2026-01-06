#include <dirent.h>
#include <ctype.h>
#include "gtop.h"

//function to get all the processes
void getProcessList(Process *list, int *count) {
    //set all process to unactive
    for(int i = 0; i < *count; i++) {
        list[i].isActive = 0;
    }
    //declare the DIR and dirent types included in <dirent.h>
    DIR *dir;
    struct dirent *entry;
    //opens the /proc directory and exits with error if impossible
    dir = opendir("/proc");
    if(dir == NULL) {
        perror ("Error: /proc impossible to open.\n");
        exit(1);
    }
    //cycle to get all the processes
    while(((entry = readdir(dir)) != NULL) && *count < MAX_PROCESSES) {
        //if the firt element of entry is a number then we can read the directory
        if(isdigit(entry->d_name[0])) {
            //int pid = atoi(entry->d_name);
            int pid = searchPid(list, count, atoi(entry->d_name));
            if(pid == -1) { //process is new
                char stat_path[267], comm_path[267]; //gets rid of warning realting to snprintf outputs
                //gets the path to stat and opens it
                snprintf(stat_path, sizeof(stat_path), "/proc/%s/stat", entry->d_name);
                FILE *f_stat = fopen(stat_path, "r");
                if(f_stat != NULL) {
                    char state = '?';
                    unsigned long user_time = 0, syst_time = 0;
                    long rss = 0;
                    fscanf(f_stat, "%*d (%*[^)]) %c", &state); //skips to state, code avoids paranteses in name field
                    for (int i = 4; i <= 24; i++) {
                        if (i == 14) {
                            fscanf(f_stat, "%lu %lu", &user_time, &syst_time);
                            i++; 
                        }
                        else if (i == 24) {
                            fscanf(f_stat, "%ld", &rss);
                            if(rss < 0) { //some tasks use virtual memory and are recorded with rss -1 or 0
                                rss = 0;
                            }
                        }
                        else {
                            fscanf(f_stat, "%*s"); //skips value
                        }
                    }
                    fclose(f_stat);
                    //memorise the stats into the list
                    if(*count < MAX_PROCESSES) {
                        list[*count].pid = atoi(entry->d_name); //converts ascii to integer
                        list[*count].state = state;
                        list[*count].timeUser = user_time;
                        list[*count].timeSyst = syst_time;
                        list[*count].timeOld = 0;
                        list[*count].rss = rss * 4; //converts to kb
                        list[*count].isActive = 1; //mark process as active
                    }
                }
                //gets the path to comm and opens it to save command name
                snprintf(comm_path, sizeof(comm_path), "/proc/%s/comm", entry->d_name);
                FILE *f_comm = fopen(comm_path, "r");
                if(f_comm != NULL) {
                    if(fgets(list[*count].name, sizeof(list[*count].name), f_comm)) { //sizeof beacuse list is initialised to 0
                        size_t len = strlen (list[*count].name); //takes dimension of list[count].name
                        if(len > 0 && list[*count].name[len - 1] == '\n') { //check for \n (enter key) in last element
                            list[*count].name[len - 1] = '\0'; //input the end string
                        }
                    }
                    fclose(f_comm);
                }
                (*count)++;
            }
            else if(pid != -1) { //process is not new
                list[pid].timeOld = list[pid].timeUser + list[pid].timeSyst; //save old time since process is old
                char stat_path[267];
                snprintf(stat_path, sizeof(stat_path), "/proc/%s/stat", entry->d_name);
                FILE *f_stat = fopen(stat_path, "r");
                if(f_stat != NULL) {
                    char state = '?';
                    unsigned long user_time = 0, syst_time = 0;
                    long rss = 0;
                    fscanf(f_stat, "%*d (%*[^)]) %c", &state);
                    for (int i = 4; i <= 24; i++) {
                        if (i == 14) {
                            fscanf(f_stat, "%lu %lu", &user_time, &syst_time);
                            i++; 
                        }
                        else if (i == 24) {
                            fscanf(f_stat, "%ld", &rss);
                            if(rss < 0) {
                                rss = 0;
                            }
                        }
                        else {
                            fscanf(f_stat, "%*s");
                        }
                    }
                    fclose(f_stat);
                    //save new stats only
                    list[pid].state = state;
                    list[pid].timeUser = user_time;
                    list[pid].timeSyst = syst_time;
                    list[pid].rss = rss * 4; //converts to kb
                    list[pid].isActive = 1; //mark process as active
                }
            }
        }
    }
    closedir(dir);
}
//function to look for a pid in the array
int searchPid(Process *list, int *count, int pid) {
    for(int i = 0; i < *count; i++) {
        if(list[i].pid == pid) {
            return i;
        }
    }
    return -1; //if list is empty or pid is not found
}
//function to get uptime
void getUptime(long *uptime_seconds) {
    FILE *f_upt = fopen("/proc/uptime", "r");
    if (f_upt != NULL) {
        double total_seconds; //uptime is stored with high precision
        if (fscanf(f_upt, "%lf", &total_seconds) == 1) {
            *uptime_seconds = (long)total_seconds; //converts uptime from double to long
        }
        fclose(f_upt);
    }
}
//function to get total ram
void getRamTotal(unsigned long *total_ram) {
    FILE *f_mem = fopen("/proc/meminfo", "r");
    if(f_mem != NULL) {
        fscanf(f_mem, "MemTotal: %lu kB", total_ram);
        fclose(f_mem);
    }
}
//function to get cpu and ram loads
void getSystemStats(unsigned long *ram_usage, unsigned long *total_cpu, unsigned long *idle_cpu, unsigned long *total_ram) {
    //gets ram usage
    FILE *f_ram = fopen("/proc/meminfo", "r");
    if(f_ram != NULL) {
        char label[32];
        unsigned long value;
        while(fscanf(f_ram, "%s %lu kB", label, &value) != EOF) {
            if(strcmp(label, "MemAvailable:") == 0) {
            *ram_usage = (*total_ram) - value;
            break;
            }
        }
        fclose(f_ram);
    }
    //gets total cpu usage
    FILE *f_cpu = fopen("/proc/stat", "r");
    if(f_cpu != NULL) {
        char label[10];
        unsigned long u, n, s, id, iow, irq, soft;
        fscanf(f_cpu, "%s %lu %lu %lu %lu %lu %lu %lu", label, &u, &n, &s, &id, &iow, &irq, &soft);
        *total_cpu = u + n + s + id + iow + irq + soft;
        *idle_cpu = id;
        fclose(f_cpu);
    }
}