
#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/sys/printk.h>
#include <math.h>

#include "pb_measdata.h"

struct MeasurementZbusMsg {
    uint8_t buf[MeasurementData_size];
    size_t len;
};

ZBUS_CHAN_DEFINE(measurement_zbus_chan,       /* Name */
        struct MeasurementZbusMsg,            /* Message type */
        NULL,                                 /* Validator */
        NULL,                                 /* User data */
        ZBUS_OBSERVERS(measurement_zbus_sub), /* observers */
        ZBUS_MSG_INIT(0)                      /* Initial value {0} */
);

ZBUS_SUBSCRIBER_DEFINE(measurement_zbus_sub, 4);

static uint32_t measurement_counter = 0;

void take_measurements(struct k_work *work)
{
    measurement_counter++;
    MeasurementData measurementDataIn = MeasurementData_init_zero;
    measurementDataIn.id = measurement_counter;
    measurementDataIn.timestamp_us = 345678;
    measurementDataIn.distances_mm[0] = 1012;
    measurementDataIn.distances_mm[1] = 1203;
    measurementDataIn.distances_mm[2] = 972;
    measurementDataIn.distances_mm[3] = 1402;
    measurementDataIn.azimuths_mrad[0] = 1012;
    measurementDataIn.azimuths_mrad[1] = 1203;
    measurementDataIn.azimuths_mrad[2] = 972;
    measurementDataIn.azimuths_mrad[3] = 1402;

    struct MeasurementZbusMsg msg;
    if( !encode_measurement_data(&measurementDataIn, msg.buf, MeasurementData_size, &msg.len) )
    {
        printk("encode_measurement_data failed\n");
        return;
    }
    printk("take_measurements : encoded msg.len = %d\n", msg.len);
    //printk("take_measurements : encoded msg.buf... 0x");
    //for(size_t i = 0 ; i< msg.len ; i++)
    //{
    //    printk(" %02x", msg.buf[i]);
    //}
    //printk("\n");

    zbus_chan_pub(&measurement_zbus_chan, &msg, K_SECONDS(1));
}

K_WORK_DEFINE(my_work_item, take_measurements);

void my_timer_handler(struct k_timer *dummy)
{
    k_work_submit(&my_work_item);
}

K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);

static void subscriber_task(void)
{
	const struct zbus_channel *chan;
    struct MeasurementZbusMsg msg;
    MeasurementData measurementDataOut = MeasurementData_init_zero;

	while (!zbus_sub_wait(&measurement_zbus_sub, &chan, K_FOREVER)) {

		if (&measurement_zbus_chan == chan) {
			zbus_chan_read(&measurement_zbus_chan, &msg, K_MSEC(500));
            printk("subscriber_task : encoded msg.len = %d\n", msg.len);
            //printk("subscriber_task : encoded msg.buf... 0x");
            //for(size_t i = 0 ; i< msg.len ; i++)
            //{
            //    printk(" %02x", msg.buf[i]);
            //}
            //printk("\n");

            if( !decode_measurement_data(&measurementDataOut, msg.buf, msg.len) )
            {
                printk("decode_measurement_data failed\n");
                continue;
            }
            print_measurement_data(&measurementDataOut);
		}
	}
}

K_THREAD_DEFINE(subscriber_task_id, CONFIG_MAIN_STACK_SIZE, subscriber_task, NULL, NULL, NULL, 3, 0, 0);

int main(void)
{
    /* start a periodic timer that expires once every second */
    k_timer_start(&my_timer, K_SECONDS(1), K_SECONDS(1));

    return 0;
}
