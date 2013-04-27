#include "Random.h"

Random::Random(){
    //ctor
    Seed = 0;
    RandomNextSeed = false;
}

Random::Random(long s){
    Seed = s;
    RandomNextSeed = false;
}

Random::Random(long s, bool randomNextSeed){
    Seed = s;
    RandomNextSeed = randomNextSeed;
}

unsigned short Random::ComputeRandom(){
    unsigned short rv = Compute_NoSeedChange();
    Seed++;
    if (RandomNextSeed)
    {
        Seed += Compute_NoSeedChange();
    }
    return rv;
}

unsigned short Random::Compute_NoSeedChange(){
    // This is a simple hash, and it's fast.
    long long v[] = {
        0x73ae2743a3eab13c,
        0x53a75d3f2123bda1,
        0x42a3bcba71a72843};

    long rv = Seed;
    for (int i = 0; i < 3; i++){
        rv += v[i];
        rv -= rv << 3;
        rv ^= (v[i] >> 7);
        rv ^= rv >> 11;
    }
    return (unsigned short)(rv & 0xFFFF);
}


Random::~Random(){
    //dtor
}

unsigned int Random::NextGuass_Byte(unsigned int ave, unsigned int sigma){
    int sum = 0;
    int sig3 = sigma + sigma + sigma;
    int low = ave - sig3;
    int hi = ave + sig3;
    for (int i = 0; i < 4; i++) sum += NextFlat_Int(low, hi);
    return (unsigned int)(sum / 4);
}


int Random::NextGuass_Int(int ave, int sigma){
    int sum = 0;
    int sig3 = sigma + sigma + sigma;
    int low = ave - sig3;
    int hi = ave + sig3;
    for (int i = 0; i < 4; i++) sum += NextFlat_Int(low, hi);
    return sum / 4;
}


float Random::NextGuass_Float(float ave, float sigma){
    float sum = 0;
    float sig3 = sigma + sigma + sigma;
    float low = ave - sig3;
    float hi = ave + sig3;
    for (int i = 0; i < 4; i++) sum += NextFlat_Float(low, hi);
    return sum / 4;
}

double Random::NextGuass_Double(double ave, double sigma){
    double sum = 0;
    double sig3 = sigma + sigma + sigma;
    double low = ave - sig3;
    double hi = ave + sig3;
    for (int i = 0; i < 4; i++) sum += NextFlat_Double(low, hi);
    return sum / 4;
}

//------------------------------------------------
unsigned int Random::NextFlat_Byte(unsigned int min,unsigned int max){
    return (unsigned int)NextFlat_Int(min, max);
}


int Random::NextFlat_Int(int min, int max){
    return (int)NextFlat_Float(min, max);
}


float Random::NextFlat_Float(float min, float max){
    float s = (max - min);
    float m = max < min ? max : min;
    return (float)(m + s * NextFloat());
}

double Random::NextFlat_Double(double min, double max){
    double s = (max - min);
    double m = max < min ? max : min;
    return (double)(m + s * NextDouble());
}

//------------------------------------------------
char Random::NextChar(){
    return (char)ComputeRandom();
}

unsigned int Random::NextByte(){
    return (unsigned int)ComputeRandom();
}

short Random::NextShort(){
    int i = ComputeRandom();
    return (short)((i & 0x1) == 0 ? -i : i);
}

unsigned short Random::NextUShort(){
    return ComputeRandom();
}

int Random::NextInt(){
    int rv = 0;
    for (int i = 0; i < 4; i++){
        rv = (rv << 16) + (int)NextUShort();
    }
    return rv;
}

long Random::NextLong(){
    long rv = 0;
    for (int i = 0; i < 4; i++)
    {
        rv = (rv << 16) + (long)NextUShort();
    }
    return rv;
}

float Random::NextFloat(){
    return (float)NextDouble();
}

double Random::NextDouble(){
    double d = (double)NextLong();
    d = d < 0 ? -d : d;
    d = d / LONG_MAX;
    return  d < 0 ? (d > 1 ? 1 : 0) : d;
}
