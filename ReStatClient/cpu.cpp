//
//  cpu.cpp
//  ReStatClient
//
//  Created by Benjamin Dews on 5/06/2014.
//  Copyright (c) 2014 Benjamin Dews. All rights reserved.
//
// Lots of other CPU stats returned here that I could include in the output
// easily if I ever need too!



#include "cpu.h"
#include "smc.h"
#include <iostream>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <mach/mach.h>
#include <mach/processor_info.h>
#include <mach/mach_host.h>
#include <unistd.h>


struct cpusample {
    uint64_t totalSystemTime;
    uint64_t totalUserTime;
    uint64_t totalIdleTime;
    
};


double printSample(struct cpusample *sample)
{
    uint64_t total = (*sample).totalSystemTime + (*sample).totalUserTime + (*sample).totalIdleTime;
    
    double onePercent = total/100.0f;
    
    
    double sys = (*sample).totalSystemTime / onePercent;
    double user = (*sample).totalUserTime / onePercent;
    double totalUsed = sys + user;
    
    return totalUsed;
}

void timeSample(struct cpusample *sample)
{
    kern_return_t kr;
    mach_msg_type_number_t count;
    host_cpu_load_info_data_t cpu_data;

    count = HOST_CPU_LOAD_INFO_COUNT;
    kr = host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (int *)&cpu_data, &count);
    if (kr != KERN_SUCCESS) {
        printf("oops: %s\n", mach_error_string(kr));
        return;
    }
    
    (*sample).totalSystemTime = cpu_data.cpu_ticks[CPU_STATE_SYSTEM];
    (*sample).totalUserTime = cpu_data.cpu_ticks[CPU_STATE_USER] + cpu_data.cpu_ticks[CPU_STATE_NICE];
    (*sample).totalIdleTime = cpu_data.cpu_ticks[CPU_STATE_IDLE];
    
}


double getCpuReading(){
    struct cpusample finalvalues;
    struct cpusample sample1;
    struct cpusample sample2;
    
    timeSample(&sample1);
    sleep(1);
    timeSample(&sample2);
    
    finalvalues.totalSystemTime = (sample2.totalSystemTime - sample1.totalSystemTime);
    finalvalues.totalUserTime = (sample2.totalUserTime - sample1.totalUserTime);
    finalvalues.totalIdleTime = (sample2.totalIdleTime - sample1.totalIdleTime);
    
    double totalUsed = printSample(&finalvalues);
    return totalUsed;
    
}

double getCpuTemp(){
    double temp = cpuTemp();
    return temp;
    
}