//
//  include.h
//  FishSwim
//
//  Created by Felix on 10/17/17.
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



//constants
const char water = '~', fish ='f';

const int mill_height = 10, mill_length = 10;

// 2-D array to simulate swim mill
char (*swim_mill)[mill_height][mill_length];

//create a key and a shared memory ID
const key_t key = 5678;
int sharedMemoryID;

//file for writing results into.
const FILE *fp;

void attachMemory()
{
    
        // Create shared memory ID segment
        if((sharedMemoryID = shmget(key, sizeof(swim_mill), 0666)) < 0)
        {
            perror("shmget");
            exit(1);
            
        }
    
    // Attach shared memory ID to data space
    if ((swim_mill = shmat(sharedMemoryID, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    
    
}





