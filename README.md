# Micro Processors Project

All code from the TU857 Y2 microprocessors project (CMPU1013)

**A dungeon crawler game for STM32 Nucleo F031K6 with ST7735S display.**

Features complete audio system with background music, dynamic tempo adjustment, and sound effects:
- Background music for menu, gameplay, victory, and defeat
- Music speeds up as timer counts down for added urgency
- Sound effects for coin collection and menu navigation

📖 **For detailed technical documentation, see [SUMMARY.md](SUMMARY.md)**

Provides 2 targets: release, debug.
Debug builds are very slow fyi.

## Not using VSCode?

### Note

In order to take intellisense from this you must run `make init` on first init

### Building

This project uses PlatformIO as a baseline tool and also provides a Makefile to make things easier

- Build: `make build`
- Upload to a board: `make upload`
- Build image files: `make images`
