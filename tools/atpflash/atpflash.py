#!/usr/bin/env python3
"""atpflash - write an application image to an AcmeTech Pro evaluation kit.

FICTIONAL DEMO CONTENT. AcmeTech and this SDK do not exist. See NOTICE.md.
This script talks to no real hardware.

Exit codes:
    0  flashed
    2  no board found on any serial port
    3  image does not fit in the application area
    4  board answered but is a different chip than --board
    5  --verify: the image read back differs from the file (since 2.1.0)
"""

import argparse
import sys
from pathlib import Path

# Application area per board. The bootloader occupies everything below APP_BASE.
BOARDS = {
    "pro29_evk": {"chip_id": 0x0029, "app_base": 0x0000_4000, "flash_end": 0x0008_0000},
    "pro100_evk": {"chip_id": 0x0100, "app_base": 0x0001_0000, "flash_end": 0x0020_0000},
}

EXIT_OK = 0
EXIT_NO_BOARD = 2
EXIT_TOO_LARGE = 3
EXIT_WRONG_CHIP = 4
EXIT_VERIFY_FAILED = 5


def find_board(port: str | None) -> tuple[str, int] | None:
    """Return (port, chip_id) of the first board that answers, or None."""
    return None  # demo: there is no hardware to find


def verify(port: str, base: int, image: bytes) -> bool:
    """Read len(image) bytes back from base and compare."""
    return True  # demo: nothing to read back


def main() -> int:
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("image", type=Path, help="raw binary built for the application area")
    p.add_argument("--board", choices=sorted(BOARDS), required=True)
    p.add_argument("--port", help="serial port; default: try each one")
    p.add_argument("--verify", action="store_true", help="read the image back and compare")
    args = p.parse_args()

    board = BOARDS[args.board]
    size = args.image.stat().st_size
    room = board["flash_end"] - board["app_base"]
    if size > room:
        print(f"image is {size} bytes; the {args.board} application area holds {room}", file=sys.stderr)
        return EXIT_TOO_LARGE

    found = find_board(args.port)
    if found is None:
        print("no AcmeTech board found. Is the USB cable in the DEBUG port, not POWER?", file=sys.stderr)
        return EXIT_NO_BOARD
    port, chip_id = found
    if chip_id != board["chip_id"]:
        print(f"{port} is chip 0x{chip_id:04x}, not {args.board}", file=sys.stderr)
        return EXIT_WRONG_CHIP

    print(f"flashing {size} bytes to {port} at 0x{board['app_base']:08x}")
    if args.verify and not verify(port, board["app_base"], args.image.read_bytes()):
        print("verify failed: the board's flash differs from the image", file=sys.stderr)
        return EXIT_VERIFY_FAILED
    return EXIT_OK


if __name__ == "__main__":
    sys.exit(main())
