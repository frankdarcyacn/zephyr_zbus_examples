.. _nanopb_zbus_ex:

nanopb_zbus_ex
##############

A simple example of using nanopb with ZBus on Zephyr.

Overview
********

At compile-time, ``MeasurementData`` defined in ``measdata.proto`` is used by nanopb to generate C code representing that data.

In ``main.c``, a timer is set up to submit a work item to run ``take_measurements()`` once every second.

``take_measurements()`` fills the ``MeasurementData`` struct with data.
The ``id`` field is incremented each time it is called while the rest of the fields are constant made-up numbers.

The ``MeasurementData`` struct is then encoded using nanopb into a byte buffer in the struct::

    struct MeasurementZbusMsg {
        uint8_t buf[MeasurementData_size];
        size_t len;
    };

which is then published to a zbus channel.

A subscriber thread waits for the zbus channel's notification, then reads the ``MeasurementZbusMsg`` struct
and then uses nanopb to decode the buffer to a ``MeasurementData`` struct with data.

This sample should build on any board supported by Zephyr!
