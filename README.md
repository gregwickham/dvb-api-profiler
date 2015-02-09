
Linux DVB API Profiler

This is an application that profiles the duration of system calls used by the linux DVB API. It can
drive multiple adapters concurrently.

The application is written in C++ and uses glibmm for the event driven framework.

This 0.1 release only has support for OFDM and accepts no command line arguments.

To run a test, edit main.c :

This line defines the number of adapters that will be concurrently tested.

    Adapter *adapter[ 4 ];

Then for each adapter, define a line similar to:

    adapter[ 0 ] = new OFDM( 2, 1, 226500000 );
      /*
        2 = index of adapter (currently only the primary tuner is supported '0'
             maps to /dev/dvb/adapter<index>/frontend0 (and dvr0 and demux0)
        
        1 = delay (in seconds) before starting this adapter
        
        226500000 = Frequency in Hz of the DVB-T (OFDM) station to tune
      */

In the file ofdm.c there is an '#if 0' that is used to select between the original
DVB API (FE_SET_FRONTEND) and the new format (FE_SET_PROPERTY).

NOTE: The bandwidth of the OFDM channel is hardcoded (currently at 7Mhz). This will need
to be changed if your broadcasts are different


 -Greg
