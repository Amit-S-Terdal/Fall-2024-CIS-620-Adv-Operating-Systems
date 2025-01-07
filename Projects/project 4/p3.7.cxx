#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h> // For getting the home directory

using namespace std;

const int MAX = 256;
const int CMD_MAX = 10;

// List of user-defined commands
const char *user_cmds = " pwd  lo  cd ";

// Function to check if the command is user-defined
bool is_user_command(const char *cmd) {
    char cmd_with_space[MAX];
    snprintf(cmd_with_space, sizeof(cmd_with_space), " %s ", cmd);
    return strstr(user_cmds, cmd_with_space) != NULL;
}

// Function to trim quotes and spaces from strings
void trim_quotes_and_spaces(char *str) {
    int len = strlen(str);
    if (len == 0) return;

    // Remove trailing spaces
    while (len > 0 && str[len - 1] == ' ') {
        str[--len] = '\0';
    }

    // Remove leading spaces
    char *start = str;
    while (*start == ' ') {
        start++;
    }

    if (start != str) {
        memmove(str, start, strlen(start) + 1);
        len = strlen(str);
    }

    // Remove surrounding quotes if present
    if ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len - 1] == '\'')) {
        str[len - 1] = '\0';
        memmove(str, str + 1, len - 1);
    }
}

// Function to parse the input command
void parse_input(char *line_input, char *the_cmd, char *new_args[]) {
    char *cp = line_input;
    int i = 0;

    // Get the command
    if ((new_args[i] = strtok(cp, " ")) != NULL) {
        strcpy(the_cmd, new_args[i]);
        
        if (strcmp(the_cmd, "cd") == 0) {
            // Capture the rest of the input as the directory path for 'cd'
            char *path = strtok(NULL, "\n");
            if (path != NULL) {
                trim_quotes_and_spaces(path);
                new_args[1] = path;
                new_args[2] = NULL;
            } else {
                new_args[1] = NULL; // Set to NULL if no path provided
            }
        } else {
            // Parse the remaining arguments normally
            while (i < CMD_MAX && (new_args[++i] = strtok(NULL, " ")) != NULL);
            new_args[i] = NULL;
        }
    }
}

// Function to handle user-defined commands
void handle_user_command(const char *the_cmd, char *new_args[]) {
    if (strcmp(the_cmd, "pwd") == 0) {
        char cwd[MAX];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            cout << cwd << endl;
        } else {
            perror("getcwd error");
        }
    } else if (strcmp(the_cmd, "lo") == 0) {
        cout << "Logging out...\n";
        exit(0);
    } else if (strcmp(the_cmd, "cd") == 0) {
        if (new_args[1] == NULL) {
            // Change to the home directory
            const char *home = getenv("HOME");
            if (!home) {
                home = getpwuid(getuid())->pw_dir; // Fallback to passwd entry
            }
            if (chdir(home) != 0) {
                perror("chdir error");
            }
        } else if (strcmp(new_args[1], "~") == 0) {
            // Handle ~ for home directory
            const char *home = getenv("HOME");
            if (!home) {
                home = getpwuid(getuid())->pw_dir; // Fallback to passwd entry
            }
            if (chdir(home) != 0) {
                perror("chdir error");
            }
        } else if (chdir(new_args[1]) != 0) {
            // Handle relative and absolute paths
            perror("chdir error");
        }
    }
}

// Function to handle system commands
void handle_system_command(char *new_args[]) {
    switch (fork()) {
    case 0:
        execvp(new_args[0], new_args);
        perror("exec failure");
        exit(1);
    case -1:
        perror("fork failure");
        exit(2);
    default:
        wait(NULL); // Wait for the child process to finish
    }
}

int main() {
    char line_input[MAX], the_cmd[CMD_MAX];
    char *new_args[CMD_MAX];

    while (1) {
        cout << "cmd> ";
        if (cin.getline(line_input, MAX)) {
            parse_input(line_input, the_cmd, new_args);

            if (is_user_command(the_cmd)) {
                handle_user_command(the_cmd, new_args);
            } else {
                handle_system_command(new_args);
            }
        }
    }
}
