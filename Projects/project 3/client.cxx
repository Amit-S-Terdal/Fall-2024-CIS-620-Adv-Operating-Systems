/*
    CLIENT ... sends messages to the server
*/

#include "local.h"
#include <cstdio>
#include <unistd.h>
using namespace std;

int main() {
    key_t key;
    pid_t cli_pid = getpid();
    int mid, n;
    MESSAGE msg;
    static char m_key[10];

    if ((key = ftok(".", SEED)) == -1) {
        perror("Client: key generation");
        return 1;
    }

    if ((mid = msgget(key, 0)) == -1) {
        mid = msgget(key, IPC_CREAT | 0660);
        switch (fork()) {
            case -1:
                perror("Client: fork");
                return 2;
            case 0:
                sprintf(m_key, "%d", mid);
                execlp("./server", "server", m_key, "&", nullptr);
                perror("Client: exec");
                return 3;
        }
    }

    while (true) {
        msg.msg_to = SERVER;
        msg.msg_fm = cli_pid;

        write(fileno(stdout), "cmd> ", 6);
        memset(msg.buffer, 0x0, BUFSIZ);

        if ((n = read(fileno(stdin), msg.buffer, BUFSIZ)) == 0) {  // Exit on Ctrl+D
            msgsnd(mid, &msg, 0, 0);  // Send a zero-length message to signal exit
            break;
        }

        n += sizeof(msg.msg_fm);

        if (msgsnd(mid, &msg, n, 0) == -1) {
            perror("Client: msgsnd");
            return 4;
        }

        if ((n = msgrcv(mid, &msg, BUFSIZ, cli_pid, 0)) != -1) {
            write(fileno(stdout), msg.buffer, n);
        }
    }

    return 0;
}
