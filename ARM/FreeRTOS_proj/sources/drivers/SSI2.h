#pragma once

#include <stdint.h>
#include "../../headers/emp_type.h"
#include "../os/system_buffers.h"
#include "SSI.h"

INT16U ssi2_in_16bit(void);
void ssi2_out_16bit(INT16U data);
RBUF_INDEX_TYPE ssi2_data_available(void);
RBUF_INDEX_TYPE ssi2_in_space_left(void);
RBUF_INDEX_TYPE ssi2_out_space_left(void);
void setup_ssi2(void);
void ssi2_isr(void);
void ssi2_in_clear(void);
void ssi2_out_clear(void);
