iopriostat
==========

Takes no arguments. Shows a breakdown of processes by their Linux IO scheduling
class and priority level within that class.

Sample usage:

```
$ ionice -c3 ./iopriostat
none:        total 95
idle:        total 1
best-effort: total 2 by-level    2    0    0    0    0    2    0    0
realtime:    total 0 by-level    0    0    0    0    0    0    0    0
```

ioprio.h shamelessly copied from Jens Axboe's ionice.c (util-linux-ng)
