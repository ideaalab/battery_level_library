/*
 * En el ejemplo se utiliza un PIC 12F1840 alimentado con una bateria LiPo de 1 celda (3.7v)
 * Usando el pin A0 enviamos los valores leidos (de manera interna) de la LiPo y los mostramos por puerto serie
 * No hace falta ninguna conexion externa para poder leer el voltaje de la bateria
 */


#include <12F1840.h>
#device adc=10
#use delay(clock=32000000)    //clock de 32Mhz

#FUSES INTRC_IO,NOWDT,PUT,NOMCLR,NOPROTECT,NOCPD,BROWNOUT_NOSL,NOCLKOUT,NOIESO,NOFCMEN,NOWRT,PLL_SW,NOSTVREN,BORV25,NODEBUG,NOLVP

/* HEADER */
#include "../battery_types.h"

//						543210
#define TRIS_A		0b00111110		//define cuales son entradas y cuales salidas

/* CONSTANTES PARA BATERIA */
#define BAT_TYPE				BAT_1LIPO

/* INCLUDES */
#include "../battery_level.c"

#use rs232(baud=19200, xmit=PIN_A0, FORCE_SW)	//config comunicacion serial

void main(void) {

	setup_oscillator(OSC_8MHZ|OSC_PLL_ON);	//configura oscilador interno
	setup_wdt(WDT_OFF);						//configuracion wdt
	setup_timer_0(T0_INTERNAL|T0_DIV_256);	//configuracion timer0
	setup_timer_1(T1_DISABLED);				//configuracion timer1
	setup_timer_2(T2_DISABLED,255,1);		//configuracion timer2
	setup_dac(DAC_OFF);						//configura DAC
	setup_adc(ADC_OFF);						//configura ADC
	setup_ccp1(CCP_OFF);					//configura CCP
	setup_spi(SPI_DISABLED);				//configura SPI
	//setup_uart(FALSE);					//configura UART
	setup_comparator(NC_NC);				//comparador apagado
	setup_vref(VREF_OFF);					//no se usa voltaje de referencia
	set_tris_a(TRIS_A);						//configura I/O
	
	do{
		printf("Nominal: %1.2f / Minimo: %1.2f / Maximo: %1.2f\r\n", V_BAT_NOM, V_BAT_MIN, V_BAT_MAX);
		printf("Voltaje Bateria: %1.2f\r\n", ADCenVoltaje());
		
		printf("Estado: ");
		
		switch(NivelBateria0_3()){
			case BAT_CRITICA:
				printf("CRITICO\r\n");
				break;
				
			case BAT_BAJA:
				printf("BAJO\r\n");
				break;
				
			case BAT_MEDIA:
				printf("MEDIO\r\n");
				break;
				
			case BAT_ALTA:
				printf("ALTO\r\n");
				break;
		}
		
		printf("Nivel de bateria 0-255: %u\r\n", NivelBateria255());
		
		printf("0-4: %u\r\n", NivelBateria0_X(4));
		
		printf("\r\n");
	
		delay_ms(1000);
	}while(true);
}