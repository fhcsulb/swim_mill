//
//  include.h
//  FishSwim
//
//  Created by Felix 
//  Copyright © 2017 Felix. All rights reserved.
//



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>



// set constant characters. water is represented by a '~' and fish is represented by a 'f'
const char water = '~', fish ='f';

// the mill height and mill length are constants. This will be a 10 x 10 grid.
const int mill_height = 10, mill_length = 10;

// create the 2-D array to simulate the mill grid.
char (*swim_mill)[mill_height][mill_length];

//create a key and a shared memory ID
const key_t key = 5678;
int sharedMemoryID;

//file for writing results into.
const FILE *fp;

void attachMemory()
{
    
        // Create shared memory ID segment
    
    // Create shared memory ID segment
    /*
     shmget is used to allocate a shared memory segment. shmget returns the identifier of the
     shared memory segment
     parameters: key to access shared memory segment
     size of the shared memory segment
     IPC_CREAT to create a new shared memory segment.
     */
    
        if((sharedMemoryID = shmget(key, sizeof(swim_mill), 0666)) < 0)
        {
            perror("shmget");
            exit(1);
            
        }
    
    // Attach shared memory ID to data space
    
    /*
     shmat is used to attach the shared memory segment with the shared memory identifier
     specified by sharedmemID.
     
     if shmat returns -1, then there was an error attaching shared memory.
     
     */
    
    if ((swim_mill = shmat(sharedMemoryID, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    
    
}





