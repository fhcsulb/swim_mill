//
//  main.c
//  FishSwim
//
//  Created by Felix
//  Copyright © 2017 Felix. All rights reserved.
//

#include "include.h"
#include <stdio.h>

// declare the prototypes
void killProcess();
void createMemory();
void initRiver();
void printRiver();
void endProcess();

// set TIME to 30 because that is how long the program is meant to run for.
int TIME = 30;

// these are the process IDs for fish and pellet.
// pid_t represents process IDs. Retrieve process IDs by calling getpid
pid_t fishID;
pid_t pelletID;


int main(int argc, const char * argv[])
{
    // this is the name of the file that output will be written into.
    // the file will be cleared and overwritten everytime the ./swimmill command executed on
    // the terminal.
    fp = fopen("/Users/Felix/Desktop/CECS_326/FishSwim/swimmill_output.txt", "w+");
    fclose(fp);
    
    // signal set-up.
    signal(SIGUSR1, endProcess);
    signal(SIGINT, killProcess);
    
    
    // write into the file. This is the signal that the swim mill simulation is about to begin!
    fp = fopen("/Users/Felix/Desktop/CECS_326/FishSwim/swimmill_output.txt", "a");
    fprintf(fp, "Swim Mill Simulation\n");
    fprintf(fp, "\nPID %d swim mill executing \n", getpid());
    fclose(fp);
    
    //printf("Swim Mill has begun\n");
    
    // function call to create shared memory.
    createMemory();
    
    // function call to initalize the river.
    // water will be represented by a '~'
    // the fish will be represented by a 'f'
    // the pellets will be represented by a 'p'
    initRiver();
    
    // function call to print the river.
    // this inital call will just print the water ('~') because the
    // fish and pellets haven't been forked just yet.
    printRiver();
    
    // call to fork the fish process.
    // fork will return a the value of 0 if the process was successfully created.
    fishID = fork();
    if(fishID == 0)
    {
        // execute ./fish
        static char *argv[] = {"","",NULL};
        execv("./fish",argv);
    }
    
    else
    {
        // call to fork the pellet process
        // fork will return a the value of 0 if the process was successfully created.
        pelletID = fork();
        if(pelletID == 0)
        {
            //execute ./pellet
            static char *argv[] = {"","",NULL};
            execv("./pellet", argv);
        }
        
        else
        {
            // for 30 seconds, print the state of the river.
            // by the time printRiver() is called here, the fish
            // process should already be forked.
            for(int seconds = TIME; seconds >= 0; seconds--)
            {
                
                printf("%d seconds remaining\n", seconds);
                sleep(1);
                printRiver();
                
                
                
                
            }
            // call to end swim mill because 30 seconds is up !
            endProcess();
            
        }
    }
    
    
    
    return 0;
    
}

// function call to initalize the river.
// water is initalized as '~'
void initRiver()
{
    for(int i=0; i < mill_height; i++) {
        printf("\n");
        for(int j=0; j < mill_length; j++) {
            (*swim_mill)[i][j] = water;
        }
    }
    
}

//function call to print the river.
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

//function call to create shared memory.
void createMemory()
{
    // Create shared memory ID segment
    /*
     shmget is used to allocate a shared memory segment. shmget returns the identifier of the
     shared memory segment
     parameters: key to access shared memory segment
     size of the shared memory segment
     IPC_CREAT to create a new shared memory segment.
     */
    
    if((sharedMemoryID = shmget(key, sizeof(swim_mill), IPC_CREAT | 0666)) < 0) {
        perror("shmget error");
        exit(1);
    }
    
    // Attach shared memory ID to data space
    
    /*
     shmat is used to attach the shared memory segment with the shared memory identifier
     specified by sharedmemID.
     
     if shmat returns -1, then there was an error attaching shared memory.
     
     */
    if ((swim_mill = shmat(sharedMemoryID, NULL, 0)) == (char *) -1) {
        perror("shmat error");
        exit(1);
    }
}


// function call to end all processes.
void endProcess() {
    //Kill child processes
    kill(pelletID, SIGUSR1);
    kill(fishID, SIGUSR1);
    
    
    //Detach and deallocate shared memory
    shmdt(swim_mill);
    shmctl(sharedMemoryID, IPC_RMID, 0);
    
    printf("\nswim mill process exited because time limit reached. PID %d\n", getpid());
    
    fp = fopen("/Users/Felix/Desktop/CECS_326/FishSwim/swimmill_output.txt", "a");
    fprintf(fp, "\nSwimMill killed because time limit reached. PID %d\n", getpid());
    fclose(fp);
    
    exit(0);
}

void killProcess()
{
    //Kill child processes
    kill(pelletID, SIGINT);
    kill(fishID, SIGINT);
    
    
    //Detach and deallocate shared memory
    shmdt(swim_mill);
    shmctl(sharedMemoryID, IPC_RMID, 0);
    
    printf("\nswim mill process exited because CTRL + C\n");
    
    fp = fopen("/Users/Felix/Desktop/CECS_326/FishSwim/swimmill_output.txt", "a");
    fprintf(fp, "\nswim mill process exited because CTRL + C\n");
    fclose(fp);
    
    exit(1);
}
