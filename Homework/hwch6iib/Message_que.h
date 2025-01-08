

/*
    A VERY simplified message queue class for use in a std UNIX
  |       environment.  See the text for instructions on how to use
  |       this class.  Copyright (c) 2002  J. S. Gray
  +
  |       Exit codes for class operations:
  |
  |       1 - Unable to create queue    2 - Cannot access queue
  |       3 - Enque has failed          4 - Deque has failed
 10       5 - Unable to remove queue
  |     */
#ifndef Message_que_h
#define Message_que_h
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
class Message_que {
  public:
  Message_que (const char ='M');    // Constructor
    void Remove( );                   // Remove the queue
    void Enque( void *, int );        // Place a message in the queue
    int  Deque( void *, int, int, bool );   // Obtain a message from queue
    bool Exist( const char ='M' );    // True if the queue exists
    void Create(  );                  // Create the queue
    void Acquire(  );                 // Acquire access to the queue
  private:
    int      msqid;                   // ID of message queue
    key_t    ipckey;                  // Key from ftok
};
#endif