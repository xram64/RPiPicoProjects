Files in this `.includes` folder are copies of library files that cannot be found automatically at build time, or edits of files that have errors.


## `lwipopts.h`
- This is a configurable file, so make does not get it automatically(?)
- Results in `fatal error: lwipopts.h: No such file or directory` when trying to use CYW43 or LWIP includes.
  - Via `pico-sdk/lib/lwip/src/include/lwip/opt.h`

## `arch/sys_arch.h`
- Make cannot locate this file.
- Results in `fatal error: arch/sys_arch.h: No such file or directory` when trying to use CYW43 or LWIP includes.
  - Via `pico-sdk/lib/lwip/src/include/lwip/timeouts.h` -> `pico-sdk/src/rp2_common/pico_lwip/lwip_nosys.c` -> `pico-sdk/lib/lwip/src/include/lwip/sys.h`
