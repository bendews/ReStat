//
//  connection.cpp
//  ReStatClient
//
//  Created by Benjamin Dews on 4/06/2014.
//  Copyright (c) 2014 Benjamin Dews. All rights reserved.
//


#include <sys/socket.h>
//#include <netinet/in.h>
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "connection.h"
#include "memory.h"
#include "cpu.h"
#include "dataFormat.h"


char *getData(){
    
    int totmem = totalMemory();
    char *TOTAL_MEMORY = formatInt(totmem);
    
    float freemem = freeMemory();
    float usedmem = usedMemory();
    char *FREE_MEMORY = formatFloat(freemem);
    char *USED_MEMORY = formatFloat(usedmem);
    
    double loadavg[1];
    getloadavg(loadavg, 1);
    char *CPU_LOAD_AVG = formatDouble(loadavg[0] * 10);
    
    double cpuLoad = getCpuReading();
    char *CPU_LOAD_CURR = formatDouble(cpuLoad);
    
    double cpuTemp = getCpuTemp();
    char *CPU_TEMP = formatDouble(cpuTemp);
    
    char host[40];//cuts username off if more than 40 characters.
    gethostname(host, sizeof(host));
    char HOSTNAME[40] ="|";
    strcat(HOSTNAME,host);
    
    int hostlen = strlen(HOSTNAME);
    for (int s = hostlen; s <40; s++){//fill excess space to prevent overflows.
        strcpy(&HOSTNAME[s], " ");
    }
    
    char *data = new char[75]; //original 35
    strcpy(data,HOSTNAME);
    strcat(data,TOTAL_MEMORY);
    strcat(data,FREE_MEMORY);
    strcat(data,USED_MEMORY);
    strcat(data,CPU_LOAD_AVG);
    strcat(data,CPU_LOAD_CURR);
    strcat(data,CPU_TEMP);
    return data;
}


int connectToHost(const char *ipInput)
{
    signal(SIGPIPE, SIG_IGN);
    int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_desc == -1)
    {
        printf("cannot create socket!\n");
        return 0;
    }
    
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr(ipInput);
    client.sin_port = htons(9025);
    
    if (connect(sock_desc, (struct sockaddr*)&client, sizeof(client)) != 0)
    {
        printf("cannot connect to server!\n");
        close(sock_desc);
        return 0; 
    }
    
    
    char *p_buf;
    int k, len;
    printf("Connection successful. Sending data..\n");
    
    while(1)
    {
        sleep(1);
        char *buf = getData();
        len = strlen(buf);
        p_buf = buf;
        
        while (len > 0)
        {
            k = send(sock_desc, p_buf, len, 0);
            if (k == -1)
            {
                printf("Connection has been lost. Retrying...\n");
                return 0;
                break;
            }
            
            p_buf += k;
            len -= k;
        }
    
        
        if (strcmp(buf, "exit") == 0)
            break;
    }
    
    close(sock_desc);
    printf("client disconnected\n");
    
    return 0;
}


