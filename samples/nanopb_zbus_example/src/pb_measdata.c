#include <zephyr/sys/printk.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "src/measdata.pb.h"

bool encode_measurement_data(MeasurementData *measurement_data, uint8_t *buffer, size_t buffer_size, size_t *message_length)
{
    bool status = false;

    if(buffer_size < MeasurementData_size)
    {
        printk("Buffer size %d is too small, need %d bytes\n", buffer_size, MeasurementData_size);
    }
    else
    {
        /* Create a stream that will write to our buffer. */
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, buffer_size);

        /* Encode the measurement_data */
        status = pb_encode(&stream, MeasurementData_fields, measurement_data);
        if (status)
        {
            *message_length = stream.bytes_written;
        }
    }

    return status;
}

bool decode_measurement_data(MeasurementData *measurement_data, uint8_t *buffer, size_t message_length)
{
    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

    /* Decode the measurement_data. */
    return pb_decode(&stream, MeasurementData_fields, measurement_data);
}

void print_measurement_data(MeasurementData *measurement_data)
{
    printk("  id = %d\n", measurement_data->id);
    printk("  timestamp_us = %d\n", measurement_data->timestamp_us);
    printk("  distances_mm =");
    for(int i = 0; i<4; i++)
    {
        printk(" %d", measurement_data->distances_mm[i]);
    }
    printk("\n");
    printk("  azimuths_mrad =");
    for(int i = 0; i<4; i++)
    {
        printk(" %d", measurement_data->azimuths_mrad[i]);
    }
    printk("\n");
}
