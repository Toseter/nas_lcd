/*
 * time delays functions
 */

#ifndef _DELAYS_H_
#define _DELAYS_H_


#define	MHZ	*1

#ifndef	XTAL_FREQ
#define	XTAL_FREQ	20MHZ		/* Crystal frequency in MHz */
#endif

#if	XTAL_FREQ < 8MHZ
#define	uS_CNT 	238			/* 4x to make 1 mSec */
#endif

#if	XTAL_FREQ == 8MHZ
#define uS_CNT  244
#endif

#if	XTAL_FREQ > 8MHZ
#define uS_CNT  246
#endif

#define FREQ_MULT	(XTAL_FREQ)/(4MHZ)

/* Time delay in usec */
#define	DelayUs(x)	{ unsigned int _dcnt; \
			  if(x>=4) _dcnt=(x*(FREQ_MULT)/16); \
			  else _dcnt=1; \
			  while(--_dcnt > 0) \
				{\
				asm("nop");\
				continue; }\
		}


/* Time delays in msec */
void DelayMs(unsigned int cnt) {
	unsigned int i;
	while (cnt--) {
		i=4;
		while(i--) {
			DelayUs(uS_CNT);	/* Adjust for error */
		} ;
	} ;
}

#endif /* _DELAYS_H_ */