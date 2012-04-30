#ifndef _MAIN_H
#define _MAIN_H

#define MAX_MSG         0x05                    /* Message buffer size */
#define MSG_LEN         0x10                    /* Message charecter count */

static unsigned int     pr_btns = 0;            /* contains pressed buttons */
static unsigned char    buf[MAX_MSG][MSG_LEN];  /* contains displayed messages */
static unsigned char    msg_num = 0;            /* number of displayed message*/


/* Device Version */
#define MINOR_VERSION   0x01    /* Version 1.01 */
#define MAJOR_VERSION   0x01

#define OSC_EP_SIZE		64

/* Commands */
#define READ_VERSION		0
#define COLLECT_OSC_DATA	1
#define READ_OSC_DATA		2
#define SET_OFFSET		3
#define SET_TIMESCALE		4
#define SET_SYNC		5

#endif