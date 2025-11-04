
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

ZBUS_CHAN_DEFINE(measurement_zbus_chan,       /* Name */
        MeasurementData,                      /* Message type */
        NULL,                                 /* Validator */
        NULL,                                 /* User data */
        ZBUS_OBSERVERS(zbus_listener),        /* observers */
        ZBUS_MSG_INIT(0)                      /* Initial value {0} */
);

static void zbus_listener_callback(const struct zbus_channel *chan)
{
    const MeasurementData *p_measurementDataOut = zbus_chan_const_msg(chan);
    printk("\n  zbus_listener_callback\n");
    printk("  id = %d\n", p_measurementDataOut->id);
    printk("  timestamp_us = %d\n", p_measurementDataOut->timestamp_us);
    printk("  distance_mm = %d\n", p_measurementDataOut->distance_mm);
    printk("  azimuth_mrad = %d\n", p_measurementDataOut->azimuth_mrad);
}

ZBUS_LISTENER_DEFINE(zbus_listener, zbus_listener_callback);

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

int main(void)
{
    /* start a periodic timer that expires once every second */
    k_timer_start(&my_timer, K_SECONDS(1), K_SECONDS(1));

    return 0;
}
