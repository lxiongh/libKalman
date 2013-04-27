#ifndef RANDOM_H
#define RANDOM_H

#include <limits.h>

class Random
{
    public:
        Random();
        virtual ~Random();
        Random(long s);
        Random(long seed, bool randomNextSeed);
        void SetSeed(long s) { Seed = s;}
        unsigned short ComputeRandom();
        unsigned short Compute_NoSeedChange();

        unsigned int NextGuass_Byte(unsigned int ave, unsigned int sigma);
        int NextGuass_Int(int ave, int sigma);
        float NextGuass_Float(float ave, float sigma);
        double NextGuass_Double(double ave, double sigma);

        unsigned int NextFlat_Byte(unsigned int min,unsigned int max);
        int NextFlat_Int(int min, int max);
        float NextFlat_Float(float min, float max);
        double NextFlat_Double(double min, double max);

        char NextChar();
        unsigned int NextByte();
        short NextShort();
        unsigned short NextUShort();
        int NextInt();
        long NextLong();
        float NextFloat();
        double NextDouble();

    protected:
    private:
        long Seed;
        bool RandomNextSeed;
};

#endif // RANDOM_H
