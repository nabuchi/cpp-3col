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

struct tblchkret
{
    bool hyouka;
    vector<uint32*>::iterator point;
};

//後でここtableのポインタで受け取れるように
//二分木探索で発見できるように
struct tblchkret tablechk(vector<uint32*> ctab, uint32 target ) {
    struct tblchkret ret;
    vector<uint32*>::iterator pp;
    ret.hyouka = false;
    ret.point = ctab.begin();
    for(pp=ctab.begin(); pp!=ctab.end(); ++pp) {
        if( (*pp)[1] == target ) {
            ret.hyouka = true;
            ret.point = pp;
            return ret;
        }
    }
    return ret;
}

//Alpha<=1/3 GAMMA=(1-Alpha)/2=1/3
//N=2^32
const uint32 N_A = 10;//2048;
const uint32 N_R = 10;//2048;
int main()
{
    //テーブル作成
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
    //goal
    list<uint32*> retable;
    cout << "後半" << endl;
    uint32 t = 1;
    srand( (unsigned) time(0) );
    while( t < N_A ){
        init_genrand( (unsigned long)( time(0)+rand() ) );
        uint32 a = genrand_int32();
        uint32 b = a;
        //debug
        //a = 1024;
        //b = a;
        //debug
        for(uint32 j=1; j<=N_R; j++) {
            //printf("b生成 b:%u thash(b):%u j:%u\n", b, thash(b),j);
            b = thash(b);
            struct tblchkret tmp;
            tmp = tablechk(table,b);
            bool bol = tmp.hyouka;
            vector<uint32*>::iterator k = tmp.point;
            if( bol ) {//tableをチェックしてイテレータkでbが見つかった
                printf( "start:%u goal:%u j:%u a:%u b:%u\n", (*k)[0], (*k)[1], j, a,b );
                
                //debug
                /*
                uint32 daa = (*k)[0];
                uint32 da = a;
                for (uint32 l=1; l<=N_R-j; l++) {
                    printf("長さ合わせ  %u\n", daa);
                    daa = thash(daa); 
                }
                uint32 dc=N_R-j+2;
                for(uint32 ll=0; ll<N_R; ll++) {
                    printf("hikaku  %u:%u count:%u\n", daa,da,dc);
                    daa = thash(daa);
                    da = thash(da);
                    dc++;
                }
                
                
                printf("%u:%u\n", (*k)[0], (*k)[1]);
                for(uint32 jj=0; jj<N_R+2; jj++) {
                    printf("%u: %d\n",(*k)[0], jj);
                    (*k)[0] = thash( (*k)[0] );
                }
                */
                //debug
                printf("入った\n");
                uint32 aa = (*k)[0];
                //printf("aa:%u\n", aa);
                //aaと(*k)[0]を同じ位置に合わせる
                for( uint32 l=1; l<=N_R-j; l++ ) {
                    aa = thash(aa);
                }
                //a-b=(*k)[1]-(*k)[0]=aa
                if( a != aa ) {
                    printf("a:%u aa:%u\n",a,aa);
                    b = thash(a);
                    uint32 bb = thash(aa);
                    uint32 count = N_R-j;
                    printf("b:%u bb:%u\n",b,bb);
                    while( b != bb ) {
                        printf("b:%u bb:%u  count:%u\n",b, bb, count);
                        //if( count >= N_R-j+1 || (*k)[1]==b || (*k)[0]==bb ) { printf("%u:%d\n",(*k)[1],j);return 0; }
                        //if (count >= N_R+2) { return 0; } 
                        a = b;
                        aa = bb;
                        b = thash(a);
                        bb = thash(aa);
                        count++;//debug
                    }
                    uint32 factor[3];
                    factor[0] = b;
                    factor[1] = a;
                    factor[2] = aa;
                    retable.push_back(factor);
                    ++t;
                }
                printf("出た%u\n",t);
                break;
            }
        }
    }
    //tableを出力
    printf("owari\n");
   return 0;
}
