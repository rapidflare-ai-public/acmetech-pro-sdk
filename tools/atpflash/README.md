# atpflash

(Fictional demo content; see NOTICE.md.)

Writes an application image to an evaluation kit over its DEBUG USB port.

```
python tools/atpflash/atpflash.py --board pro29_evk build/app.bin
python tools/atpflash/atpflash.py --board pro100_evk --port /dev/ttyACM1 --verify build/app.bin
```

Link your application at the board's application base: `0x00004000` on the
Pro-29 EVK, `0x00010000` on the Pro-100 EVK. The bootloader lives below it.
See the exit codes at the top of `atpflash.py`.
