#ifndef _w32_IDNA_H
#define _w32_IDNA_H

#include <stdlib.h>

//#include "font.h"
#define STB_SUPPORT_UNICODE_FONT
#ifdef STB_SUPPORT_UNICODE_FONT

typedef long  ucs4_t;

typedef struct {
        int istate;
      } *conv_t;

typedef struct {
        unsigned short  indx;  /* index into big table */
        unsigned short  used;  /* bitmask of used entries */
      } Summary16;


int gb2312_mbtowc (ucs4_t *pwc, const unsigned char *s, int n);//gb2312 to unicode
int gb2312_wctomb (unsigned char *r, ucs4_t wc, int n);    //unicode to gb2312


#define RET_ILUNI      -1
#define RET_ILSEQ      -2
#define RET_TOOSMALL   -3
#define RET_TOOFEW(n) (-4-(n))

#endif

#endif  /* _w32_IDNA_H */
