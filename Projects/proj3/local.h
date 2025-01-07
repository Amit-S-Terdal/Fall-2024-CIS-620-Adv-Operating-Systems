#ifndef LOCAL_H
#define LOCAL_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>

#define SEED 'M'             // Seed for generating message queue key
#define SERVER 1L            // Server message type

struct MESSAGE {
    long msg_to;             // Type of message (e.g., SERVER or client PID)
    long msg_fm;             // Sender type, use long instead of pid_t
    char buffer[512];        // Reduced to 512 bytes to avoid size issues
};

#endif

