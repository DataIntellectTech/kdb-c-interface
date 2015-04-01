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

#define KXVER 3
#include "k.h"

K printq(K x);

static void fmt_time(char *str, time_t time, int adjusted)
{
    static char buffer[4096];

    struct tm *timeinfo = localtime(&time);
    if (adjusted) timeinfo->tm_hour -= 1;
    strftime(buffer, sizeof(buffer), str, timeinfo);

    printf("%s ", buffer);
}

static K printatom(K x)
{
    switch (xt) {
        case  -1: printf("%db", x->g); break;
        case  -4: printf("0x%02x", x->g); break;
        case  -5: printf("%d", x->h); break;
        case  -6: printf("%d", x->i); break;
        case  -7: printf("%lld", x->j); break;
        case  -8: printf("%.2f", x->e); break;
        case  -9: printf("%.2f", x->f); break;
        case -10: printf("\"%c\"", x->g); break;
        case -11: printf("`%s", x->s); break;
        case -12: fmt_time("%Y.%m.%dD%H:%M:%S.", ((x->j) / 8.64e13 + 10957)*8.64e4, 0); break;
        case -13: printf("%04d.%02d", (x->i)/12+2000, (x->i)%12+1); break;
        case -14: fmt_time("%Y.%m.%d", ((x->i) + 10957)*8.64e4, 0); break;
        case -15: fmt_time("%Y.%m.%dD%H:%M:%S", ((x->f) + 10957)*8.64e4, 0); break;
        case -16: { fmt_time("%jD%H:%M:%S", (x->j)/1000000000, 1);
                    printf(".%09lld", (x->j)%1000000000); break; }
        case -17: fmt_time("%H:%M", (x->i) * 60, 1); break;
        case -18: fmt_time("%H:%M:%S", x->i, 1); break;
        case -19: { fmt_time("%H:%M:%S", (x->i) / 1000, 1);
                    printf(".%03d", (x->i)%1000); break; }
        default: return krr("notimplemented");
    }

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
        printitem(keys, row);
        printf("| ");
        printitem(data, row);
        if (row < keys->n - 1) printf("\n");
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

    for (int i = 0; i < colcount; i++)
        printf("%s\t", kS(columns)[i]);
    printf("\n");

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
