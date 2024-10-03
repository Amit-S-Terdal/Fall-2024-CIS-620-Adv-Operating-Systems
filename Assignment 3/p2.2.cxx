/*
        Using the stat system call
*/
#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>  // For getpwuid()
#include <grp.h>  // For getgrgid()
#include <ctime>  // For time formatting
#include <iomanip>
using namespace std;

const int N_BITS = 3;

int main(int argc, char *argv[]) {
    unsigned int mask = 0700;
    struct stat buff;
    static const char *perm[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};

    if (argc > 1) {
        if ((stat(argv[1], &buff) != -1)) {
            
            // Display permissions
            cout << ((S_ISDIR(buff.st_mode)) ? 'd' : '-');  // Directory or file
            for (int i = 3; i; --i) {
                cout << perm[(buff.st_mode & mask) >> (i - 1) * N_BITS];
                mask >>= N_BITS;
            }

            // Number of links
            cout << " " << buff.st_nlink;

            // Username
            struct passwd *pw = getpwuid(buff.st_uid);
            if (pw) {
                cout << " " << pw->pw_name;
            } else {
                cout << " " << buff.st_uid;
            }

            // Group name
            struct group *gr = getgrgid(buff.st_gid);
            if (gr) {
                cout << " " << gr->gr_name;
            } else {
                cout << " " << buff.st_gid;
            }

            // File size
            cout << " " << buff.st_size;

            // Last modification time
            struct tm *timeinfo = localtime(&buff.st_mtime);
            char date[20];
            strftime(date, sizeof(date), "%b %d %H:%M", timeinfo);
            cout << " " << date;

            // File name
            cout << " " << argv[1] << endl;
        } else {
            perror(argv[1]);
            return 1;
        }
    } else {
        cerr << "Usage: " << argv[0] << " file_name\n";
        return 2;
    }

    return 0;
}