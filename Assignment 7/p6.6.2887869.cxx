#include "Message_que.h"
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>

using namespace std;

typedef struct {
    long int m_type;
    char m_text[1024];
} MESSAGE;

extern char *optarg;
extern int  optind, opterr, optopt;

int main(int argc, char *argv[]) {
    int c;
    char optstring[] = "sri:m:q"; 
    opterr = 0;
    bool snd_msg = false, get_msg = false, rmv_que = false;
    MESSAGE my_msg;
    memset(my_msg.m_text, 0x0, 1024);
  
    Message_que MQ('M');
    if (!MQ.Exist('M'))
        MQ.Create();
    else
        MQ.Acquire();
  
    while ((c = getopt(argc, argv, optstring)) != -1) {
        switch (c) {
            case 's':
                snd_msg = true;
                break;
            case 'r':
                get_msg = true;
                break;
            case 'i':
                my_msg.m_type = atol(optarg);
                break;
            case 'm':
                strcpy(my_msg.m_text, optarg);
                break;
            case 'q':  // Remove message queue
                rmv_que = true;
                break;
            default:
                cerr << "Invalid option: -" << (char)optopt << endl;
                return 1;
        }
    }

    if (snd_msg && my_msg.m_type > 0) {
        MQ.Enque(&my_msg, strlen(my_msg.m_text) + 1);
        cerr << "Added  : " << my_msg.m_text << endl;
    } else if (get_msg && my_msg.m_type > 0) {
        MESSAGE temp_msg;  // Temporary message buffer
        int result = MQ.Deque(&temp_msg, sizeof(temp_msg.m_text), my_msg.m_type, true);
        
        if (result > 0) {
            cerr << "Message: " << temp_msg.m_text << endl;
            // Re-enqueue the message
            MQ.Enque(&temp_msg, strlen(temp_msg.m_text) + 1);
            cerr << "Re-enqueued message: " << temp_msg.m_text << endl;
        } else {
            cerr << "No message found." << endl;
        }
    } else if (rmv_que) {
        MQ.Remove();
        cerr << "Message queue removed." << endl;
    } else {
        cerr << "Invalid command line option(s)" << endl;
    }
    
    return 0;
}
