//
//  fish.c
//  FishSwim
//
//  Created by Felix
//  Copyright © 2017 Felix. All rights reserved.
//

#include "include.h"
#include <stdio.h>


// prototype declaration
void endProcess();
int * findPellet();
bool eat(int, int);
void moveFishRight();
void moveFishLeft();
void killProcess();

// declare a variable for the fish's current position.
int fish_current = (mill_length/2);


int main()
{
    // print to the console that the fish is executing.
    printf("PID %d fish executing\n", getpid());
    
    fp = fopen("/Users/Felix/Desktop/CECS_326/FishSwim/swimmill_output.txt", "a");
    fprintf(fp, "PID %d fish executing\n", getpid());
    fclose(fp);
    
    // signal setup
    signal(SIGUSR1, endProcess);
    signal(SIGINT, killProcess);
    
    //attach process to shared memory
    attachMemory();
    
    // initially, the fish's position is still at the last row and in the center
    (*swim_mill)[mill_height - 1][mill_length /2] = fish;
    
    while(1)
    {
        // function call to find the nearest pellet location. This will return
        // an int* of the x and y coordinates.
        int * pelletLocation = findPellet();
        
        // if the value returned is 0, this means that the fish does not detect
        // any pellets in the river.
        if(pelletLocation == 0) {
            sleep(1);
            continue;
        }
        
        // write the return value of findPellet() to an int variable(s)
        int pellet_x = *pelletLocation;
        int pellet_y = *(pelletLocation+1);
        
        // while the fish cannot eat, the fish should keep trying to look for pellets.
        while(eat(pellet_x, pellet_y) == false )
        {
            sleep(1);
            
            /*
            pelletLocation = findPellet();
            if(pelletLocation == 0) {
                sleep(1);
                continue;
            }
            pellet_x = *pelletLocation;
            pellet_y = *(pelletLocation+1);
            */
            
            // use position of pellet to determine where the fish goes
            // if the fish is to the left of the pellet, move the fish right
            if(pellet_y > fish_current)
            {
                moveFishRight();
            }
            else if(pellet_y < fish_current)
            { //fish is to the right of the pellet
                moveFishLeft();
            }
            break;
        }
        // sleep(1); < -- commented that out for trials. REMEMBER!
        
        
        
    }
    
}

// function to find the location of the nearest pellet to the fish.
// start from the bottom row of the river and traverse up till the top row
// the nearest pellet will be the one that is closest to the bottom of the river.
int * findPellet()
{
    // create an array of size 2 to store the coordinates of the nearest pellet.
    static int nearest_pellet[2];
    for(int ii=9; ii >= 0; ii--)
    {
        for(int jj=0; jj < mill_length; jj++ )
        {
            if((*swim_mill)[ii][jj] == 'p')
            {
                // if the pellet was found in the river, then save it's coordinates
                // and return those coordinates.
                nearest_pellet[0] = ii;
                nearest_pellet[1] = jj;
                
                return nearest_pellet;
            }
        }
    }
    return 0;
}

// this is a function to check if the fish can eat.
// if the pellet is right above the fish, then it can eat it (return true).
// otherwise, return false
bool eat(int x, int y)
{
    
    if((*swim_mill)[mill_height-2][fish_current] == 'p')
    {
        // this statement is here so the fish doesn't get overwritten.
        (*swim_mill)[mill_height-1][fish_current] = fish;
        
        return true;
    }
    
    
    return false;
}

// function call to move the fish right.
// fish's previous location becomes water
void moveFishRight()
{
    (*swim_mill)[9][fish_current] = water;
    fish_current++;
    (*swim_mill)[9][fish_current] = fish;
    
}


// function call to move the fish left.
// fish's previous location becomes water
void moveFishLeft()
{
    (*swim_mill)[9][fish_current] = water;
    fish_current--;
    (*swim_mill)[9][fish_current] = fish;
}


// Kill and end process
// Detach shared memory
void endProcess()
{
    // Kill processes time limit
    printf("\nFish killed because time limit reached. PID %d \n", getpid());
    
    fp = fopen("/Users/Felix/Desktop/CECS_326/FishSwim/swimmill_output.txt", "a");
    fprintf(fp, "\nFish killed because time limit reached. PID %d \n", getpid());
    fclose(fp);
    
    shmdt(swim_mill);
    exit(0);
}

void killProcess()
{
    // Kill processes time limit
    printf("\nFish killed because CTRL + C \n");
    
    fp = fopen("/Users/Felix/Desktop/CECS_326/FishSwim/swimmill_output.txt", "a");
    fprintf(fp, "\nFish killed because CTRL + C. PID %d\n",getpid());
    fclose(fp);
    exit(1);
}

