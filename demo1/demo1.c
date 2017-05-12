#include <16F1826.h>

#FUSES INTRC_IO, NOWDT, PUT, NOMCLR, NOPROTECT, NOCPD, BROWNOUT, NOCLKOUT, NOIESO, NOFCMEN, NOWRT, PLL_SW, NOSTVREN, BORV25, NODEBUG, NOLVP

#device adc=10
#use delay(clock=32MHZ)    //clock de 32Mhz
#use standard_io(a)
#use standard_io(b)

#include "../battery_types.h"

#define BAT_ADC_CH					V_BAT_ADC_CH
#define BAT_TYPE					BAT_23A
#define BAT_VREF_INTERNO			VREF_ADC_4v096
#define BAT_R1						100000
#define BAT_R2						39000

#define TRIS_A	0b11111111
#define TRIS_B	0b01111111

#define V_IN_PIN	PIN_A4
#define SERIAL_TX	PIN_B7

#define V_BAT_ADC_CH	4		//canal 4 del adc para el sensor de bateria

#include "../battery_level.c"

#use rs232(baud=19200, xmit=SERIAL_TX)

void main(void){
	setup_oscillator(OSC_8MHZ|OSC_PLL_ON);	//configura oscilador interno

	setup_wdt(WDT_OFF);						//configuracion wdt
	setup_timer_0(T0_INTERNAL|T0_DIV_1);	//configuracion timer0
	setup_timer_1(T1_DISABLED);				//configura timer1
	setup_timer_2(T2_DISABLED,0,1);			//configuracion timer2
	setup_dac(DAC_OFF);						//configura DAC
	setup_adc(ADC_OFF);						//configura ADC
	setup_ccp1(CCP_OFF);					//configura CCP
	setup_spi(FALSE);						//configura SPI
	setup_comparator(NC_NC_NC_NC);			//comparador apagado
	setup_vref(FALSE);						//no se usa voltaje de referencia
	
	set_tris_a(TRIS_A);
	set_tris_b(TRIS_B);
	
	do{
		//valor leido del ADC
		printf("ADC leido: %lu\n\r", LeerADC());
		//valor ADC convertido en voltaje (0 - Vref)
		printf("Voltaje en pin ADC: %fv\n\r", ADCenVoltaje(VOLT_0_VREF));
		//valor ADC convertido en voltaje de bateria (0 - V_BAT)
		printf("Voltaje de la bateria: %fv\n\r\n\r", ADCenVoltaje(VOLT_0_VEXT));
		delay_ms(1000);
	}while(TRUE);
}