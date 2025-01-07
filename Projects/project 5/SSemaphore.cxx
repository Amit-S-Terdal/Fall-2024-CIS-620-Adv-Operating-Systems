#include "SSemaphore.h"
#include <iostream>
#include <cstdlib>

SSemaphore::SSemaphore(key_t key, int initialValue, bool owner) : isOwner(owner) {
    my_pid = getpid();

    if ((semid = semget(key, 1, IPC_CREAT | 0666)) == -1) {
        std::cerr << "Failed to create semaphore" << std::endl;
        exit(1);
    }

    if (isOwner) {
        arg.val = initialValue;
        if (semctl(semid, 0, SETVAL, arg) == -1) {
            std::cerr << "Failed to initialize semaphore" << std::endl;
            exit(1);
        }
    }
}

SSemaphore::~SSemaphore() {
    // Empty destructor to prevent automatic cleanup.
}

void SSemaphore::remove() {
    if (isOwner && semctl(semid, 0, IPC_RMID) == -1) {
        std::cerr << "Failed to remove semaphore" << std::endl;
    }
}

int SSemaphore::P() {
    struct sembuf p = {0, -1, SEM_UNDO};
    return semop(semid, &p, 1);
}

void SSemaphore::V() {
    struct sembuf v = {0, 1, SEM_UNDO};
    semop(semid, &v, 1);
}

void SSemaphore::Put(int const value) {
    arg.val = value;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        std::cerr << "Failed to set semaphore value" << std::endl;
    }
}

int SSemaphore::Get() {
    return semctl(semid, 0, GETVAL);
}
