#include "SSemaphore.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

#define BUFFER "./buffer"
#define END_SIGNAL -1 // Special value to indicate end of production

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << argv[0] << " sleep_time" << std::endl;
        return 1;
    }

    int sleep_time = atoi(argv[1]);
    key_t keyMade = ftok(".", 'M');
    key_t keyRead = ftok(".", 'R');

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 2;
    } else if (pid == 0) { // Consumer Process
        std::cout << "Consumer process started\n";
        SSemaphore semaphoreMade(keyMade, 0, false); // Consumer is not owner
        SSemaphore semaphoreRead(keyRead, 1, false); // Consumer is not owner

        int lastConsumedNumber = 0; // Track last number read by consumer
        while (true) {
            std::cout << "Consumer waiting for producer...\n";
            semaphoreMade.P();  // Wait for producer

            FILE *fptr = fopen(BUFFER, "r");
            if (!fptr) {
                perror("fopen failed");
                return 3;
            }
            int number;
            fscanf(fptr, "%d", &number);
            fclose(fptr);

            // Check for end signal
            if (number == END_SIGNAL) {
                std::cout << "Consumer detected end of production. Exiting...\n";
                break; // Break out of the loop and exit the consumer process
            }

            lastConsumedNumber = number; // Save last read number
            std::cout << "Consumer read number: " << number << std::endl;
            semaphoreRead.V();  // Signal producer
        }

        // After the loop, write the last consumed number before signaling end of production
        FILE *fptr = fopen(BUFFER, "w");
        if (fptr) {
            fprintf(fptr, "%d\n", lastConsumedNumber); // Write the last consumed number
            fclose(fptr);
        }

        return 0; // Ensure consumer exits fully
    } else { // Producer Process
        std::cout << "Producer process started\n";
        SSemaphore semaphoreMade(keyMade, 0, true); // Producer is owner
        SSemaphore semaphoreRead(keyRead, 1, true); // Producer is owner

        srand(getpid());
        for (int i = 0; i < 10; i++) {
            sleep(sleep_time);
            int number = rand() % 100;
            std::cout << "Producer generated number: " << number << std::endl;

            std::cout << "Producer waiting for consumer...\n";
            semaphoreRead.P();  // Wait for consumer

            FILE *fptr = fopen(BUFFER, "w");
            if (!fptr) {
                perror("fopen failed");
                return 4;
            }
            fprintf(fptr, "%d\n", number);
            fclose(fptr);

            std::cout << "Producer wrote number: " << number << std::endl;
            semaphoreMade.V();  // Signal consumer
        }

        // Signal end of production
        semaphoreRead.P(); // Ensure consumer reads last value

        FILE *fptr = fopen(BUFFER, "w");
        if (fptr) {
            fprintf(fptr, "%d\n", END_SIGNAL); // Write end signal to buffer
            fclose(fptr);
        }

        semaphoreMade.V(); // Signal consumer to read end signal

        // Clean up (only producer calls remove)
        std::cout << "Producer cleaning up semaphores\n";
        semaphoreMade.remove();
        semaphoreRead.remove();
        std::cout << "Semaphores removed\n";
    }
    return 0;
}
