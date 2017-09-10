#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED
// 1.ELF Hash Function
unsigned int ELFHash(const char *str, int TableSize)
{
    unsigned int hash = 0;
    unsigned int x    = 0;

    while (*str)
    {
        hash = (hash << 4) + (*str++);
        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }

    return (hash % TableSize);
}

// 2.SDBM Hash Function
unsigned int SDBMHash(const char *str, int TableSize)
{
    unsigned int hash = 0;

    while (*str)
    {
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash % TableSize);
}

// 3.RS Hash Function
unsigned int RSHash(const char *str, int TableSize)
{
    unsigned int b = 378551;
    unsigned int a = 63689;
    unsigned int hash = 0;

    while (*str)
    {
        hash = hash * a + (*str++);
        a *= b;
    }

    return (hash % TableSize);
}

// 4.JS Hash Function
unsigned int JSHash(const char *str, int TableSize)
{
    unsigned int hash = 1315423911;

    while (*str)
    {
        hash ^= ((hash << 5) + (*str++) + (hash >> 2));
    }

    return (hash % TableSize);
}

// 5.PJW Hash Function
unsigned int PJWHash(const char *str, int TableSize)
{
    unsigned int BitsInUnignedInt = (unsigned int)(sizeof(unsigned int) * 8);
    unsigned int ThreeQuarters    = (unsigned int)((BitsInUnignedInt  * 3) / 4);
    unsigned int OneEighth        = (unsigned int)(BitsInUnignedInt / 8);
    unsigned int HighBits         = (unsigned int)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
    unsigned int hash             = 0;
    unsigned int test             = 0;

    while (*str)
    {
        hash = (hash << OneEighth) + (*str++);
        if ((test = hash & HighBits) != 0)
        {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return (hash % TableSize);
}

// 6.BKDR Hash Function
unsigned int BKDRHash(const char *str, int TableSize)
{
    unsigned int seed = 131;
    unsigned int hash = 0;

    while (*str)
    {
        hash = hash * seed + (*str++);
    }

    return (hash % TableSize);
}

// 7.DJB Hash Function
unsigned int DJBHash(const char *str, int TableSize)
{
    unsigned int hash = 5381;

    while (*str)
    {
        hash += (hash << 5) + (*str++);
    }

    return (hash % TableSize);
}

// 8.AP Hash Function
unsigned int APHash(const char *str, int TableSize)
{
    unsigned int hash = 0;
    int i;

    for (i=0; *str; i++)
    {
        if ((i & 1) == 0)
        {
            hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
        }
        else
        {
            hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
        }
    }

   return (hash % TableSize);
}

#endif // HASH_H_INCLUDED
