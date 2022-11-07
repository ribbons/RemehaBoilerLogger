Remeha Boiler Logger
====================

A service to decode and log sensor data from a Remeha Avanta Plus 28c to
syslog.

I wrote this application to log data from my boiler (shown as `Avanta Plus V1
(P2)` in Recom). I'm sharing it in the hope that it proves useful to someone
else (especially as the info online about the boiler is pretty sparse).


Compile
-------

You'll need CMake and Boost.Test installed.  For instance, on Debian, Ubuntu
or Raspberry Pi OS you can run the following command to install them:

``` sh
sudo apt-get install cmake libboost-test-dev
```

Once they are installed you can run the following commands to build everything:

``` sh
cmake -S . -B build
cmake --build build
```


Contribute
----------

I'm compiling with GCC and running the service on a Raspberry Pi, but my aim
has been to keep the bulk of the code portable. Feel free to raise pull
requests to fix any portability issues you encounter.

I've tried to structure the classes in a fairly sensible way to make it easier
to extend to work with other similar Remeha boilers.  Please also feel free to
raise PRs to add support for other boilers.

convert.pl takes data files which are distributed with Remeha's Windows-only
[Recom software](http://www.recom-software.com/index.php?id=207) and generates
the packed bitfields and code used in the application to extract values from
chunks of data returned by the boiler.
