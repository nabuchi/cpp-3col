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
const uint32 N_A = 500;//2048;
const uint32 N_R = 500;//2048;
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
        for(uint32 j=1; j<=N_R; j++) {
            b = thash(b);
            struct tblchkret tmp;
            tmp = tablechk(table,b);
            bool bol = tmp.hyouka;
            vector<uint32*>::iterator k = tmp.point;
            if( bol ) {//tableをチェックしてイテレータkでbが見つかった
                printf("%u:%u\n", b, (*k)[1]);
                printf("入った\n");
                uint32 aa = (*k)[0];
                //同じ値でマージしてないかチェック
                
                for( uint32 l=1; l<=N_R-j; l++ ) {
                    aa = thash(aa);
                }
                
                //a-b=(*k)[1]-(*k)[0]=aa
                if( a != aa ) {
                    b = thash(a);
                    uint32 bb = thash(aa);
                    uint32 count = 0;
                    while( b != bb ) {
                        count++;
                        printf("%u:%u  %u\n",b, bb, count);
                        if( count >= N_R+1 ) { return 0; }
                        a = b;
                        aa = bb;
                        b = thash(a);
                        bb = thash(aa);
                    }
                    uint32 factor[3];
                    factor[0] = b;
                    factor[1] = a;
                    factor[2] = aa;
                    retable.push_back(factor);
                    ++t;
                }
                continue;
                printf("出た\n");
            }
        }
    }
    //tableを出力
    printf("owari\n");
   return 0;
}
