/*
    SERVER - receives messages from clients
*/

#include "local.h"
#include <iostream>
#include <cstdio>
#include <ctype.h>
#include <stdlib.h>
#include <unordered_set>

using namespace std;

unordered_set<pid_t> active_clients;

void process_msg(char *b, int len) {
    for (int i = 0; i < len; ++i) {
        if (isalpha(*(b + i))) {
            *(b + i) = toupper(*(b + i));
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " msq_id &" << endl;
        return 1;
    }

    int mid = atoi(argv[1]);
    MESSAGE msg;
    int n;

    while (true) {
        memset(msg.buffer, 0x0, BUFSIZ);

        if ((n = msgrcv(mid, &msg, BUFSIZ, SERVER, 0)) == -1) {
            perror("Server: msgrcv");
            return 2;
        }

        if (n == 0) {  // Client has exited
            pid_t client_pid = msg.msg_fm;
            active_clients.erase(client_pid);
            if (active_clients.empty()) {  // All clients are done
                break;
            }
            continue;
        }

        // Register the client if it's not already in the set
        active_clients.insert(msg.msg_fm);

        // Process and respond to the client
        process_msg(msg.buffer, strlen(msg.buffer));
        msg.msg_to = msg.msg_fm;
        msg.msg_fm = SERVER;
        n += sizeof(msg.msg_fm);

        if (msgsnd(mid, &msg, n, 0) == -1) {
            perror("Server: msgsnd");
            return 3;
        }
    }

    msgctl(mid, IPC_RMID, nullptr);  // Remove the message queue
    exit(0);
}
