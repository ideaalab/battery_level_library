#include "battery_level.h"

/* ----------- FUNCIONES PUBLICAS ----------- */

/*
 * Lee el valor del ADC y devuelve el voltaje
 * En el caso del FVR, solo puede devolver el valor leido
 * En el caso de la lectura externa con ADC, puede devolver:
 * VOLT_0_VREF: el voltaje leido del adc, despues del divisor de tension
 * VOLT_0_VEXT: el valor leido "real" antes del divisor de tension
 */
#ifdef BATTERY_READ_INTERNAL_H
float ADCenVoltaje(void){
#else
float ADCenVoltaje(short read){
#endif
	
#if getenv("ADC_RESOLUTION") == 8
int ValADC = ~LeerADC();			//leo voltaje ADC
#elif getenv("ADC_RESOLUTION") == 10
long ValADC = ~LeerADC() & 0x3FF;	//leo voltaje ADC
#endif

float Voltaje;

//conversion de la lectura interna en voltaje Vdd
#ifdef BATTERY_READ_INTERNAL_H
	Voltaje = (float)ADCMAX_X_FVR / ValADC;
//conversion de la lectura externa en voltaje
#else
	//devuelve el valor voltaje leido por el ADC (0v - Vref)
	if(read == VOLT_0_VREF){
		Voltaje = (float)ValADC * BAT_PIC_VREF / ADC_MAX_VAL;
	}
	//devuelve el voltaje convertido
	else{
		Voltaje = (float)ValADC * BAT_PIC_VREF * BAT_PROPORCION_IN_OUT / ADC_MAX_VAL;
	}
	
#endif
	
	return Voltaje;
}

/*
 * Devuelve un valor de bateria con etiquetas BAT_CRITICA, BAT_BAJA, BAT_MEDIA y BAT_ALTA
 */
int NivelBateria0_3(void){

#if getenv("ADC_RESOLUTION") == 8
int ValADC = 0;
#elif getenv("ADC_RESOLUTION") == 10
long ValADC = 0;
#endif

	ValADC = LeerADC();	//leo voltaje ADC
	
	//bateria critica
	if(ValADC < ADC_BAT_BAJA){
		return BAT_CRITICA;
	}
	//bateria baja
	if(ValADC < ADC_BAT_MEDIA){
		return BAT_BAJA;
	}
	//bateria media
	if(ValADC < ADC_BAT_ALTA){
		return BAT_MEDIA;
	}
	//bateria alta
	else{
		return BAT_ALTA;
	}
}

/*
 * Devuelve un valor de bateria entre 0 y 255
 */
int NivelBateria255(void){
signed long ValADC = 0;
signed long Val255 = 0;
	
	ValADC = LeerADC();	//leo voltaje ADC
	ValADC = ValADC - ADC_BAT_MIN;
	Val255 = ValADC * 255 / RANGO_ADC_BAT ;
	
	/*printf("RANGO: %Lu\r\n", RANGO_ADC_BAT);
	printf("ADC255: %Ld\r\n\r\n", Val255);
	printf("ADC-MIN: %Ld\r\n", ValADC);
	printf("ADC: %Lu\r\n", ValADC);
	printf("MIN: %Lu\r\n", ADC_BAT_MIN);*/
	
	if(Val255 < 0) Val255 = 0;
	if(Val255 > 255) Val255 = 255;
	
	return (int)Val255;
}

/*
 * Devuelve un valor de bateria entre 0 y Max
 */
int NivelBateria0_X(int Max){
signed long ValADC = 0;
signed long Val = 0;
	
	ValADC = LeerADC();	//leo voltaje ADC
	ValADC = ValADC - ADC_BAT_MIN;
	Val = ValADC * Max / RANGO_ADC_BAT;
	
	if(Val < 0) Val = 0;
	if(Val > Max) Val = Max;
	
	return (int)Val;
}

/* ----------- FUNCIONES PRIVADAS ----------- */
/*
 * Guarda los valores de configuracion del ADC
 * Al leer el la bateria hay que cambiar la configuracion del ADC
 * Esta funcion guarda los valores originales para luego poder restaurarlos
 * al finalizar la lectura
 */
void GuardarConfigADC(void){
	//guarda la config de FVR
	Save_FVRCON = FVRCON;
	//guarda la config general del ADC
	Save_ADCON0 = ADCON0;
	Save_ADCON1 = ADCON1;
}

/*
 * Restaura los valores del ADC previos a la lectura de la bateria
 */
void RestaurarConfigADC(void){
	FVRCON = Save_FVRCON;
	ADCON0 = Save_ADCON0;
	ADCON1 = Save_ADCON1;
}

/*
 * Configura el ADC para leer el voltaje de la bateria
 */
void ConfigurarADC(void){
#ifdef BATTERY_READ_EXTERNAL_H
	//configura ADC externo
	setup_vref(VREF_OFF);			//configura VRef
	setup_adc(ADC_CLOCK_INTERNAL);	//configura ADC
	set_adc_channel(BAT_ADC_CH);	//selecciona canal
#else
	//configura ADC interno
	setup_vref(FVR_DEF);			//configura VRef
	setup_adc(ADC_CLOCK_INTERNAL);	//configura ADC
	set_adc_channel(FVR_CHANNEL);	//selecciona canal
#endif
	
	delay_us(10);	//estabilizo el voltaje del canal
}

/*
 * Funcion que lee el ADC
 * Antes de leer guarda config ADC anterior
 * y luego de leer restaura valores previos
 */
#if getenv("ADC_RESOLUTION") == 8
	int LeerADC(void){
		int tmp;
		
		GuardarConfigADC();
		ConfigurarADC();
		
	#ifdef BATTERY_READ_EXTERNAL_H
		tmp = read_adc();	//leo voltaje ADC
	#else
		tmp = ~read_adc();	//leo voltaje FVR
	#endif
		
		RestaurarConfigADC();
		return tmp;
	}
#elif getenv("ADC_RESOLUTION") == 10
	long LeerADC(void){
		long tmp;
		
		GuardarConfigADC();
		ConfigurarADC();
		
	#ifdef BATTERY_READ_EXTERNAL_H
		tmp = read_adc();	//leo voltaje ADC
	#else
		tmp = ~read_adc() & 0x3FF;	//leo voltaje FVR
	#endif

		RestaurarConfigADC();
		return tmp;
	}
#endif