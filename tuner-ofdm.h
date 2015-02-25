#ifndef _TUNER_OFDM_H_
#define _TUNER_OFDM_H_

/*
 * DVB API Profiler by Greg Wickham is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.
 */

#include    "adapter.h"

class TunerOFDM : public Adapter {

    public:
     
        TunerOFDM( int index, int delay, int frequency );

    protected:
        
        int tune( void );

    private:

        int         _frequency;

};

#endif
