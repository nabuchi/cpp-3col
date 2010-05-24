#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <list>
#include <vector>
#include <functional>
#include <algorithm>
#include <ctime>

#include "mt.h"
//#include "md5Main.h"

#ifndef uint32
#define uint32 unsigned int
#endif

using namespace std;

uint32 thash(uint32 m)
{
    uint32 ret;
    init_genrand((unsigned long)m);
    ret = genrand_int32();
    return ret;   
}
struct GoalLess
{
    bool operator()( const uint32* li, const uint32* ri ) const
    {
        return li[1] < ri[1];
    }
};

//Alpha<=1/3 GAMMA=(1-Alpha)/2=1/3
//N=2^32
const uint32 N_A = 500;//2048;
const uint32 N_R = 500;//2048;
int main()
{
    vector<uint32*> table;
    uint32* factor;
    for(uint32 i=0; i<N_A; i++) {
        factor = new uint32[2];
        uint32 p = (uint32)i*i;
        //printf("%u\n", p);
        factor[0] = p; 
        for(uint32 j=0; j<N_R; j++) {
            p = thash(p);
        }
        factor[1] = p;
        //printf("%u:%u\n", factor[0], factor[1]);
        table.push_back(factor);
    }
    sort(table.begin(), table.end(), GoalLess() );
    //後半
    cout << "後半" << endl;
    init_genrand( (unsigned long)time(0) );
    vector<uint32*>::iterator it = table.begin();
    while( it != table.end() ) {
        printf( "%u:%u\n", (*it)[0], (*it)[1] );
        r = genrand_int32();
        for(uint32 i=0; i<N_R; i++) {
            r = thash(r);
        }
        
        
        ++it;
    }
    return 0;
/*
  list<uint32*> table;
  init_genrand((unsigned long)time(NULL));
  for(int i=0;i<FUKA;i++) {
    uint32 s = genrand_int32();
    uint32 a = s;
    uint32 l = 0;
    uint32* factor;
    while( l<L_MAX ) {
        a = thash(a);
        //printf("1:%u:%u:%u\n",a,M,s);
        l++;
        if( a < M ) {
            factor = new uint32[3];
            factor[0] = s;
            factor[1] = a;
            factor[2] = l;    
            table.push_back(factor);
            //if (i%100 == 0) {printf("1.5(%d):%u:%u\n",i,a,l); }
            printf("1.5(%d):%u:%u\n",i,a,l);
            break;
        }
    }
  }
    table.sort( GoalLess() );
    
    //print
    list<uint32*>::iterator it = table.begin();
    while( it != table.end() ) {
        uint32* tmp = *it;
        printf("2:%u:%u:%u\n",tmp[0],tmp[1],tmp[2]);
        ++it;
    }

    it = table.begin();
    while( it != table.end() ) {
        list<uint32*>::iterator jt = it+1;
        while( jt != table.end() && *jt == *it )
            ++jt;
        if( jt >= it+3 ) {
            // L is maxL
            uint32 L = 0;
            list<uint32*>::iterator tt = it;
            while( tt != jt) {
                if( L < *tt )
                    L = *tt;
                ++tt;
            }
            for( int l=L; l>=0; l-- )
                for( tt=
         
*/
}
