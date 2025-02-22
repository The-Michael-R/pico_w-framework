# CMake/picoW/FreeRTOS framework with WLAN enabled

This is kind of an basic and somewhat empty shell or framework which can be used
as start point for projects using WLAN and FreeRTOS with the Raspberry Pico-W
(RP2040 based) or Pico2-W (RP2350 based).

Also a debug-print function with different severity levels. The print using the
UART as well sending the messages via WLAN UTP (if connected). A simple
`netcat -luz -p 54323` listens to the messages. That means no wired connection
to the computer is needed to receive information about the program state.

And since it is useful to have the right time, the AON-Timer (always-on) is set
via SNTP to the current time after the WLAN connection is established.

An example code `task1` is implemented as task printing the time every second.
This task is used as example how to integrate own code into the framework.

# Requirements

The raspberry [pico-sdk](https://github.com/raspberrypi/pico-sdk)
(tested with [2.1.1](https://github.com/raspberrypi/pico-sdk))

As long as it is not merged into the official FreeRTOS, the raspberry port of
[FreeRTOS-Kernel](https://github.com/raspberrypi/FreeRTOS-Kernel) (tested with
[4f7299d](https://github.com/raspberrypi/FreeRTOS-Kernel/tree/4f7299d6ea746b27a9dd19e87af568e34bd65b15))

The build-environment as described in the raspberry-pico SDK should be in place
and work.

The paths to both SDKs are set in the [`CMakeLists.txt`](CMakeLists.txt).

> [!Note]
> If you see the following runtime error please update the piko-sdk to 2.1.1 or
> newer:
> ```
> [...]
> assertion "!(raw_irq_mask[get_core_num()] & gpio_mask)" failed: file "[...]/pico-sdk/src/rp2_common
> [...]
> ```

# Features

## FreeRTOS

The code is ready to start with FreeRTOS. A basic configuration for multi-core
(SMP) FreeRTOS is in place with a basic example task.

## Enabled WLAN

Basic WLAN functionality is implemented and ready to use. Only the SSID and WLAN
password needs to be provided. Either by modify the file `libs/lib/wlan/wlan.c`
or preferred by using a `src/_wlan_credentials.c` with the following content:

```c
/**
 * This file holds the SSID and the WLAN password.
 * This file must not be published (included in the .gitignore)
 */

#ifndef WLAN_CREDENTIALS_H
#define WLAN_CREDENTIALS_H

#define WLAN_SSID "Dark Helmet"
#define WLAN_PASSWORD "123456"

#endif  /* WLAN_CREDENTIALS_H */
```

The file is excluded by the [`.gitignore`](.gitignore) thus safer for commit the
code to public places like GitHub.

### SNTP

As the code has WLAN enabled I included a SNTP client using the pico AON-Timer.
The time can be obtained using the `aon_timer_get_time_calendar()` function as
shown in `libs/lib/task1/task1.c`.

## Debug messages

The framework uses a flexible debug print routine with some nice features

* Per-function configurable verbose setting. Helps to debug functions while
  keeping the other prints in place.
* Configure the verbosity while runtime using `vDebugSetSeverity()`.
* Color coded prints including core number, file, function and line number for
  easy debugging.
* Print via UART and, if connected, via UTP broadcast on port 54323 messages
  over WLAN. The port can be configured in [`CMakeLists.txt`](CMakeLists.txt).A
  simple netcat replaces the USB-connection. This might be useful if the pico-w
  is installed at a place to inconvenient for debugging.

```bash
$ netcat -luz -p 54323
--E-- C1 task1.c:vTask1Main().83 - Ping 16:54:29!
--E-- C1 task1.c:vTask1Main().83 - Ping 16:54:30!
--E-- C1 task1.c:vTask1Main().83 - Ping 16:54:31!
```

# File structure and implementing own functions

The code structure supposed to be quiet simple to understand and extend.

## Folder structure

All _user_ functions supposed to live in `libs/include/<YourFunct>/` for the
h-files and `libs/lib/<YourFunct>/` for the c-files.

Your task-related functions are called in three stages by the `src/main.c` file
as mentioned below. Additional functions related to the task might be placed
along the task-related files but they need to be listed in the `CMakeLists.txt`
file as well.

## cmake file structure

No panic about the many cmake related files all over the place! Extending the
function only needs minor changes/understanding.

The top-level configuration to configure the project name, target board
(pico-w or pico2-w), host-name and SDK-locations are in the
[`CMakeLists.txt`](CMakeLists.txt). For starter only changes are required in the
top part of that file. I tried to comment the places well enough to understand
the places. This sile is also responsible for the quiet restrictive compiler
options.

Adding new (and remove the task1 example) needs a simple change in the
[`libs/CMakeLists.txt`](libs/CMakeLists.txt). Just add a line like
`#add_subdirectory("lib/<YourFunct>")`to that file pointing to the folder.

Additional a `libs/lib/<YourFunct>/CMakeLists.txt` needs to be created along
your c-files. This file, unfortunately is a bit complicated on the first view.
But take a look on the existing libs (e.g. the one in wlan). The important ones
are:

* `set(CURR_LIB WirelessLAN)` is a unique name of the library.
* `add_library(${CURR_LIB} ...)` lists all c-files in the folder.
* `target_link_libraries(${CURR_LIB} PUBLIC ...)` lists all SDK libraries

The other entries come in place in case of specific changes required.

## File structure

The idea is that each library block has three main functions.

1. Pre-RTOS used to initialize the HW prior the FreeRTOS starts. This is
   purely single-threaded (unless you do this manually). Called in
   `src/main.c:eMainPreInit()`
2. RTOS-Init specific prepares all tasks/timers for launch. The FreeRTOS
   scheduler is not started.Called in `src/main.c:eMainRtosInit()`
3. As static functions the task/timers and all other stuff. Functions are
   scheduled by FreeRTOS right after starting the RTOS via
   `vTaskStartScheduler()`.


