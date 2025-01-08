// To run the code use:
// g++ -o justin_2887869 justin_2887869.cxx SSemaphore.cxx -pthread
// ./justin_2887869

#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <mutex>
#include <condition_variable>
#include <thread> 

using namespace std;

#define SHM_KEY 12345  

mutex mtx;  // Mutex for thread synchronization
condition_variable cv;  // Condition variable for signaling threads
bool parent_turn = true;  // Flag to alternate between threads

// Function to print uppercase letters
void print_uppercase(int* counter) {
    for (int i = 0; i < 10; ++i) {  // Print 10 uppercase letters
        unique_lock<mutex> lock(mtx);  // Lock mutex for thread-safe printing
        cv.wait(lock, [] { return parent_turn; });  // Wait for the parent's turn

        // Print uppercase character
        cout << char(*counter + 'A');
        cout.flush();  // Ensure immediate output
        (*counter)++;  // Increment counter for next character
        parent_turn = false;  // Change turn to lowercase printing
        cv.notify_all();  // Notify the other thread to proceed
        usleep(100000);  // Sleep for a short period to control speed
    }
}

// Function to print lowercase letters
void print_lowercase(int* counter) {
    for (int i = 0; i < 10; ++i) {  // Print 10 lowercase letters
        unique_lock<mutex> lock(mtx);  // Lock mutex for thread-safe printing
        cv.wait(lock, [] { return !parent_turn; });  // Wait for lowercase turn

        // Print lowercase character
        cout << char(*counter + 'a');
        cout.flush();  // Ensure immediate output
        (*counter)++;  // Increment counter for next character
        parent_turn = true;  // Change turn to uppercase printing
        cv.notify_all();  // Notify the other thread to proceed
        usleep(100000);  // Sleep for a short period to control speed
    }
}

int main() {
    // Shared memory allocation to store the counter variable
    int shm_id = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget failed");
        return 1;
    }

    // Attach shared memory segment to the process's address space
    int* counter = (int*) shmat(shm_id, nullptr, 0);
    if (counter == (int*)-1) {
        perror("shmat failed");
        return 1;
    }

    // Initialize the counter to 0
    *counter = 0;

    // Create threads for printing uppercase and lowercase letters
    thread t1(print_uppercase, counter);
    thread t2(print_lowercase, counter);

    // Wait for both threads to finish execution
    t1.join();
    t2.join();

    // Add a new line to ensure the output is followed by a newline
    cout << endl;

    // Detach shared memory segment from the process
    shmdt(counter);

    // Remove the shared memory segment from the system
    shmctl(shm_id, IPC_RMID, nullptr);

    return 0;
}
