    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include <sys/types.h>
    #include <unistd.h>
    #include <cstring>

    using namespace std;
    const int size = 4096;

    int main() {
        // Create a string to store the file path to /proc/<pid>/environ
        ostringstream oss (ostringstream::out);
        oss  << "/proc/" << getpid() << "/environ";  // Change from "cmdline" to "environ"
        cout << "Reading from file: " << oss.str() << endl;

        static char buffer[size];
        ifstream i_file;
        i_file.open(oss.str().c_str());  // Open the environ file for reading
        if (!i_file) {
            cerr << "Error: Could not open " << oss.str() << endl;
            return 1;
        }

        // Read the contents of the environ file
        i_file.read(buffer, size);

        // Start at the first character in the buffer
        char *p = &buffer[0];
        
        // Loop through the buffer, displaying each environment variable
        do {
            cout << "[" << p << "]" << endl;  // Print the current environment variable
            p += strlen(p) + 1;  // Move to the next environment variable (next null-separated section)
        } while (*p);  // Continue while there are valid characters left
        
        return 0;
    }

