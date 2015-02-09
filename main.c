
/*
 * DVB API Profiler by Greg Wickham is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.
 */
#include    <glibmm.h>

#include    "ofdm.h"

static Glib::RefPtr<Glib::MainLoop> loop;

int main( int argc, char *argv[] ) {

    Adapter *adapter[ 4 ];

    loop = Glib::MainLoop::create( false );

    adapter[ 0 ] = new OFDM( 2, 1, 226500000 );
    adapter[ 1 ] = new OFDM( 3, 4, 177500000 );
    adapter[ 3 ] = new OFDM( 4, 7, 219500000 );
    adapter[ 2 ] = new OFDM( 5, 10, 557625000 );

    loop->run();

}

/* END OF FILE */
