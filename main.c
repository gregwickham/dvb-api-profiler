
/*
 * DVB API Profiler by Greg Wickham is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.
 */
#include    <glibmm.h>

#include    "ofdm.h"
#include    "qpsk.h"

static Glib::RefPtr<Glib::MainLoop> loop;

int main( int argc, char *argv[] ) {

    Adapter *adapter[ 4 ];

    loop = Glib::MainLoop::create( false );

    adapter[ 0 ] = new QPSK( 0, 1, 1062000, 27800000, SEC_VOLTAGE_18 );
    adapter[ 1 ] = new QPSK( 1, 1, 1062000, 27800000, SEC_VOLTAGE_18 );
#if 0
    adapter[ 3 ] = new OFDM( 4, 7, 219500000 );
    adapter[ 2 ] = new OFDM( 5, 10, 557625000 );
#endif

    loop->run();

}

/* END OF FILE */
