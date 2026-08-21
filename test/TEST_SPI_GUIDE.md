# SPI Communication Test for Modulo_IO (Slave)

## Overview

`test_spi_communication.c` contains a comprehensive test suite for validating the SPI slave communication protocol used in the Modulo_IO module. It verifies:

- ✓ TX buffer preload with current state (digital outputs + ADC readings)
- ✓ RX command parsing (0x01 for read, 0x02 for write)
- ✓ Response format consistency
- ✓ Data field encoding/decoding
- ✓ CS timing sequence (documentation with expected behavior)

## Quick Start

### Option 1: Run embedded in firmware (Recommended)

1. **Add to main.c** (in the `main()` function, after initialization):
```c
#include "../test/test_spi_communication.c"

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_ADC2_Init();
    MX_SPI1_Init();
    MX_TIM1_Init();
    
    /* Start test suite */
    #ifdef DEBUG_SPI_TEST
        spi_test_suite();
    #endif
    
    /* Remainder of main loop ... */
}
```

2. **Enable test flag in platformio.ini**:
```ini
[env:debug]
build_flags = 
    -DDEBUG_SPI_TEST
```

3. **Compile and upload**:
```bash
platformio run --environment debug
```

4. **View output** via serial monitor (9600 baud or configured rate)

### Option 2: Unit testing via PlatformIO

Create a test runner in `test/test_spi_unit.cpp`:
```cpp
#include <unity.h>
#include "test_spi_communication.c"

void test_spi_preload() {
    test_tx_buffer_preload();
}

void setup() {
    // Setup test environment
}

void loop() {}
```

Run with: `platformio test -e native`

## Test Cases

### 1. **TX Buffer Preload** (`test_tx_buffer_preload`)
- Sets arbitrary digital outputs and ADC values
- Simulates preload logic from `main()` init
- Verifies TX buffer format:
  - Byte 0: Digital outputs (8 bits)
  - Byte 1: ADC1 value
  - Byte 2: ADC2 value
  - Byte 3: End marker (0xFF)

### 2. **RX Write Command (0x02)** (`test_rx_command_write`)
- Simulates master sending write command
- Verifies parsing of digital outputs bitmap
- Verifies parsing of analog output values
- Checks that state is updated correctly

### 3. **RX Read Command (0x01)** (`test_rx_command_read`)
- Simulates master sending read command
- Verifies command byte recognition
- Checks reserved bytes

### 4. **TX Response Format** (`test_tx_response_read`)
- Verifies response packet structure
- Checks all fields are correctly encoded

### 5. **CS Timing Validation** (`test_cs_timing_validation`)
- **Manual verification required** — compares expected master/slave behavior
- Use oscilloscope or logic analyzer to verify:
  - CS goes LOW before first SCK pulse
  - CS goes HIGH after last SCK pulse
  - No glitches or timing violations

### 6. **Full Cycle Simulation** (`test_full_cycle`)
- End-to-end test of a complete master ↔ slave transaction
- Verifies TX preload → RX parse → callback → TX update cycle

## Expected Output

```
======================================
  SPI Slave Communication Test Suite
  Modulo_IO (Esclavo SPI)
======================================

--- TEST: TX Buffer Preload ---
[PASS] TxBuffer[0] = 0x85 (bits 0,2,7 set)
[PASS] TxBuffer[1] = 123 (ADC1 value)
[PASS] TxBuffer[2] = 45 (ADC2 value)
[PASS] TxBuffer[3] = 0xFF (marker)

--- TEST: RX Command Write (0x02) ---
[PASS] modOd[0] = 1 (bit 0 of 0xA5)
[PASS] modOd[1] = 0 (bit 1 of 0xA5)
[PASS] modOd[2] = 1 (bit 2 of 0xA5)
[PASS] modOd[5] = 1 (bit 5 of 0xA5)
[PASS] modOa[0] = 100
[PASS] modOa[1] = 200

... (more tests) ...

======================================
  Test Results Summary
======================================
PASSED: 20
FAILED: 0
TOTAL:  20
======================================

✓ All tests PASSED! SPI protocol is correct.
Now verify with hardware:
  1. Connect master and slave via SPI
  2. Check CS timing with oscilloscope
  3. Verify data exchange with logic analyzer

```

## Debugging Tips

### If tests fail:

1. **Check bit order**: Verify `FIRSTBIT_MSB` is set consistently
2. **Check byte order**: Ensure master and slave agree on packet format
3. **Check ADC readings**: Simulate different ADC values in test
4. **Check GPIO mappings**: Verify digital output indices match hardware

### Hardware verification (after unit tests pass):

```bash
# Check SPI speed
# - Master: BaudRatePrescaler_8 (9MHz at 72MHz clock)
# - Expected: ~9 MHz SCK frequency

# Check signal integrity
# - Use oscilloscope to capture at least 3 full transactions
# - Verify no ringing, noise, or timing violations
# - Confirm CS assertion/deassertion timing

# Check data corruption
# - Add UART logging to main loop
# - Print RxBuffer and TxBuffer after each transfer
# - Compare with expected values
```

## Integration Checklist

- [ ] Test compiles without errors
- [ ] Test runs and shows PASSED results
- [ ] Hardware connected (SPI + CS)
- [ ] Oscilloscope/logic analyzer confirms CS timing
- [ ] Master and slave exchange valid data
- [ ] No timeout or error callbacks triggered
- [ ] Both TX and RX buffers updated correctly

## Next Steps

1. Run this test suite to verify protocol correctness
2. Apply the 4 CS timing changes to the master (`induSPI.c`)
3. Connect hardware and observe SPI transactions
4. If data is corrupted, add more detailed logging in callbacks
5. Validate with oscilloscope that CS, SCK, MOSI, MISO timing is correct
