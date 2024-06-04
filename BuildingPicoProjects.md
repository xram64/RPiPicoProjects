# Info
- Add a copy of `pico_sdk_import.cmake` to each project folder.


# Build commands
Run commands from within the project folder.

## Pico W - Standard build:
```sh
cmake -DPICO_BOARD=pico_w -DPICO_SDK_PATH=~/git/pico-sdk/ .
```

## Pico W - With WiFi connection:
```sh
cmake -DPICO_BOARD=pico_w -DPICO_SDK_PATH=~/git/pico-sdk/ -DWIFI_SSID="_SSID_" -DWIFI_PASSWORD="_PASSWORD_" .
```

## After building, run make:
```sh
make
```