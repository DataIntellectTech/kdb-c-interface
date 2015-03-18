////
// makeq.c
//
// A simple example of how to create K objects within C. Handles
// atoms, lists, dictionaries and tables. (enumerated types, functions
// and nested types are currently unsupported).
//
// @updated: 17/03/2015
// @authors: Kent Lee, Kevin Piar, Mark Rooney
//
// AquaQ Analytics
// kdb+ consultancy, training and support
//
// For questions, comments, requests or bug reports, please contact us
// w: www.aquaq.co.uk
// e: info@aquaq.co.uk
// p: +44 (0)28 9051 1232

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#define KXVER 3
#include "k.h"

#define charfrom(x) x[rand()%(sizeof((x)-1))]
#define arraylen(x) (sizeof(x)/(sizeof(*(x))))

static const char alpha[] = "abcdefghijklmnopqrstuvwxyz";
static const char types[] = "bxhijefcs";

static char* gensym(char *buffer, size_t size)
{
    int i;
    for (i = 0; i < size; i++)
        buffer[i] = charfrom(alpha);
    buffer[i] = '\0';

    return buffer;
}

static long long randtimestamp()
{
    long long year = rand() % 10 + 2010;
    long long month = rand() % 12 + 1;
    long long day = rand() % 28 + 1;
    long long hour = rand() % 24;
    long long min = rand() % 60;
    long long sec = rand() % 60;
    long long nano = rand() % 1000000000;

    return (((ymd(year,month,day)*24+hour)*60+min)*60+sec)*1000000000+nano;
}

static long long randdate()
{
    long long year = rand() % 10 + 2010;
    long long month = rand() % 12 + 1;
    long long day = rand() % 28 + 1;
 
    return ymd(year, month, day);
}

static double randdatetime()
{
    double year = rand() % 10 + 2010;
    double month = rand() % 12 + 1;
    double day = rand() % 24;
    double hour = rand() % 24;
    double min = rand() % 60;
    double sec = rand() % 60;
    double mili = rand() % 1000;
    return ymd(year,month,day)+(hour+(min+(sec+mili/1000)/60)/60)/24;
}

static long long randtimespan()
{
    long long day = rand() % 10;
    long long hour = rand() % 24;
    long long min = rand() % 60;
    long long sec = rand() % 60;
    long long nano = rand() % 1000000000;
    return (((day*24+hour)*60+min)*60+sec)*1000000000+nano; 
}

static long long randtime()
{
    int hour = rand() % 24;
    int min = rand() % 60;
    int sec = rand() % 60;
    int mili = rand() % 1000;
    return ((hour*60+min)*60+sec)*1000+mili;
}

static long long randmonth()
{
    long long year = rand() % 10 + 2010;
    long long month = rand() % 12 + 1;
    return (year-2000)*12+month-1;
}

static long long randminute()
{
    int min = rand() % 60;
    int sec = rand() % 60;
    return min*60+sec;
}

static long long randsecond()
{
    int hour = rand() % 24;
    int min = rand() % 60;
    int sec = rand() % 60;
    return (hour*60+min)*60+sec;
}

static K makeatom(char ch)
{
    static char buffer[32];

    switch (ch) {
        case 'b': return kb(rand() % 2);
        case 'x': return kg(rand());
        case 'h': return kh(rand() % 100);
        case 'i': return ki(rand() % 100);
        case 'j': return kj(rand() % 100);
        case 'e': return ke(rand() / 100.0);
        case 'f': return kf(rand() / 100.0);
        case 'c': return kc(charfrom(alpha)); 
        case 's': return ks(gensym(buffer, rand() % 32));
        case 'p': return ktj(-KP, randtimestamp()); 
        case 'm': { K result = ka(-KM); result->i = randmonth(); return result; }
        case 'd': return kd(randdate());
        case 'z': return kz(randdatetime());
        case 'n': return ktj(-KN, randtimespan());
        case 'u': { K result = ka(-KU); result->i = randminute(); return result; }
        case 'v': { K result = ka(-KV); result->i = randsecond(); return result; }
        case 't': return kt(randtime()); 
    }

    return krr("notimplemented");
}

#define makeatoms(t,a,v) do { result = ktn(t,5); for (int i = 0; i < 5; i++) a(result)[i] = (v); } while (0)

static K makelist(char ch)
{
    static char buffer[32];
    K result;

    switch (ch) {
        case 'B': makeatoms(KB, kG, rand() % 2); break;
        case 'X': makeatoms(KG, kG, rand()); break;
        case 'H': makeatoms(KH, kH, rand() % 100); break;
        case 'I': makeatoms(KI, kI, rand() % 100); break;
        case 'J': makeatoms(KJ, kJ, rand() % 100); break;
        case 'E': makeatoms(KE, kE, rand() / 100.0); break;
        case 'F': makeatoms(KF, kF, rand() / 100.0); break;
        case 'C': makeatoms(KC, kC, charfrom(alpha));  break;
        case 'S': makeatoms(KS, kS, gensym(buffer, rand() % 32)); break;
        case 'P': makeatoms(KP, kJ, randtimestamp()); break;
        case 'M': makeatoms(KM, kI, randmonth()); break;
        case 'D': makeatoms(KD, kI, randdate()); break;
        case 'Z': makeatoms(KZ, kF, randdatetime()); break;
        case 'N': makeatoms(KN, kJ, randtimespan()); break;
        case 'U': makeatoms(KU, kI, randminute()); break;
        case 'V': makeatoms(KV, kI, randsecond()); break;
        case 'T': makeatoms(KT, kI, randtime()); break;
        default: return krr("notimplemented");
    }

    return result;
}

static K makedict(K x)
{
    static char buffer[] = "t_ ";
 
    int ntypes = 9;
    K keys = ktn(KS, ntypes);
    K values = ktn(0, ntypes);
    
    for (int i = 0; i < ntypes; i++) {
        buffer[2] = types[i];
        kS(keys)[i] = ss(buffer);
        kK(values)[i] = makelist(toupper(types[i]));
    }

    return xD(keys, values);
}

K makeq(K x)
{
    if (xt != -11) return krr("type");

    if (0 == strcmp(x->s, "dictionary")) return makedict(x);
    else if (0 == strcmp(x->s, "table")) return xT(makedict(x));
    else if (strlen(x->s) != 1) return krr("*notimplemented");

    if (isupper(x->s[0])) return makelist(x->s[0]);
    else return makeatom(x->s[0]);
}
