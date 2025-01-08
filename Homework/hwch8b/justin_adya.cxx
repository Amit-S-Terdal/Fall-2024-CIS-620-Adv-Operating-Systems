#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include "SSemaphore.h"

using namespace std;

int main() {
    SSemaphore S;
    S.Put(1);

    key_t key = ftok("/tmp", 'b');
    int shmid = shmget(key, sizeof(char), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failure");
        return 1;
    }

    char* c = (char*)shmat(shmid, nullptr, 0);
    if (c == (char*)-1) {
        perror("shmat failure");
        return 1;
    }

    *c = 0;

    switch (fork()) {
        case -1:
            perror("fork failure");
            return 1;
        case 0:
            srand(getpid());
            for (int i = 0; i < 10; ++i) {
                S.P();
                cout << char(*c + 'a');
                cout.flush();
                ++(*c);
                S.V();
                sleep(rand() % 3 + 1);
            } break;
        default:
            srand(getpid());
            for (int i = 0; i < 10; ++i) {
                S.P();
                cout << char(*c + 'A');
                cout.flush();
                ++(*c);
                S.V();
                sleep(rand() % 3 + 1);
            }
            break;
    }
        wait(0);

    if (shmdt(c) == -1) {
        perror("shmdt failure");
        return 1;
    }

    if (shmctl(shmid, IPC_RMID, nullptr) == -1) {
    }

    cout << "\n";
    return 0;
}