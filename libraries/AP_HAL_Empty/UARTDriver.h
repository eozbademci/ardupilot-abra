#pragma once

#include "AP_HAL_Empty.h"

class Empty::UARTDriver : public AP_HAL::UARTDriver {
public:
    UARTDriver();
    /* Empty implementations of UARTDriver virtual methods */
    void begin(uint32_t b) override;
    void begin(uint32_t b, uint16_t rxS, uint16_t txS) override;
    void end() override;
    void flush() override;
    bool is_initialized() override;
    void set_blocking_writes(bool blocking) override;
    bool tx_pending() override;

    /* Empty implementations of Stream virtual methods */
    uint32_t available() override;
    uint32_t txspace() override;
    bool read(uint8_t &b) override WARN_IF_UNUSED;
    bool discard_input() override;

    /* Empty implementations of Print virtual methods */
    size_t write(uint8_t c) override;
    size_t write(const uint8_t *buffer, size_t size) override;

#if HAL_UART_STATS_ENABLED
    // request information on uart I/O for one uart
    void uart_info(ExpandingString &str) override;
#endif
};
