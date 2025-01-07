#include "local.h"
#include <iostream>
#include <unordered_set>
#include <cstdlib>
#include <csignal>

using namespace std;

int msgid;  // Message queue ID
unordered_set<long> active_clients;

void cleanup_and_exit(int signum) {
    if (msgctl(msgid, IPC_RMID, nullptr) == -1) {
        perror("Server: msgctl (remove message queue)");
    }
    cout << "Server: Message queue removed. Exiting." << endl;
    exit(0);
}

void process_msg(char *buffer, int len) {
    for (int i = 0; i < len; ++i) {
        if (isalpha(buffer[i])) {
            buffer[i] = toupper(buffer[i]);
        }
    }
}

int main() {
    // Create or get message queue
    key_t key = ftok(".", SEED);
    if (key == -1) {
        perror("Server: key generation failed");
        return 1;
    }

    msgid = msgget(key, IPC_CREAT | 0660);
    if (msgid == -1) {
        perror("Server: msgget failed");
        return 1;
    }

    signal(SIGINT, cleanup_and_exit); // Handle interruption for cleanup

    MESSAGE msg;
    cout << "Server: Started, waiting for messages..." << endl;

    while (true) {
        if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), SERVER, 0) == -1) {
            perror("Server: msgrcv failed");
            cleanup_and_exit(0);
        }

        // Handle client connect and disconnect
        string msg_content(msg.buffer);
        if (msg_content == "CONNECT") {
            active_clients.insert(msg.msg_fm);
            cout << "Server: Client " << msg.msg_fm << " connected. Active clients: " << active_clients.size() << endl;
            continue;
        } else if (msg_content == "DISCONNECT") {
            active_clients.erase(msg.msg_fm);
            cout << "Server: Client " << msg.msg_fm << " disconnected. Active clients: " << active_clients.size() << endl;
            if (active_clients.empty()) {
                cleanup_and_exit(0); // Clean up and exit if no clients remain
            }
            continue;
        }

        // Process and respond to the message
        process_msg(msg.buffer, strlen(msg.buffer));
        msg.msg_to = msg.msg_fm; // Send response back to the client
        msg.msg_fm = SERVER;

        if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
            perror("Server: msgsnd failed");
        }
    }

    return 0;
}
