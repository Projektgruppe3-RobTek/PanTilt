#pragma once

#include <stdint.h>
#include "../../headers/emp_type.h"
#include "../os/system_buffers.h"
#include "SSI.h"

INT16U ssi3_in_16bit(void);
void ssi3_out_16bit(INT16U data);
RBUF_INDEX_TYPE ssi3_data_available(void);
RBUF_INDEX_TYPE ssi3_in_space_left(void);
RBUF_INDEX_TYPE ssi3_out_space_left(void);
void setup_ssi3(void);
void ssi3_isr(void);
void ssi3_in_clear(void);
void ssi3_out_clear(void);
