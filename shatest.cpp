#include <iostream>
#include "sha1.h"

#ifndef uint32
#define uint32 unsigned int
#endif

using namespace std;

uint32 thash(uint32 m)
{
    SHA1 sha;
    unsigned message_digest[5];
    char str[10];

    sha.Reset();
    sprintf(str, "%u", m);
    sha << str;
    sha.Result(message_digest);

    return (uint32)message_digest[4];
}

int main()
{
    uint32 seed = 1;
    for(uint32 i=0; i<20; i++) {
        printf("%u:%u\n",seed,i);
        seed = thash(seed);
    }
    return 0;
}

