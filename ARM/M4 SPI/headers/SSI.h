#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "emp_type.h"
#include "system_buffers.h"

INT16U ssi0_in_16bit(void);
void ssi0_out_16bit(INT16U data);
RBUF_INDEX_TYPE ssi0_data_avaliable(void);
RBUF_INDEX_TYPE ssi0_in_space_left(void);
RBUF_INDEX_TYPE ssi0_out_space_left(void);
void setup_ssi0(void);
void ssi0_isr(void);
void ssi0_in_clear(void);
void ssi0_out_clear(void);

#endif
