
#ifndef PB_MEASDATA_H_INCLUDED
#define PB_MEASDATA_H_INCLUDED

#include "src/measdata.pb.h"

bool encode_measurement_data(MeasurementData *message, uint8_t *buffer, size_t buffer_size, size_t *message_length);
bool decode_measurement_data(MeasurementData *message, uint8_t *buffer, size_t message_length);
void print_measurement_data(MeasurementData *message);

#endif /* PB_MEASDATA_H_INCLUDED */
