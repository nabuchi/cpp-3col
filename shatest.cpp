#include <iostream>
#include <time.h>
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
    uint32 inp;
    uint32 count;
    cout << "入力と回数" << endl; 
    cin >> inp >> count;
    uint32 seed = inp;
    clock_t start = clock();
    for(uint32 i=0; i<count; i++) {
        //printf("%u:%u\n",seed,i);
        seed = thash(seed);
    }
    clock_t end = clock();
    printf("処理時間:%d\n",(end-start));
    return 0;
}

