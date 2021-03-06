//
// Created by 余晖 on 2022/3/25.
//

#include <stdio.h>
#include <stdlib.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);
int see_more(FILE *);

int main(int argc, char const *argv[])
{
    FILE * fp;
    if (argc == 1){
        do_more(stdin);
    }else
        while (--argc)
            if ((fp = fopen(*++argv, "r")) != NULL)
            {
                do_more(fp);
                fclose(fp);
            } else
                exit(1);
    return 0;
}

void do_more(FILE *fp) {
    /*
     * read PAGELEN lines, then call see_more() for further instructions.
     */
    char line[LINELEN];
    int num_of_lines = 0;
    int see_more(FILE *),reply;
    FILE * fp_tty;
    fp_tty = fopen("/dev/tty", "r"); //CMD stream
    if (fp_tty == NULL)
        exit(1);
    while (fgets(line, LINELEN, fp)){
        if (num_of_lines == PAGELEN){
            reply = see_more(fp_tty);
            if (reply == 0)
                break;
            num_of_lines -= reply;
        }
        if (fputs(line, stdout) == EOF)
            exit(1);
        num_of_lines++;
    }
}

int see_more(FILE *cmd){
    int c;
    printf("\033[7m more? \033[m"); //reverse on a vt 100;
    while ((c = getc(cmd)) != EOF){ //从控制台获取按键
        if (c == 'q')
            return 0;
        if (c == ' ')
            return PAGELEN;
        if (c == '\n')
            return 1;
    }
    return 0;
}