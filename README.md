iopriostat
==========

Takes no arguments. Shows a breakdown of processes by their Linux IO scheduling
class and priority level within that class.

Sample usage:

<pre>
$ ionice -c 3 ./iopriostat
none:        total 94
idle:        total 1 <-- this would be our iopriostat process
best-effort: total 2 by-level 2 0 0 0 0 2 0 0
realtime:    total 0 by-level 0 0 0 0 0 0 0 0
</pre>

ioprio.h shamelessly copied from Jens Axboe's ionice.c (util-linux-ng)
