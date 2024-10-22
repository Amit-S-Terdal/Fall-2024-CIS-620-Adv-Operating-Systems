/*
  The child process f24b
*/
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
using namespace std;

int main(int argc, char *argv[]) {
    pid_t pid = getpid();
    int ret_value;
    int sigNumber = 9; // Changed from 8 to 9 and vice versa
    srand((unsigned) pid);
    ret_value = int(rand() % 256); // generate a return value
    sleep(rand() % 3); // sleep a bit
    if (atoi(*(argv + 1)) % 2) { // assuming argv[1] exists!
        cout << "Child " << pid << " is terminating with signal " 
             << setw(4) << setfill('0') << hex
             << setiosflags(ios::uppercase) << sigNumber << endl;
        kill(pid, sigNumber); // commit hara-kiri
    } else {
        cout << "Child " << pid << " is terminating with exit("
             << setw(4) << setfill('0') << setiosflags(ios::uppercase)
             << hex << ret_value << ")" << endl;
        exit(ret_value);
    }
}
