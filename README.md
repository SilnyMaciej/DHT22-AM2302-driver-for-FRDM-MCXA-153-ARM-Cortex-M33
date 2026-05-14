# DHT22 (AM2302) Driver for FRDM-MCXA153

A lightweight C driver for the **DHT22 / AM2302** temperature and humidity sensor. This repository is specifically tailored for the **FRDM-MCXA153** development board, powered by the **ARM Cortex-M33** MCXA153 microcontroller within the NXP MCUXpresso SDK environment.

The driver implements precise 1-wire communication via custom bit-banging routines and accurate microsecond delays to fulfill the strict timing requirements of the DHT22 sensor.

## Repository Structure

* **`dht22_driver.h`** — Function declarations, error codes (`GND_ERROR`, `VCC_ERROR`, `CHECK_SUM_ERROR`), and timing definitions.
* **`dht22_driver.c`** — Core logic for managing the 1-wire protocol, sensor initialization, and reading the 40-bit data packet.
* **`delay_us.h` / `delay_us.c`** — Microsecond delay utility optimized for the Cortex-M33 clock architecture.

## Key Features

* Packaged 32-bit single-function read macro (`DHT22_Get_Temperature_And_RH()`).
* Comprehensive error handling for physical layer faults (GND shorts, VCC line timeouts).
* Integral 8-bit checksum verification processed entirely inside the driver layer.
* Built-in support for sub-zero temperatures via sign-bit extraction (`0x8000`).

## Hardware Setup

1. Connect the DHT22 **VCC** pin to the **3.3V** power rail on the FRDM-MCXA153.
2. Connect the DHT22 **GND** pin to the **GND** rail.
3. Connect the DHT22 **DATA** pin to your selected GPIO pin on the board.
4. Add a **4.7kΩ to 10kΩ pull-up resistor** between the DATA line and VCC (critical for the 1-wire open-drain architecture).

## Integration & Usage Guide

### 1. Peripheral & Pin Configuration
Ensure you configure your target GPIO pin using the NXP MCUXpresso **Pins Tool** (`pin_mux.c`) and initialize your system clocks (`clock_config.c`). The driver handles internal data direction switching automatically during the read routine.

### 2. Application Implementation Example

Replace your application entry point (e.g., `main.c` / `DHT22_sample.c`) with the following implementation:

```c

        // Read raw data package from the sensor
        uint32_t raw_data = DHT22_Get_Temperature_And_RH();

        switch(raw_data) {
            case GND_ERROR:
                PRINTF("ERROR: Sensor not responding (GND_ERROR)!\r\n");
                break;

            case VCC_ERROR:
                PRINTF("ERROR: Data line timeout (VCC_ERROR)!\r\n");
                break;

            case CHECK_SUM_ERROR:
                PRINTF("ERROR: Incorrect checksum (CHECK_SUM_ERROR)!\r\n");
                break;

            default:
                // Parse 16-bit Relative Humidity data
                uint16_t rh_combined = (((raw_data >> 24) & 0xFF) << 8) | ((raw_data >> 16) & 0xFF);

                // Parse 16-bit Temperature data
                uint16_t t_combined  = (((raw_data >> 8) & 0xFF) << 8) | (raw_data & 0xFF);

                // Display Humidity
                PRINTF("Humidity: %d.%d %%\r\n", rh_combined / 10, rh_combined % 10);

                // Check sign-bit for below 0°C temperatures
                if(t_combined & 0x8000) {
                    uint16_t t_val = t_combined & 0x7FFF;
                    PRINTF("Temp: -%d.%d C\r\n", t_val / 10, t_val % 10);
                } else {
                    PRINTF("Temp: %d.%d C\r\n", t_combined / 10, t_combined % 10);
                }
                break;
        }

        // DHT22 requires a minimum 2-second interval between consecutive reads
        delay_us(2000000U);
```
