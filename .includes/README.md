Files in this `.includes` folder are copies of library files that cannot be found automatically at build time, or edits of files that have errors.


## `lwipopts_common.h`
- This is a configurable file, so Make does not get it automatically.
- Add a `#include "lwipopts_common.h"` line to a project-specific `lwipopts.h` file, or copy and rename this file into the project folder.
- Results in `fatal error: lwipopts.h: No such file or directory` when trying to use CYW43 or LWIP includes.
  - Via `pico-sdk/lib/lwip/src/include/lwip/opt.h`

## `arch/sys_arch.h`
- *Only used for NO_SYS=0?*
- Make cannot locate this file.
- Results in `fatal error: arch/sys_arch.h: No such file or directory` when trying to use CYW43 or LWIP includes.
  - Via `pico-sdk/lib/lwip/src/include/lwip/timeouts.h` -> `pico-sdk/src/rp2_common/pico_lwip/lwip_nosys.c` -> `pico-sdk/lib/lwip/src/include/lwip/sys.h`
