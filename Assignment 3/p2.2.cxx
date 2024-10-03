#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <ctime>
#include <iomanip>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " file_name" << endl;
        return 1;
    }
    
    // File type
    struct stat buff;
    if (stat(argv[1], &buff) == -1) {
        perror(argv[1]);
        return 1;
    }

    // Permissions
    cout << ((buff.st_mode & S_IRUSR) ? 'r' : '-')  // User read permission
         << ((buff.st_mode & S_IWUSR) ? 'w' : '-')  // User write permission
         << ((buff.st_mode & S_IXUSR) ? 'x' : '-')  // User execute permission
         << ((buff.st_mode & S_IRGRP) ? 'r' : '-')  // Group read permission
         << ((buff.st_mode & S_IWGRP) ? 'w' : '-')  // Group write permission
         << ((buff.st_mode & S_IXGRP) ? 'x' : '-')  // Group execute permission
         << ((buff.st_mode & S_IROTH) ? 'r' : '-')  // Others read permission
         << ((buff.st_mode & S_IWOTH) ? 'w' : '-')  // Others write permission
         << ((buff.st_mode & S_IXOTH) ? 'x' : '-'); // Others execute permission


    // Number of links
    cout << ' ' << buff.st_nlink;

    // User name
    struct passwd *pw = getpwuid(buff.st_uid);
    cout << ' ' << (pw ? pw->pw_name : to_string(buff.st_uid));

    // Group name
    struct group *gr = getgrgid(buff.st_gid);
    cout << ' ' << (gr ? gr->gr_name : to_string(buff.st_gid));

    // File size
    cout << ' ' << buff.st_size;

    // Last modification time
    char timebuf[80];
    struct tm *tm_info = localtime(&buff.st_mtime);
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", tm_info);
    cout << ' ' << timebuf;

    // File name
    cout << ' ' << argv[1] << endl;

    return 0;
}
