# Info
- Add a copy of `pico_sdk_import.cmake` to each project folder.
- A `CMakeLists.txt` file is required for each project, to specify library locations, build-time defines, and other compiler/linker settings.

## Build Commands
- All build commands should be run in a `/build` folder inside the project, to keep all generated build files in one place.

- After a build, the `/build/PROJECT_NAME.uf2` executable can be copied into a mounted Pico drive to write (hold `BOOTSEL` button on Pico while connecting with USB).

## CMake
```sh
## CMake for Pico W, standard build:
mkdir build && cd build
cmake -DPICO_BOARD=pico_w -DPICO_SDK_PATH=~/Pico/pico-sdk/ ..

# CMake for Pico W, with WiFi credentials:
mkdir build && cd build
cmake -DPICO_BOARD=pico_w -DPICO_SDK_PATH=~/Pico/pico-sdk/ -DWIFI_SSID="_SSID_" -DWIFI_PASSWORD="_PASSWORD_" ..
```

## Make
```sh
## After building, run `make`:
make

## Or, run `make` with 2 CPU cores:
make -j2
```

## Mount and copy a UF2 firmware while connected to a Raspberry Pi via USB:
```sh
# Create a mount directory if none exist
sudo mkdir /mnt/pico

# Look up USB device name (usually `sda1`, `sdb1`, etc.)
sudo fdisk -l

# Mount the drive
sudo mount /dev/sda1 /mnt/usb

# Copy the UF2 file into the mounted drive
sudo cp PATH_TO_UF2 /mnt/pico
```

---

# MicroPython

## Installing
- Download the [MicroPython UF2 file (for Pico W)](https://micropython.org/download/rp2-pico-w/rp2-pico-w-latest.uf2).
- Hold `BOOTSEL` button on Pico W while connecting to PC or Pi by USB.
- Drag the MicroPython UF2 file (`firmware.uf2`) onto the Pico W (or mount USB device and copy).
- A Python REPL will now be accessible via USB Serial (at `/dev/ttyACM0` on Linux).
- Install and run `minicom` to access the serial port and launch the REPL:
  - `sudo apt install minicom`
  - `minicom -o -D /dev/ttyACM0`
  - Press Ctrl+D (`^D`) to restart MicroPython from the `minicom` REPL.
- VSCode extension: [MicroPico](https://marketplace.visualstudio.com/items?itemName=paulober.pico-w-go)

## References
- [Connecting to the Internet with Raspberry Pi Pico W (Section 3.1)](https://datasheets.raspberrypi.com/picow/connecting-to-the-internet-with-pico-w.pdf)
- [Pico Python SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-python-sdk.pdf)
- [MicroPython Wiki](https://github.com/micropython/micropython/wiki)