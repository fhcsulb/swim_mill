//
//  fish.c
//  FishSwim
//
//  Created by Felix 
//  Copyright © 2017 Felix. All rights reserved.
//

#include "include.h"
#include <stdio.h>



void endProcess();
int * findPellet();
bool eat(int, int);
void moveFishRight();
void moveFishLeft();


int fish_current = (mill_length/2);


int main()
{
    printf("PID %d fish executing\n\n", getpid());
    
    fp = fopen("/Users/Felix/Desktop/CECS_326/FishSwim/swimmill_output.txt", "a");
    fprintf(fp, "PID %d fish executing\n", getpid());
    fclose(fp);
    
    signal(SIGUSR1, endProcess);
   
    
    //attach process to shared memory
    attachMemory();
    
    (*swim_mill)[mill_height - 1][mill_length /2] = fish;
    
    while(1) {
        
        int * plocation = findPellet();
        if(plocation == 0) {
              sleep(1);
            continue;
        }
        int x = *plocation;
        int y = *(plocation+1);
        
        
        while(eat(x,y) == false /* and while the pellet is still in the river*/) {
               sleep(1);
            
             plocation = findPellet();
            if(plocation == 0) {
                sleep(1);
                continue;
            }
            x = *plocation;
            y = *(plocation+1);
            
            
            // use position of pellet to determine where the fish goes
            if(y > fish_current) { // fish is to the left of pellet
                moveFishRight();
            }
            else if(y < fish_current) { //fish is to the right of the pellet
                moveFishLeft();
            }
        }
          // sleep(1); < -- commented that out for trials. REMEMBER!
       
       
    }
    
    
}
  

int * findPellet() {
    
    static int loc[2];
    for(int ii=9; ii >= 0; ii--) {
        for(int jj=0; jj < mill_length; jj++ ) {
            if((*swim_mill)[ii][jj] == 'p') {
                loc[0] = ii;
                loc[1] = jj; 
                
                return loc;
            }
        }
    }
     return 0;
}

bool eat(int x, int y) {
    //if(x == (river_height-1) && y == current) {
    if((*swim_mill)[mill_height-2][fish_current] == 'p') {
        (*swim_mill)[mill_height-1][fish_current] = fish; //make sure fish isn't overridden
        
       
        
        return true;
    }
    
     

        
    return false;
}

void moveFishRight() {
    (*swim_mill)[9][fish_current] = water;
    fish_current++;
    (*swim_mill)[9][fish_current] = fish;
    
}

void moveFishLeft() {
    (*swim_mill)[9][fish_current] = water;
    fish_current--;
    (*swim_mill)[9][fish_current] = fish;
}


// Kill and end process
// Detach shared memory
void endProcess(){
    // Kill processes time limit
    printf("\nFish killed because time limit reached. PID %d \n", getpid());
    
     fp = fopen("/Users/Felix/Desktop/CECS_326/FishSwim/swimmill_output.txt", "a");
    fprintf(fp, "\nFish killed because time limit reached. PID %d \n", getpid());
    fclose(fp);
    
    shmdt(swim_mill);
    exit(0);
}


