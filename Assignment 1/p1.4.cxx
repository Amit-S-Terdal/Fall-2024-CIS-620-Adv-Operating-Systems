/* File : p1.4.cxx */
/*
    Program 1.1 modified to display identifier addresses
*/        

#include <iostream>
#include <unistd.h>                      // needed for write
#include <cstring>                       // needed for strcpy
#include <cstdlib>                       // needed for exit
#include <cstdint>                       // needed for uintptr_t

using namespace std;

const char *cptr = "Hello World\n";    // static by placement
char buffer1[25];

inline void SHW_ADR(const char *ID, uintptr_t address) {
    cout << "The id " << ID << "\t is at : "
         << hex << address << endl;
}

int main() {
    void showit(const char *);           // function prototype
    int i = 0;                          // automatic variable

 // Display addresses
SHW_ADR("main", uintptr_t(main));   // function addresses
SHW_ADR("showit", uintptr_t(showit));
SHW_ADR("cptr", uintptr_t(&cptr));   // static
SHW_ADR("buffer1", uintptr_t(&buffer1));
SHW_ADR("i", uintptr_t(&i));         // automatic

    strcpy(buffer1, "A demonstration\n"); // library function
    write(1, buffer1, strlen(buffer1) + 1); // system call
    showit(cptr);                        // function call
    return 0;
}

void showit(const char *p) {            // Changed to accept const char*
    char *buffer2;
    SHW_ADR("buffer2", uintptr_t(&buffer2)); // display address

    if ((buffer2 = new char[strlen(p) + 1]) != NULL) {
        strcpy(buffer2, p);                 // copy the string
        cout << buffer2;                    // display string
        delete[] buffer2;                   // release location
    } else {
        cerr << "Allocation error.\n";
        exit(1);
    }
}
