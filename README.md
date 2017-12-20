Remeha Boiler Logger
====================

This is an application I've written to log data from my Remeha Avanta Plus 28c
boiler (shown as `Avanta Plus V1 (P2)` in Recom) to syslog. I'm sharing it in
the hope that it proves useful to anyone else (especially as the info online
about the boiler is pretty sparse).

I'm compiling with GCC and running it on a Raspberry Pi, but my aim has been
to keep the bulk of the code portable. Feel free to raise pull requests to
fix any portability issues you encounter.

I've tried to structure the classes in a fairly sensible way to make it easier
to extend to work with other similar Remeha boilers.  Please also feel free to
raise PRs for this.

convert.pl takes data files which are distributed with Remeha's Windows-only
[Recom software](http://www.recom-software.com/index.php?id=207) and generates
the packed bitfields and code used in the application to extract values from
chunks of data returned by the boiler.
