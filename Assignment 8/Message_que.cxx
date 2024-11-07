/*
    Message queue implementation—Copyright (c) 2002 J. S. Gray
*/
#include "Message_que.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

// Message queue constructor.
Message_que::Message_que(const char the_key) {
    ipckey = ftok(".", the_key);
    msqid  = -1;
}

// Remove the message queue (if this process created it)
void Message_que::Remove() {
    if (msgctl(msqid, IPC_RMID, (struct msqid_ds *)0) == -1)
        exit(5);
}

// Place a message in the message queue.
void Message_que::Enque(void *msg, int msg_size) {
    if (msgsnd(msqid, msg, msg_size, 0) == -1)
        exit(3);
}

// Return a message from the message queue.
// 'non_destructive' controls whether the message should be removed or not.
int Message_que::Deque(void *msg, int msg_size, int msg_type, bool non_destructive) {
    int flags = non_destructive ? IPC_NOWAIT : 0;
    int n;
    memset(msg, 0x0, msg_size);  // Clear space
    if ((n = msgrcv(msqid, msg, msg_size, msg_type, flags)) == -1) {
        if (non_destructive && errno == ENOMSG) {
            return 0;  // No message found, return 0 for non-destructive
        }
        exit(4);
    }
    return n;
}

// True if message queue exists else false.
bool Message_que::Exist(const char the_key) {
    return (msgget(ipckey, 0) != -1);
}

// Generate a new message queue.
void Message_que::Create() {
    if ((msqid = msgget(ipckey, IPC_CREAT | 0660)) == -1)
        exit(1);
}

// Acquire (gain access to) existing message queue.
void Message_que::Acquire() {
    if ((msqid = msgget(ipckey, 0)) == -1)
        exit(2);
}



//output command 
//1) g++ -o 2887869 p6.6.2887869.cxx Message_que.cxx Message_que.h
//2)  ./2887869 -s -i 1 -m "Hello, World!" 
//3)  ./2887869 -s -i 1 -m "Hello, World1!"
//4)  ./2887869 -s -i 1 -m "Hello, World2!"
//  all these will add the messages to que
//5)  ./2887869 -r -i 1
//6)  ./2887869 -r -i 1
//7)  ./2887869 -r -i 1
//  all these will retive and re-enque them back 
//8)  ./2887869 -q 
// this will remove all the messages that are in the que