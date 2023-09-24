#include "err.h"
#include <iostream>

void handle_error(bool condition, const char* msg_err){
    if (condition){
        perror(msg_err); 
        exit(1);
    }
}
