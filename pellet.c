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
    
    fp = fopen("/Users/Felix/Desktop/CECS_326/FishSwim/swimmill_output.txt", "a");
    fprintf(fp, "PID %d pellet executing\n\n", getpid());
    fclose(fp);
    
    //signal setup
    signal(SIGUSR1, endProcess);
    
    // attach process to memory.
    attachMemory();
    
    printf("Pellet memory has been attachd ! \n");
    
    srand(time(NULL));
    
    // create pthread array
    pthread_t pellets[20];
    
    // Create 10 random threads
    for(int i = 0; i < 20; i++){
        int position[2] = {0, rand()%10};// Generates the pellet position
        pthread_create(&pellets[i], NULL, pellet_thread, position);
        
        // every 3 seconds a new pellet will be added. Added this in
        // so that pellets would populate at regular time intervals and appear
        // at different locations which would test fish movement. 
        sleep(rand()%4+3);
    }
    // Joins all threads
    for(int i = 0; i < 20; i++){
        pthread_join(pellets[i], NULL);
    }
    
   
}

void *pellet_thread(int *loc)
{
    // retrieve the pellet's location.
    int x = *loc;
    int y = *(loc+1);
    
    fp = fopen("/Users/Felix/Desktop/CECS_326/FishSwim/swimmill_output.txt", "a");
    fprintf(fp, "Pellet has been created in column %d \n",y);
    fclose(fp);
    
    // place the pellet into the river.
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
    
    // by the time the pellet reaches the last row of the river,
    // if there isn't a fish there, then change that position back to water. 
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
    fprintf(fp, "Pellet killed because time limit reached. PID %d\n", getpid());
    fclose(fp);
    
    shmdt(swim_mill);
    exit(0);
}


