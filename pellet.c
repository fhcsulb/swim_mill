//
//  pellet.c
//  FishSwim
//
//  Created by Felix 
//  Copyright © 2017 Felix. All rights reserved.
//

#include "include.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void *pellet_thread(int *);
void endProcess();



int main()
{
    printf("PID %d pellet executing\n", getpid());
    
    signal(SIGUSR1, endProcess);
    
    attachMemory();
    printf("Pellet memory has been attachd ! \n");
    
    srand(time(NULL));
    
    
    pthread_t pellet[10];
    
    // Create 10 random threads
    for(int i = 0; i < 10; i++){
        int position[2] = {0, rand()%10};// Generates the pellet position
        pthread_create(&pellet[i], NULL, pellet_thread, position);
        sleep(rand()%3+2);
    }
    // Joins all threads
    for(int i = 0; i < 10; i++){
        pthread_join(pellet[i], NULL);
    }
    
   
}

void *pellet_thread(int *loc)
{
    int x = *loc;
    int y = *(loc+1);
    
    //drop pellet
    (*swim_mill)[x][y] = 'p';
    
    // move the pellet down the river
    while(x < (mill_height-1)) {
        sleep(1);
        // update previous pellet location
        (*swim_mill)[x][y] = water;
        
        //move pellet down river
        x++;
        if((*swim_mill)[x][y] != fish) {
            (*swim_mill)[x][y] = 'p';
        }
    }
    
    sleep(1);
    if((*swim_mill)[x][y] != fish) {
        (*swim_mill)[x][y] = water;
    }
    
    return 0;
}

// Kill and end process
// Detach shared memory
void endProcess(){
    printf("\nPellet killed because time limit reached. PID %d\n", getpid());
    
    fp = fopen("/Users/Felix/Desktop/CECS_326/FishSwim/swimmill_output.txt", "a");
    fprintf(fp, "\nPellet killed because time limit reached. PID %d\n", getpid());
    fclose(fp);
    
    shmdt(swim_mill);
    exit(0);
}


