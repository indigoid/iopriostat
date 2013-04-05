iopriostat
==========

Takes no arguments, shows a breakdown of processes by their Linux IO scheduling
class and priority level within that class.

Sample usage:

$ ./iopriostat
93 processes with no scheduling class
No processes with real-time IO priority
2 processes with best-effort IO priority
        2 processes at best-effort priority 3
No processes with idle IO priority

ioprio.h shamelessly copied from Jens Axboe's ionice.c (util-linux-ng)
