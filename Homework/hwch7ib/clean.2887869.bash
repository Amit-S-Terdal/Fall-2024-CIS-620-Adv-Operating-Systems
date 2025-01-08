#!/bin/bash
#
# Korn Shell script to manage IPC resources based on command-line options
#

# Usage function to display correct usage
usage() {
  echo "Usage: $0 {-q | -s | -m | -a}"
  echo "   -q : Remove message queues"
  echo "   -s : Remove semaphores"
  echo "   -m : Remove shared memory segments"
  echo "   -a : Remove all IPC facilities"
  exit 1
}

# Warning if the script is run by root
if [ "$(id -u)" -eq 0 ]; then
  echo "Warning: Running this script as root may have system-wide effects."
  echo "Do you want to proceed? (y/n)"
  read answer
  if [ "$answer" != "y" ]; then
    echo "Operation canceled."
    exit 0
  fi
fi

# Check for exactly one argument
if [ "$#" -ne 1 ]; then
  usage
fi

# Execute based on the option provided
case "$1" in
  -q)
    echo "Removing all message queues for $USER..."
    list=$(ipcs -q | grep "$USER" | awk '{print $2}')
    count=0
    for queue in $list; do
      ipcrm -q "$queue" > /dev/null 2>&1
      ((count=count+1))
    done
    echo "$count message queue(s) removed for $USER."
    ;;
  -s)
    echo "Removing all semaphores for $USER..."
    list=$(ipcs -s | grep "$USER" | awk '{print $2}')
    count=0
    for semaphore in $list; do
      ipcrm -s "$semaphore" > /dev/null 2>&1
      ((count=count+1))
    done
    echo "$count semaphore(s) removed for $USER."
    ;;
  -m)
    echo "Removing all shared memory segments for $USER..."
    list=$(ipcs -m | grep "$USER" | awk '{print $2}')
    count=0
    for memory in $list; do
      ipcrm -m "$memory" > /dev/null 2>&1
      ((count=count+1))
    done
    echo "$count shared memory segment(s) removed for $USER."
    ;;
  -a)
    echo "Removing all IPC facilities (message queues, semaphores, shared memory segments) for $USER..."
    
    # Counters for each IPC type
    mq_count=0
    sem_count=0
    shm_count=0

    # Remove message queues
    list=$(ipcs -q | grep "$USER" | awk '{print $2}')
    for queue in $list; do
      ipcrm -q "$queue" > /dev/null 2>&1
      ((mq_count=mq_count+1))
    done

    # Remove semaphores
    list=$(ipcs -s | grep "$USER" | awk '{print $2}')
    for semaphore in $list; do
      ipcrm -s "$semaphore" > /dev/null 2>&1
      ((sem_count=sem_count+1))
    done

    # Remove shared memory segments
    list=$(ipcs -m | grep "$USER" | awk '{print $2}')
    for memory in $list; do
      ipcrm -m "$memory" > /dev/null 2>&1
      ((shm_count=shm_count+1))
    done

    # Display summary
    echo "$sem_count semaphore(s) for $USER removed"
    echo "$mq_count message queue(s) for $USER removed"
    echo "$shm_count shared memory segment(s) for $USER removed"
    ;;
  *)
    usage
    ;;
esac

echo "Operation completed."
