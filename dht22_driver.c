#include "pin_mux.h"
#include "dht22_driver.h"


uint32_t DHT22_Get_Temperature_And_RH(void) {

	volatile uint32_t timeout = DHT22_TIMEOUT_CYCLES;
    uint8_t data[5] = {0};

    DHT22_GPIO->PCOR = DHT22_PIN_MASK;
    DHT22_GPIO->PDDR |= DHT22_PIN_MASK;


    delay_us(2000U);
    DHT22_GPIO->PDDR &= ~DHT22_PIN_MASK;

    delay_us(42U);

    while(!(DHT22_GPIO->PDIR & DHT22_PIN_MASK)) {
    	 if(--timeout == 0) return GND_ERROR;
    	 __asm("nop");
    }


    timeout = DHT22_TIMEOUT_CYCLES;

    while(DHT22_GPIO->PDIR & DHT22_PIN_MASK) {
    	if(--timeout == 0) return VCC_ERROR;
    	__asm("nop");

    }

    uint8_t bit = 0;

    for(uint8_t i = 0; i < 40; i++) {

    	timeout = DHT22_TIMEOUT_CYCLES;

    	while(!(DHT22_GPIO->PDIR & DHT22_PIN_MASK)) {
    		if(--timeout == 0) return GND_ERROR;
    		__asm("nop");
    	 }

        delay_us(30U);

        bit = (DHT22_GPIO->PDIR & DHT22_PIN_MASK) != 0;
        *(data + (i >> 3)) |= (bit << (7 - (i & 7)));

        timeout = DHT22_TIMEOUT_CYCLES;

        while(DHT22_GPIO->PDIR & DHT22_PIN_MASK) {
        	if(--timeout == 0) return VCC_ERROR;
        	__asm("nop");
        }


    }



   if((uint8_t)(*data + *(data + 1) + *(data + 2) + *(data + 3)) == *(data + 4)) {

	  return ((uint32_t)(*(data)) << 24) | ((uint32_t)(*(data + 1)) << 16) | ((uint32_t)(*(data + 2)) << 8) | ((uint32_t)(*(data + 3)));
   }
   return CHECK_SUM_ERROR;

}
