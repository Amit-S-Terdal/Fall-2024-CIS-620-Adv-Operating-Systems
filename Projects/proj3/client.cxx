#include "local.h"
#include <iostream>
#include <string>
#include <csignal>
#include <unistd.h>

using namespace std;

int msgid;

void disconnect_and_exit() {
    MESSAGE msg;
    msg.msg_to = SERVER;
    msg.msg_fm = getpid();
    strcpy(msg.buffer, "DISCONNECT");
    msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0); // Notify server of disconnection
    cout << "Client: Disconnected from server." << endl;
}

int main() {
    key_t key = ftok(".", SEED);
    if (key == -1) {
        perror("Client: key generation failed");
        return 1;
    }

    msgid = msgget(key, IPC_CREAT | 0660);
    if (msgid == -1) {
        perror("Client: msgget failed");
        return 1;
    }

    // Notify server of new connection
    MESSAGE msg;
    msg.msg_to = SERVER;
    msg.msg_fm = getpid();
    strcpy(msg.buffer, "CONNECT");
    if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
        perror("Client: msgsnd failed (CONNECT)");
        return 1;
    }

    signal(SIGINT, [](int) { disconnect_and_exit(); exit(0); });

    // Client main loop
    string input;
    while (true) {
        cout << "cmd> ";
        if (!getline(cin, input)) {
            disconnect_and_exit();
            break;
        }

        msg.msg_to = SERVER;
        msg.msg_fm = getpid();
        strncpy(msg.buffer, input.c_str(), 511);
        msg.buffer[511] = '\0';

        if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
            perror("Client: msgsnd failed");
            continue;
        }

        // Receive server response
        if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), getpid(), 0) == -1) {
            perror("Client: msgrcv failed");
            continue;
        }

        cout << "Response: " << msg.buffer << endl;
    }

    return 0;
}
