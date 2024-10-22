/*
  A parent process that waits for a child to finish f24b
*/
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

int main(int argc, char *argv[] ){
  pid_t pid, w;
  int status;
  if ( argc < 2 ) {
    cerr << "Usage " << *argv << " int_value\n";
    return 1;
  }
  if ((pid = fork( )) == 0)
    execl("./child", "child", argv[1], (char *) 0);
  else // assuming no failures here
    cout << "Forked child " << pid << endl;
  /*
    Wait for the children
  */
  while ((w=wait(&status)) && w != -1)
    cout << "Wait on PID: " << dec << w << " returns status of "
         << setw(4) << setfill('0') << hex
         << setiosflags(ios::uppercase) << status << endl;
  return 0;
}