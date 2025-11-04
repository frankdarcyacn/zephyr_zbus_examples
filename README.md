# zephyr_zbus_examples

This repo contains some exploratory work with ZBus in Zephyr.

## Prerequisites

Set up the dependencies and tools needed to build Zephyr by following its
[Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html).

When this is complete, you should have a python environment in which [Zephyr's `west` tool](https://docs.zephyrproject.org/latest/develop/west/index.html) is available.

`west` is a multipurpose tool used extensively in development and test of applications using Zephyr, as outlined below.

## Workspace Setup

Create a top directory (eg. named `zephyr_workspace`) and clone this repo in it.

```
mkdir zephyr_workspace
cd zephyr_workspace
git clone <url for this repo> zephyr_zbus_examples
```

Initialize the workspace (this just creates `.west/config`)...

```
west init -l zephyr_zbus_examples
```

Update the workspace - the following command pulls code from multiple git repos
and lays it out in the workspace as specified in `zephyr_zbus_examples/west.yml`...

```
west update
```

## Build and run on your host

To build one of the example apps to run on your host system...

```
west build -p -b native_sim zephyr_zbus_examples/samples/zbus_msg_sub_example/
```

Assuming this is successful, all generated build output should be
in the newly created `build` directory,
and you can directly run the executable as follow...
```
build/zephyr/zephyr.exe
```

## Build for an ARM-based Nordic development board

`west.yml` pulls in the Zephyr RTOS git repo which in turn refers to
many modules (in other git repos) in its `west.yml` file.
In order to figure out exactly which modules are needed (or not) 
and to minimize the amount of code that needs to be downloaded (by cloning git repos)
`west.yml` in this repo uses `name-allowlist:`
to restrict the module list imported from Zephyr to only those needed.

Having the following Zephyr modules listed under `name-allowlist:` in `west.yml`...
```
          - cmsis_6
          - hal_nordic
          - nanopb
```
means that their git repos should have been cloned under the directory `modules/hal`
when you ran `west update` and this means you should be able to
build for ARM-based Nordic development boards,
assuming other dependencies they may need are in place.
One of the samples uses `nanopb` hence that is needed too.

Let's use the [nRF5340 Development Kit](https://www.nordicsemi.com/Software-and-tools/Development-Kits/nRF5340-DK) as an example.

The Zephyr page for the
[nRF5340](https://docs.zephyrproject.org/latest/boards/nordic/nrf5340dk/doc/index.html)
contains information related to using Zephyr with this board.

To build for the nRF5340 SoC's application core (`cpuapp`)...
```
west build -p -b nrf5340dk/nrf5340/cpuapp zephyr_zbus_examples/samples/zbus_msg_sub_example/
```
