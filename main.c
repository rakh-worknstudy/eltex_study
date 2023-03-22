#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

//        P
//       / \ 
//      0   1
//     / \   \ 
//    2   3   4

int main(void) {
    pid_t children_pid[5];
    pid_t child_pid;
    int temp;
    /* CREATE [0] */
    child_pid = fork();
    /* IF IN [0] */
    if (child_pid == 0) {
        /* CREATE [2] */
        child_pid = fork();
        /* IF IN [0] */
        if (child_pid > 0) {
            children_pid[2] = child_pid;
            /* CREATE [3] */
            child_pid = fork();
            /* IF IN [0] */
            if (child_pid > 0) {
                children_pid[3] = child_pid;
                wait(&temp);
            } else {
                sleep(20);
            }
        /* IF IN [2] */
        } else {
            sleep(20);
        }
    /* IF IN [P] */
    } else {
        children_pid[0] = child_pid;
        /* CREATE [1] */
        child_pid = fork();
        /* IF IN [1] */
        if (child_pid == 0) {
            /* CREATE [4] */
            child_pid = fork();
            /* IF IN [1] */
            if (child_pid > 0) {
                children_pid[4] = child_pid;
                wait(&temp);
            /* IF IN [4] */
            } else {
                sleep(20);
            }
        /* IF IN [P] */
        } else {
            children_pid[1] = child_pid;
            wait(&temp);
        }
    }
}