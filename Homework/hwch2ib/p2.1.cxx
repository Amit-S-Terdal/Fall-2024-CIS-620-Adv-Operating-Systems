/*
               Displaying process group ID information
        */

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
using namespace std;

int main() {
    cout << "\n\nInitial process \t PID " << getpid()
         << "\t PPID " << getppid()
         << "\t GID " << getpgid(0) 
         << endl;

    for (int i = 0; i < 3; ++i) {
        if (fork() == 0) { // Child process
            // Set the process group ID to the child process's own PID
            setpgid(0, 0); // Set this process's PGID to its own PID (make it group leader)
            cout << "New process      \t PID " << getpid()
                 << "\t PPID " << getppid()
                 << "\t GID " << getpgid(0) 
                 << endl;
                 
        }
    }

    return 0;
}