////
// printq.c
//
// A simple example of how to access and print the
// different K object types from C. Handles atoms, lists
// dictionaries and tables (enumerated types and functions
// are currently unsupported).
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

K printq(K x);

static inline int fmt_time(char *buf, size_t size, char *str, time_t time, int adjusted)
{
    struct tm *timeinfo = localtime(&time);
    if (adjusted) timeinfo->tm_hour -= 1;
    return strftime(buf, size, str, timeinfo);
}

// Takes any kdb+ atom type and prints it to std out followed by the delimiter character
// that is provided as the second argument. If a type is not printable a 'notimplemented'
// error will be thrown back to kdb+.
static K printatom(K x)
{
    static char buffer[4096];
 
    switch (xt) {
        case  -1: snprintf(buffer, 4096, "%db", x->g); break;
        case  -4: snprintf(buffer, 4096, "0x%02x", x->g); break;
        case  -5: snprintf(buffer, 4096, "%d", x->h); break;
        case  -6: snprintf(buffer, 4096, "%d", x->i); break;
        case  -7: snprintf(buffer, 4096, "%lld", x->j); break;
        case  -8: snprintf(buffer, 4096, "%.2f", x->e); break;
        case  -9: snprintf(buffer, 4096, "%.2f", x->f); break;
        case -10: snprintf(buffer, 4096, "\"%c\"", x->i); break;
        case -11: snprintf(buffer, 4096, "`%s", x->s); break;
        case -12: fmt_time(buffer, 4096, "%Y.%m.%dD%H:%M:%S.", ((x->j)/8.64e13+10957)*8.64e4, 0); break;
        case -13: snprintf(buffer, 4096, "%04d.%02d", (x->i)/12+2000, (x->i)%12+1); break;
        case -14: fmt_time(buffer, 4096, "%Y.%m.%d", ((x->i)+10957)*8.64e4, 0); break;
        case -15: fmt_time(buffer, 4096, "%Y.%m.%dD%H:%M:%S", ((x->f)+10957)*8.64e4, 0); break;
        case -16: { int pos = fmt_time(buffer, 4096, "%jD%H:%M:%S", (x->j)/1000000000, 1);
                  snprintf(buffer + pos, 4096 - pos, ".%09lld", (x->j)%1000000000); break; }
        case -17: fmt_time(buffer, 4096, "%H:%M", (x->i) * 60, 1); break;
        case -18: fmt_time(buffer, 4096, "%H:%M:%S", x->i, 1); break;
        case -19: { int pos = fmt_time(buffer, 4096, "%H:%M:%S", (x->i) / 1000, 1);
                  snprintf(buffer + pos, 4096 - pos, ".%03d", (x->i)%1000); break; }
        default: return krr("notimplemented");
    }
 
    printf("%s ", buffer);
    return (K) 0;
}

#define showatom(c,a,x,i) do { K r = c(a((x))[i]); printatom(r); r0(r); } while(0)

static K printitem(K x, int index)
{
    switch (xt) {
        case  0: printq(kK(x)[index]); break;
        case  1: showatom(kb, kG, x, index); break; 
        case  4: showatom(kg, kG, x, index); break;
        case  5: showatom(kh, kH, x, index); break;
        case  6: showatom(ki, kI, x, index); break;
        case  7: showatom(kj, kJ, x, index); break;
        case  8: showatom(ke, kE, x, index); break;
        case  9: showatom(kf, kF, x, index); break;
        case 10: showatom(kc, kC, x, index); break;
        case 11: showatom(ks, kS, x, index); break;
        case 14: showatom(kd, kI, x, index); break;
        case 15: showatom(kz, kF, x, index); break;
        default: return krr("notimplemented");
    }
    
    return (K) 0;
}

static K printlist(K x)
{
    if (x->n == 1) printf(",");
    
    for (int i = 0; i < x->n; i++)
        printitem(x, i);
 
    return (K) 0;
}

static K printdict(K x)
{
    K keys = kK(x)[0];
    K data = kK(x)[1];
    
    for (int row = 0; row < keys->n; row++) {
        printf("%s\t| ", kS(keys)[row]);
        printitem(data, row);
        if (row < keys->n - 1)printf("\n");
    }
    return (K) 0;
}

static K printtable(K x)
{
    K flip = ktd(x);
    K columns = kK(flip->k)[0];
    K rows = kK(flip->k)[1];

    int colcount = columns->n;
    int rowcount = kK(rows)[0]->n;
 
    // print out the table headers separated by tabs.
    for (int i = 0; i < colcount; i++)
        printf("%s\t", kS(columns)[i]); 
    printf("\n");

    // print out each of the table rows followed by
    // a new line.
    for (int i = 0; i < rowcount; i++) {
        for (int j = 0; j < colcount; j++) {
           printitem(kK(rows)[j], i);
           printf("\t");
        }
        printf("\n");
    }
 
    return (K) 0; 
}

K printq(K x)
{
    K result;

    if (xt < 0) result = printatom(x);
    else if ((xt >= 0) && (xt < 20)) result = printlist(x);
    else if (xt == 98) result = printtable(x);
    else if (xt == 99) result = printdict(x);
    else result = krr("notimplemented");
    
    printf("\n");
    return result;
}
