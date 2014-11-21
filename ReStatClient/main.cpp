//
//  main.cpp
//  ReStatClient
//
//  Created by Benjamin Dews on 4/06/2014.
//  Copyright (c) 2014 Benjamin Dews. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include "pthread.h"
#include "memory.h"
#include "cpu.h"
#include "connection.h"
#include "dataFormat.h"

struct program_data{
    int hasQuit;
    char ipInput[260];
};

void *connect(void *data){
    struct program_data *tData;
    tData = (struct program_data*) data;
    while ((*tData).hasQuit == 0){
        connectToHost((*tData).ipInput);
        sleep(10);//prevents spamming the network if server is down
    }
    
    return NULL;
    
}


void *readInput(void *data){
    struct program_data *tData;
    tData = (struct program_data*) data;
    char input[5];
    
    while (true){
        scanf ("%s",input);
        if ((strncmp(input, "quit", 4))||
            (strncmp(input, "Quit", 4))||
            (strncmp(input, "exit", 4))||
            (strncmp(input, "Exit", 4)) == 0)
            {
                (*tData).hasQuit = 1;
                break;
            }
    }
    
    
    
    return NULL;
}








int main(int argc, const char * argv[])
{
    
    //would have liked to keep threading serpate to main,
    //but my C code needs to be portable and easily hooked into
    //so therefore I cant mix input(to handle quitting) and function
    
    
    printf("Connecting to server on IP:%s\n",argv[1]);
    printf("'Exit' or 'Quit' to close client\n");

    
    pthread_t thread[2];
    struct program_data tData;
    void *status;
    
    tData.hasQuit = 0;
    strcpy(tData.ipInput, argv[1]);
    
    while (tData.hasQuit == 0){
    pthread_create(&thread[0], NULL, connect, &tData);
    pthread_create(&thread[1], NULL, readInput,&tData);
    pthread_join(thread[1], &status);
    }
    
    

    
    printf("Client exiting...\n");
    return 0;
}

