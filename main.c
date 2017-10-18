//
//  main.c
//  FishSwim
//
//  Created by Felix on 10/17/17.
//  Copyright © 2017 Felix. All rights reserved.
//

#include "include.h"
#include <stdio.h>


void createMem();
void initRiver();
void printRiver();
void endProcess();

int TIME = 30;

pid_t fishID;
pid_t pelletID;


int main(int argc, const char * argv[])
{
     signal(SIGINT, endProcess);
    
     printf("Swim Mill Simulation\n");
    
    printf("Swim Mill intitial state\n");
    
    createMem();
    initRiver();
    printRiver();
    
    fishID = fork();
    if(fishID == 0)
    {
       
        static char *argv[] = {"","",NULL};
        execv("fish",argv);
    }
    
    else
    {
        pelletID = fork();
        if(pelletID == 0)
        {
            static char *argv[] = {"","",NULL};
            execv("./pellet", argv);
        }
        
        else
        {
            for(int seconds = TIME; seconds >= 0; seconds--)
            {
                
                printf("%d seconds remaining\n", seconds);
                sleep(1);
                printRiver();
                
            }
            endProcess();
            
        }
    }
    
    
    
    return 0;
    
    
}


void initRiver()
{
    for(int i=0; i < mill_height; i++) {
        printf("\n");
        for(int j=0; j < mill_length; j++) {
            (*swim_mill)[i][j] = water;
        }
    }
     
}


void printRiver()
{
    // system("clear");
    for(int i=0; i < mill_height; i++) {
        
        for(int j=0; j < mill_length; j++ ) {
            printf("%c", (*swim_mill)[i][j]);
        }
        printf("\n");
        
    }
    printf("\n---------------------");
    printf("\n");
}

void createMem()
{
    // Create shared memory ID segment
    if((sharedMemoryID = shmget(key, sizeof(swim_mill), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    
    // Attach shared memory ID to data space
    if ((swim_mill = shmat(sharedMemoryID, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
}


void endProcess() {
    //Kill child processes
    kill(fishID, SIGUSR1);
    kill(pelletID, SIGUSR1);
    
    //Detach and deallocate shared memory
    shmdt(swim_mill);
    shmctl(sharedMemoryID, IPC_RMID, 0);
    
    printf("\nswim mill process exited because time limit reached. PID %d\n", getpid());
    exit(0);
}
