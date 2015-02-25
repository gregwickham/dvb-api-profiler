#ifndef _QPSK_H_
#define _QPSK_H_

#include    <linux/dvb/frontend.h>

/*
 * DVB API Profiler by Greg Wickham is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.
 */

#include    "adapter.h"

class QPSK : public Adapter {

    public:
     
        QPSK( int index, int delay, int frequency, int sr, fe_sec_voltage_t pol );

    protected:
        
        int tune( void );

    private:

        int                 _frequency;
        int                 _sr;
        fe_sec_voltage_t    _pol;

};

#endif
