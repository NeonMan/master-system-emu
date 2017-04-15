Attempt at keeping track what VDP functionalities should be implemented and
which are being tested.

| Req. ID       | Tested? | Comments                                          |
| ------------- |:-------:| ------------------------------------------------- |
| VDP-100       |   NO    | Control port writes are in two byte pairs.        |
| VDP-101       |   NO    | Byte count flag is set on 1st byte, reset on 2nd. |
| VDP-200       |   NO    | Control port reads reset the byte counter.        |
| VDP-300       |   NO    | Data port read/write reset the byte counter.      |
