//
//  memory.cpp
//  ReStatClient
//
//  Created by Benjamin Dews on 4/06/2014.
//  Copyright (c) 2014 Benjamin Dews. All rights reserved.
//

#include "memory.h"
#include <sys/types.h>
#include <sys/sysctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <sys/statvfs.h>


float usedMemory(){
    float usedmem = 0.0;
    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics_data_t vm_stats;
    
    mach_port = mach_host_self();
    count = sizeof(vm_stats) / sizeof(natural_t);
    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
        KERN_SUCCESS == host_statistics(mach_port, HOST_VM_INFO,
                                        (host_info_t)&vm_stats, &count))
    {
        usedmem = ((float)vm_stats.active_count +
                   (float)vm_stats.inactive_count +
                   (float)vm_stats.wire_count) *  (float)page_size;
    }
    usedmem = usedmem / 1024000000;
    
    return usedmem;
}




float freeMemory(){
    float freemem = 0.0;
    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics_data_t vm_stats;
    
    mach_port = mach_host_self();
    count = sizeof(vm_stats) / sizeof(natural_t);
    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
        KERN_SUCCESS == host_statistics(mach_port, HOST_VM_INFO,
                                        (host_info_t)&vm_stats, &count))
    {
        freemem = (float)vm_stats.free_count * (float)page_size;
    }
    freemem = freemem / 1024000000;
    
    return freemem;
}

int totalMemory() {
    int mib[2];
    int64_t physical_memory;
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    size_t length = sizeof(int64_t);
    sysctl(mib, 2, &physical_memory, &length, NULL, 0);
    physical_memory = physical_memory / 1024000000;
    return physical_memory;
}