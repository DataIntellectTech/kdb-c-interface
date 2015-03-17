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

// KXVER needs to be defined before the k.h header file is
// included. Setting this to 2 should allow this code to be
// compiled for and used with version 2.x.
#define KXVER 3
#include "k.h"

// macro to fetch a single byte from an array.
#define charfrom(x) x[rand()%(sizeof((x)-1))]

// macro to return the length of any array that is statically
// allocated at compile time. (e.g alpha and types below)
#define arraylen(x) (sizeof(x)/(sizeof(*(x))))

static const char alpha[] = "abcdefghijklmnopqrstuvwxyz";
static const char types[] = "bxhijefcs";

////
// Takes a reference to a buffer and a size and populates the
// buffer with random alphabetic characters up until size
// characters have been written. Ensures that the last byte
// written to the buffer is a null/terminating character.
//
static char* gensym(char *buffer, size_t size)
{
    int i;
    for (i = 0; i < size; i++)
        buffer[i] = charfrom(alpha);
    buffer[i] = '\0';

    return buffer;
}

////
// Takes a symbol as a parameter that represents the short
// name of the type and creates a random atom with that type.
//
// e.g. makeatom[`i] will create an integer in q
//      makeatom[`c] will crreate a character in q
//
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
        case 'p': {
            long long year = rand() % 10 + 2010;
            long long month = rand() % 12 + 1;
            long long day = rand() % 28 + 1;
            long long hour = rand() % 24;
            long long min = rand() % 60;
            long long sec = rand() % 60;
            long long nano = rand() % 1000000000;
            long long tq = (((ymd(year,month,day)*24+hour)*60+min)*60+sec)*1000000000+nano;
            return ktj(-KP, tq); }
        case 'm': {
            long long year = rand() % 10 + 2010;
            long long month = rand() % 12 + 1;
            K result = ka(-KM);
            result->i = (year-2000)*12+month-1;
            return result; }
        case 'd': {
            long long year = rand() % 10 + 2010;
            long long month = rand() % 12 + 1;
            long long day = rand() % 28 + 1;
            return kd(ymd(year,month,day)); }
        case 'z': {
            double year = rand() % 10 + 2010;
            double month = rand() % 12 + 1;
            double day = rand() % 24;
            double hour = rand() % 24;
            double min = rand() % 60;
            double sec = rand() % 60;
            double mili = rand() % 1000;
            return kz(ymd(year,month,day)+(hour+(min+(sec+mili/1000)/60)/60)/24); }
        case 'n': {
            long long day = rand() % 10;
            long long hour = rand() % 24;
            long long min = rand() % 60;
            long long sec = rand() % 60;
            long long nano = rand() % 1000000000;
            return ktj(-KN, (((day*24+hour)*60+min)*60+sec)*1000000000+nano); }
        case 'u': {
            int min = rand() % 60;
            int sec = rand() % 60;
            K result = ka(-KU);
            result->i = min*60+sec;
            return result; }
        case 'v': {
            int hour = rand() % 24;
            int min = rand() % 60;
            int sec = rand() % 60;
            K result = ka(-KV);
            result->i = (hour*60+min)*60+sec;
            return result; }
        case 't': {
            int hour = rand() % 24;
            int min = rand() % 60;
            int sec = rand() % 60;
            int mili = rand() % 1000;
            return kt(((hour*60+min)*60+sec)*1000+mili); }
    }

    return krr("notimplemented");
}

#define makeatoms(t,a,v) do { result = ktn(t,5); for (int i = 0; i < 5; i++) a(result)[i] = (v); } while (0)

////
// Creates a list of 5 atoms based on the character that is passed in as a parameter.
// If the input character doesn't match a type, then a 'notimplemented' error will be
// thrown.
//
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
        default: return krr("notimplemented");
    }

    return result;
}

////
// Creates a dictionary with a list of each of the kdb+ atomic types as values. The
// lists created are of length 5.
//
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

////
// Takes a symbol as a parameter and attempts to create a random kdb+ type
// that matches.
//
// e.g makeq[`i] will create an integer
//     makeq[`I] will create an integer list
//     makeq[`dictionary] will create a dictionary
//     makeq[`table] will create a table
//
K makeq(K x)
{
    if (xt != -11) return krr("type");

    if (0 == strcmp(x->s, "dictionary")) return makedict(x);
    else if (0 == strcmp(x->s, "table")) return xT(makedict(x));
    else if (strlen(x->s) != 1) return krr("*notimplemented");

    if (isupper(x->s[0])) return makelist(x->s[0]);
    else return makeatom(x->s[0]);
}
