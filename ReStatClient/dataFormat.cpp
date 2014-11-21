//
//  data.cpp
//  ReStatClient
//
//  Created by Benjamin Dews on 4/06/2014.
//  Copyright (c) 2014 Benjamin Dews. All rights reserved.
//

#include "dataFormat.h"



char *formatFloat(float data){
    
    
    char *array= new char[10];
    snprintf(array, sizeof(array), "|%0.2f", data);
    
    
    return array;
}



char *formatDouble(double data){
    
    
    char *array= new char[10];
    snprintf(array, sizeof(array), "|%f", data);
    
    
    return array;
}




char *formatInt(int data){
    
    
    char *array= new char[10];
    snprintf(array, sizeof(array), "|%d", data);
    
    
    return array;
}