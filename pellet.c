//
//  pellet.c
//  FishSwim
//
//  Created by Felix on 10/17/17.
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

void *pellet_thread(void *loc);
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

void *pellet_thread(void *loc)
{
    // Pellet location
    int r = *((int *)loc);
    int c = *((int *)loc+1);
    int x = 0;
    
    // Places pellet thread inside array
    // printf("\nPellet %d created R: %d C: %d\n", (int)pthread_self(), r, c);
    (*swim_mill)[r][c] = 'p';
    sleep(1);
    
    for(int row = r; row < 10; row++){
        // Find the current location of the fish
        for(x = 0; x < 10; x++){
            if((*swim_mill)[9][x] == fish){
                break;
            }
        }
        
        // Pellet flows with stream from row 9 > 0
        if (row < 9){
            (*swim_mill)[row][c] = water;
            (*swim_mill)[row+1][c] = 'p';
            
        }
        
        // If F and Pellet are in same location it is eaten.
        else if(row == 9){
            if ((*swim_mill)[row][c] == fish){
               (*swim_mill)[row][c] = fish;
                //printf("\nPellet %d was eaten at Col: %d!\n", (int)pthread_self(), c);
                
                
                pthread_exit(0);
                break;
            }else{
                (*swim_mill)[row][c] = water;
                
                // printf("\nPellet %d left at Col: %d!\n", (int)pthread_self(), c);
            }
        }
        sleep(1);
    }
    pthread_exit(0);
}

// Kill and end process
// Detach shared memory
void endProcess(){
    printf("\nPellet killed because time limit reached. PID %d\n", getpid());
    shmdt(swim_mill);
    exit(0);
}


