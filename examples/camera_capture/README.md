# camera_capture

(Fictional demo content; see NOTICE.md.)

Initializes an AcmeTech Cameras Pro-37 on the board's camera connector and
captures a frame every second: QVGA on the Pro-29 EVK, VGA on the Pro-100 EVK.

If `acam37_init` fails it prints the status split into module and code. See
`include/atp_status.h` for how to read it.
