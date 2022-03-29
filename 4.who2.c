/*
 * who1.c - a first version of the who program
 *      open, read UTMP file, and show result
 *
 *  UTMP: the utmp file records information about who is currently using the system.
 *
 *  using "man -k utmp"
 */
#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>  // open()
#include <unistd.h> // read() close()
#include <time.h>
#include <string.h>


#define SHOWHOST    /*include remote machine on output*/
typedef struct utmp utmp;

void show_info(utmp *p_utmp);

void showtime(time_t *timeval);

int main(int argc, char const *argv[]) {
    utmp current_record;
    int utmp_fd, record_len = sizeof(current_record);

    if ((utmp_fd = open(UTMP_FILE, O_RDONLY)) == -1) {
        perror("open utmp file");
        exit(1);
    }
    while (read(utmp_fd, &current_record, record_len) == record_len) {
        show_info(&current_record);
    }
    close(utmp_fd);
    return 0;
}

/*
 * show_info()
 * displays contents of the utmp struct in human readable from
 * "note" these sizes should not be hardwired.
 */
/*
 * Definitions for ut_type at utmp.h
 *
 * #define EMPTY            0
 * #define RUN_LVL          1
 * #define BOOT_TIME        2
 * #define OLD_TIME         3
 * #define NEW_TIME         4
 * #define INIT_PROCESS     5   Process spawned by "init"
 * #define LOGIN_PROCESS    6   A "getty" process waiting for login
 * #define USER_PROCESS     7   A User process
 * #define DEAD_PROCESS     8
 */
void show_info(utmp *p_utmp) {
    if (p_utmp->ut_type != USER_PROCESS)
        return;
    printf("%-8.8s", p_utmp->ut_name);    /* the logname */
    printf("\t");    /* a space */
    printf("%-8.8s", p_utmp->ut_line);    /* the tty */
    printf("\t");    /* a space */
/*    char *ct = ctime(&p_utmp->ut_time);
    char *buf[20];
    memcpy(buf, ct, 20);*/
    //printf("%s", buf);    /* login time */
    showtime(&p_utmp->ut_time);
    printf("\t");   /* a space */
#ifdef SHOWHOST
    printf("%s", p_utmp->ut_host);
#endif
    printf("\n");   /* new line */
}

/*
 * displays time in a format fit for human consumption
 * uses ctime to build a string then picks parts out of it
 * Note: %12.12s prints a string 12 chars wide and LIMITS.  it to 12chars.
 */
void showtime(time_t *timeval) {
    char *cp;   /* to hold address of time */
    cp = ctime(timeval);    /* convert time to string */

    printf("%12.12s", cp + 4);

}

