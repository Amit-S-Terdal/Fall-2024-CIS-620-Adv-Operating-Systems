//File : SSemaphore.cxx
    /*
        SSemaphore implementation - Copyright (c)  2002  J. S. Gray
     */
    #include "SSemaphore.h"
                                          // Generate a private semaphore
    SSemaphore::SSemaphore(){
      zero.sem_num   = 0, zero.sem_op   =  0, zero.sem_flg   = SEM_UNDO;
      lock.sem_num   = 0, lock.sem_op   = -1, lock.sem_flg   = SEM_UNDO;
      unlock.sem_num = 0, unlock.sem_op =  1, unlock.sem_flg = SEM_UNDO;
      my_pid = getpid( );
      if((semid = semget( IPC_PRIVATE, 1, 0660 )) == -1 ){
          exit( 1 );
      }
      Put( 0 );                           // Default - set to zero @ start
    }
                                          // Remove semaphore if creator
    SSemaphore::~SSemaphore() {
      if ( getpid( ) == my_pid )
        if ( semctl( semid, 0, IPC_RMID ) == -1 )
          exit( 2 );
    }
                                         // LOCK semaphore
    int                                  // Atomic test & decrement
    SSemaphore::P( ){
      if ( semop( semid, &lock, 1 ) == -1 )
        exit( 3 );
      return 0;
    }
                                         // UNLOCK semaphore
    void                                 // Increment semaphore
    SSemaphore::V( ){
      if ( semop( semid, &unlock, 1 ) == -1 )
        exit( 4 );
    }

    int                                  // Wait for semaphore to be 0
    SSemaphore::Z( ){
      if ( semop( semid, &zero, 1 ) == -1 )
        exit( 5 );
      return 0;
    }
                                         // Assign value to the semaphore
    void
    SSemaphore::Put( int const value ){
      arg.val = value;
      if ( semctl(semid, 0, SETVAL, arg ) == -1 )
        exit( 6 );
    }
                                         // Return value of the semaphore
    int
    SSemaphore::Get( ){
      int sem_value;
      if ((sem_value=semctl(semid, 0, GETVAL, 0)) == -1 )
        exit( 7 );
      return sem_value;
    }