
#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/sys/printk.h>


/* Struct definitions */
typedef struct _MeasurementData {
    /* Unique identifier of the measurement. */
    uint32_t id;
    /* Timestamp of the measurement. */
    uint32_t timestamp_us;
    /* Distance in mm. */
    uint32_t distance_mm;
    /* Azimuth in mrad. */
    uint32_t azimuth_mrad;
} MeasurementData;

ZBUS_CHAN_DEFINE(measurement_zbus_chan,          /* Name */
        MeasurementData,                         /* Message type */
        NULL,                                    /* Validator */
        NULL,                                    /* User data */
        ZBUS_OBSERVERS(zbus_message_subscriber), /* observers */
        ZBUS_MSG_INIT(0)                         /* Initial value {0} */
);

ZBUS_MSG_SUBSCRIBER_DEFINE(zbus_message_subscriber);

static uint32_t measurement_counter = 0;

void take_measurement(struct k_work *work)
{
    measurement_counter++;
    MeasurementData measurementDataIn;
    measurementDataIn.id = measurement_counter;
    measurementDataIn.timestamp_us = 345678;
    measurementDataIn.distance_mm = 123;
    measurementDataIn.azimuth_mrad = 321;

    zbus_chan_pub(&measurement_zbus_chan, &measurementDataIn, K_SECONDS(1));
}

K_WORK_DEFINE(my_work_item, take_measurement);

void my_timer_handler(struct k_timer *dummy)
{
    k_work_submit(&my_work_item);
}

K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);

static void message_subscriber_task(void)
{
	const struct zbus_channel *chan;
    MeasurementData measurementDataOut;

	while (!zbus_sub_wait_msg(&zbus_message_subscriber, &chan, &measurementDataOut, K_FOREVER)) {

        if (&measurement_zbus_chan != chan) {
            printk("Wrong channel %p!", chan);
            continue;
        }
        printk("\n  message_subscriber_task\n");
        printk("  id = %d\n", measurementDataOut.id);
        printk("  timestamp_us = %d\n", measurementDataOut.timestamp_us);
        printk("  distance_mm = %d\n", measurementDataOut.distance_mm);
        printk("  azimuth_mrad = %d\n", measurementDataOut.azimuth_mrad);
    }
}

K_THREAD_DEFINE(task_id, CONFIG_MAIN_STACK_SIZE, message_subscriber_task, NULL, NULL, NULL, 3, 0, 0);

int main(void)
{
    /* start a periodic timer that expires once every second */
    k_timer_start(&my_timer, K_SECONDS(1), K_SECONDS(1));

    return 0;
}
