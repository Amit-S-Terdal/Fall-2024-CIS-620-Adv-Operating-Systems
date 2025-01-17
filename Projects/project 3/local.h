/*
    Common header file for 2nd Message Queue Example
*/

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

const char SEED = 'M';            // Common seed for ftok
const long int SERVER = 1L;       // Message type for server

typedef struct {
    long int msg_to;              // Message in queue for this type
    long int msg_fm;              // Placed in the queue by this type
    char buffer[BUFSIZ];          // The actual message
} MESSAGE;

using namespace std;
