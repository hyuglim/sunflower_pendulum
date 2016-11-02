#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "sf-types.h"
#include "tag.h"
#include "devsim7708.h"
#include "sh7708.h"
#include "devscc.h"
#include "devrtc.h"
#include "devexcp.h"
#include "devlog.h"
#include "devloc.h"
#include "devsensor.h"
#include "misc.h"
#include "fault.h"
#include "physics.h"
#include "interrupts-hitachi-sh.h"
#include "network-hitachi-sh.h"
#include "print.h"
#include "batt.h"

#include "sift-mac.h"

enum
{
	PENDULUM_SENSOR = 0,
	TRUE = 1,
	FALSE = 0,
};


#ifdef LOGMARKERS
#	include "logmarkers.h"
#endif


/*	    8 GPRs + PR		*/
uchar		REGSAVESTACK[36];
SiftState	*SIFT_MAC;

static void		hdlr_install(void);
static void		write_log(ulong period);

void
startup(int argc, char *argv[])
{

	SIFT_MAC = NULL;
	hdlr_install();

    ulong start_time = devrtc_getusecs();
    int total_seconds = 5;
    
    volatile double prev_x_acceleration = (double) devloc_getxloc();
    int swing_count = 0;

    while (devrtc_getusecs() < start_time + 1000000 * total_seconds) {
        volatile double x_acceleration = (double) devloc_getxloc();
        printf("x_acceleration value is %f\n", x_acceleration);

        if (prev_x_acceleration * x_acceleration < 0) {
            swing_count++;
        }

        if (x_acceleration != 0) { // do not double count the change
            prev_x_acceleration = x_acceleration;
        }
    }
    
    printf("detected %d swings in the pendulum\n", swing_count);

	return;		
}


void
nic_hdlr(int evt)
{
	int		whichifc;
	SiftFrame	*payload;
	ulong 		timestamp, now;


    /*      Lower 12 bits of interrupt code specify IFC #   	*/
    whichifc = evt & 0xFFF;
	
NETTRACEMARK(8);
LOGMARK(0);
	/*	Let the MAC layer take care of whatever it needs to	*/
	sift_nichdlr(SIFT_MAC, whichifc);
LOGMARK(1);
NETTRACEMARK(9);

	/*								*/
	/*	Now, retrieve the payload from the MAC layer, if any	*/
	/*								*/
	payload = sift_receive(SIFT_MAC, FRAME_DATA);
	if (payload == NULL)
	{
        printf("nic_hdlr payload is null\n");

		return;
	}

	now = devrtc_getusecs();

	/*	There will always be some old frames, since broadcast leads to loops	*/
	memmove(&timestamp, payload->data, 4);
	sift_freeframe(payload);

	return;
}

void
intr_hdlr(void)
{
	int	evt = devexcp_getintevt();


	/*	Only call nic_hdlr() for RX_OK interrupts	*/
	if ((evt >= NIC_RX_EXCP_CODE) && (evt < NIC_RX_EXCP_CODE_END))
	{
		/*	Only begin triggering when sift_init is done	*/
		if (SIFT_MAC != NULL)
		{
			nic_hdlr(evt);
		}
	}
	else if (evt == TMU0_TUNI0_EXCP_CODE)
	{
	}

	return;
}

void
hdlr_install(void)
{
	extern	uchar	vec_stub_begin, vec_stub_end;
	uchar	*dstptr = (uchar *)0x8000600;
	uchar	*srcptr = &vec_stub_begin;


	/*	Copy the vector instructions to vector base	*/
	while (srcptr < &vec_stub_end)
	{
		*dstptr++ = *srcptr++;
	}

	return;
}

void
fatal(char *str)
{
	lprint("fatal method called %s", str); 
	exit(-1);
}

