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

static void	hdlr_install(void);

void
startup(int argc, char *argv[])
{

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
intr_hdlr(void)
{
	int	evt = devexcp_getintevt();


	/*	Only call nic_hdlr() for RX_OK interrupts	*/
	if ((evt >= NIC_RX_EXCP_CODE) && (evt < NIC_RX_EXCP_CODE_END))
	{
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

