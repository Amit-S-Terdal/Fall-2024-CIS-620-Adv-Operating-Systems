#ifndef SSemaphore_h
#define SSemaphore_h

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <iostream>

class SSemaphore {
public:
    SSemaphore(key_t key = IPC_PRIVATE, int initialValue = 0, bool isOwner = true);
    ~SSemaphore();
    
    int P();  // Lock (decrement semaphore)
    void V(); // Unlock (increment semaphore)
    void Put(int const value); // Assign a value to the semaphore
    int Get(); // Get the current value of the semaphore
    void remove(); // Explicitly remove the semaphore (producer only)

private:
    int semid; // Semaphore ID
    pid_t my_pid; // Process ID of the creator
    bool isOwner; // Indicates if this instance is responsible for cleanup

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short int *array;
        struct seminfo *__buf;
    } arg;
};

#endif
