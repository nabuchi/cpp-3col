#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <list>
#include <vector>
#include <functional>
#include <algorithm>
#include <ctime>
#include <time.h>
#include <map>

#include "mt.h"
#include "sha1.h"

#ifndef uint32
#define uint32 unsigned int
#endif

using namespace std;

uint32 gc;
uint32 gcc;
uint32 gccc;

uint32 thash(uint32 m)
{
    gc++;
    if(gc == 4000000000) { gcc++; }
    if(gcc == 4000000000) { gccc++; }
    SHA1 sha;
    unsigned message_digest[5];
    char str[10];

    sha.Reset();
    sprintf(str, "%u", m);
    sha << str;
    sha.Result(message_digest);

    return (uint32)message_digest[4];   
}
uint32 thash2(uint32 m)
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
struct StartLess
{
    bool operator()( const uint32* li, const uint32* ri ) const
    {
        return li[0] < ri[0];
    }
};
struct ZeroEqual
{
    bool operator() (const uint32* li, const uint32* ri) {
        return li[0] == ri[0];
    }
};
struct tblchkret_l
{
    bool hyouka;
    list<uint32*>::iterator point;
};
struct tblchkret_l tablechk_l(list<uint32*> ctab, uint32 target ) {
    struct tblchkret_l ret;
    list<uint32*>::iterator pp;
    ret.hyouka = false;
    ret.point = ctab.begin();
    for(pp=ctab.begin(); pp!=ctab.end(); ++pp) {
        if( (*pp)[0] == target ) {
            ret.hyouka = true;
            ret.point = pp;
            return ret;
        }
    }
    return ret;
}

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
//N=2^32a
const uint32 N_A = 1663;//2048;
const uint32 N_R = 1663;//2048;
int main()
{
    time_t start,end;
    start = time(0);
    printf("開始時間:%d\n",start);
    gc = 0;
    gcc = 0;
    gccc = 0;
    //テーブル作成
    //vector<uint32*> table;
    map<uint32, uint32> table;
    uint32* factor;
    for(uint32 i=0; i<N_A; i++) {
        factor = new uint32[2];
        uint32 p = (uint32)i*i;
        //printf("%u\n", p);
        factor[0] = p; 
        for(uint j=0; j<N_R; j++) {
            p = thash(p);
        }
        factor[1] = p;
        //printf("%u:%u\n", factor[0], factor[1]);
        //table.push_back(factor);
        table.insert( pair<uint32, uint32>( factor[1], factor[0] ) );
    }
    //sort(table.begin(), table.end(), GoalLess() );
    //goal
    //list<uint32*> retable;
    map<uint32, uint32*> retable;
    time_t kourap = time(0);
    printf("後半開始時間:%d\n",kourap);
    printf("前半時間:%d gc:%u gcc:%u\n",kourap-start,gc,gcc);
    cout << "後半" << endl;
    uint32 t = 0;
    init_genrand( (unsigned long)time(0) );
    while( retable.size() < N_A ){
        //printf("seed:%u\n",seed);
        uint32 a = genrand_int32();
        //printf("%u\n",a);
        uint32 b = a;
        //debug
        //a = 1024;
        //b = a;
        //debug
        for(uint32 j=1; j<=N_R; j++) {
            //printf("b生成 b:%u thash(b):%u j:%u\n", b, thash(b),j);
            b = thash(b);
            //struct tblchkret tmp;
            //tmp = tablechk(table,b);
            //if( table[b] != nil )
            //bool bol = tmp.hyouka;
            //vector<uint32*>::iterator k = tmp.point;
            map<uint32, uint32>::iterator k;
            k = table.find(b);
            if( k != table.end() ) {//tableをチェックしてイテレータkでbが見つかった
                //printf( "start:%u goal:%u j:%u a:%u b:%u\n", (*k)[0], (*k)[1], j, a,b );
                
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
                //printf("入った\n");
                //uint32 aa = (*k)[0];
                uint32 aa = k->second;
                //printf("aa:%u\n", aa);
                //aaと(*k)[0]を同じ位置に合わせる
                for( uint32 l=1; l<=N_R-j; l++ ) {
                    aa = thash(aa);
                }
                //a-b=(*k)[1]-(*k)[0]=aa
                if( a != aa ) {
                    //printf("a:%u aa:%u\n",a,aa);
                    b = thash(a);
                    uint32 bb = thash(aa);
                    uint32 count = N_R-j;
                    //printf("b:%u bb:%u\n",b,bb);
                    while( b != bb ) {
                        //printf("b:%u bb:%u  count:%u\n",b, bb, count);
                        //if( count >= N_R-j+1 || (*k)[1]==b || (*k)[0]==bb ) { printf("%u:%d\n",(*k)[1],j);return 0; }
                        //if (count >= N_R+2) { return 0; } 
                        a = b;
                        aa = bb;
                        b = thash(a);
                        bb = thash(aa);
                        count++;//debug
                    }
                    uint32* factor;
                    factor = new uint32[2];
                    factor[0] = a;
                    factor[1] = aa;
                    //printf("b:%u a:%u aa:%u\n",b,a,aa);
                    //retable.push_back(factor);
                    retable.insert( pair<uint32, uint32*>( b, factor ) );
                    ++t;
                    if( t%100 == 0 ) { printf("出た%u\n",retable.size()); }
                    //if( !(retable.size() < N_A) ) {//ループが終わりそうだったらuniqueする 
                    //    retable.sort(StartLess());retable.unique(ZeroEqual());printf("unique:%u\n", retable.size());
                    //}
                    break;
                } else {
                    printf("一致した%u\n",t);
                    break;
                }
            }
        }
    }
    //tableを出力
    printf("owari\n");
    //retable.sort( StartLess() );    
    //for(map<uint32, uint32*>::iterator kout = retable.begin(); kout != retable.end(); ++kout) {
    //    printf("Img:%u Pr1:%u Pr2:%u Size:%u\n", kout->first, (kout->second)[0], (kout->second)[1], retable.size() );
    //}
    //retableにはuniqueな2collisionsがN_A個入ってる
    //ランダムな値に関数を取ってretableと一致するかどうか調べる
    time_t srap = time(0);
    printf("サーチ開始時間:%d\n",srap);
    printf("後半時間:%d gc:%u gcc:%u\n",srap-kourap,gc,gcc);
    printf("search\n");
    const uint32 N_B = 2581897;//2^20
    init_genrand( (unsigned long)time(0) );
    for(uint32 i=0; i<N_B; i++) {
        uint32 a = genrand_int32();
        //printf("%u\n",a);
        uint32 b = thash(a);
        struct tblchkret_l tmp;
        //tmp = tablechk_l(retable, b);
        //bool bol = tmp.hyouka;
        ///list<uint32*>::iterator k = tmp.point;
        map<uint32, uint32*>::iterator k = retable.find(b);
        if( k != retable.end() ) {
            printf( "a:%u k1:%u k2:%u  from%u i:%u gc:%u gcc:%u\n",a,(k->second)[0],(k->second)[1],b,i,gc,gcc );
            if( (k->second)[0] != a && (k->second)[1] != a ) {
                time_t okrap = time(0);
                printf( "%u:%u:%u from %u   i:%u gc:%u gcc:%u time:%d\n",a,(k->second)[0],(k->second)[1],b,i,gc,gcc,okrap-srap );
                return 0;
            }
        }
    }
    end = time(0);
    printf("終了時間:%d\n",end);
    printf("サーチ時間:%d gc:%u gcc:%u\n",end-srap,gc,gcc);
    printf("処理時間:%d\n",end-start);
    return 0;
}
