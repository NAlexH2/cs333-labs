#pragma once
#ifndef __RAND_H
 #define __RAND_H

#define RAND_MAX (1 << 31)


unsigned int rand(void);
void srand(unsigned int seed);

#endif // __RAND_H