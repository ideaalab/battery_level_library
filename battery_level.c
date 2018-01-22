#include "battery_level.h"

/* ----------- FUNCIONES PUBLICAS ----------- */

/*
 * Lee el valor del ADC y devuelve el voltaje
 * Para lectura del FVR tenemos que "desinvertir" el ADC leido e invertido en la
 * funcion LeerADC() para que los calculos funcionen
 * En el caso del FVR, solo puede devolver el valor leido
 * En el caso de la lectura externa con ADC, puede devolver:
 * VOLT_0_VREF: el voltaje leido del adc, despues del divisor de tension
 * VOLT_0_VEXT: el valor leido "real" antes del divisor de tension
 */
#ifdef BATTERY_READ_INTERNAL_H
float ADCenVoltaje(void){
#if getenv("ADC_RESOLUTION") == 8
int ValADC = ~LeerADC();			//leo voltaje ADC
#elif getenv("ADC_RESOLUTION") == 10
long ValADC = ~LeerADC() & 0x3FF;	//leo voltaje ADC
#endif

	float Voltaje;
	
	//conversion de la lectura interna en voltaje Vdd
	Voltaje = (float)ADCMAX_X_FVR / ValADC;
	return(Voltaje);
}
#else
float ADCenVoltaje(short read){
#if getenv("ADC_RESOLUTION") == 8
int ValADC = LeerADC();			//leo voltaje ADC
#elif getenv("ADC_RESOLUTION") == 10
long ValADC = LeerADC();	//leo voltaje ADC
#endif

	float Voltaje;
	
	//calcula el voltaje leido por el ADC (0v - Vref)
	Voltaje = BAT_VOLTxADC * ValADC;
	
	if(read == VOLT_0_VEXT){
		//devuelve el voltaje convertido
		Voltaje = BAT_PROPORCION_IN_OUT * Voltaje;
	}
	
	return(Voltaje);
}
#endif

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
 * Siendo 0 = V_BAT_MIN y 255 = V_BAT_MAX
 * El valor leido puede estar por debajo o por encima de MIN y MAX pero esta
 * funcion no lo tiene en cuenta.
 */
int NivelBateria255(void){
long ValADC = 0;
long Val255 = 0;

	ValADC = LeerADC();	//leo voltaje ADC
	
	if(ValADC < ADC_BAT_MIN){
		Val255 = 0;
	}
	else if(ValADC >= ADC_BAT_MAX){
		Val255 = 255;
	}
	else{
		ValADC = ValADC - ADC_BAT_MIN;
		Val255 = ValADC * (255.0 / RANGO_ADC_BAT);
		
		if(Val255 > 255) Val255 = 255;
	}
	
	return(Val255);
}

/*
 * Devuelve un valor de bateria entre 0 y Max
 * 0 significa que la bateria esta por debajo del valor minimo
 * 1 a Max es el estado de la bateria entre V_BAT_MIN y V_BAT_MAX
 * Max es que la bateria esta en su maximo valor o por encima
 */
int NivelBateria0_X(int Max){
long ValADC = 0;
long Val = 0;
	
	ValADC = LeerADC();	//leo voltaje ADC
	
	if(ValADC < ADC_BAT_MIN){
		Val = 0;
	}
	else if(ValADC >= ADC_BAT_MAX){
		Val = Max;
	}
	else{
		ValADC = ValADC - ADC_BAT_MIN;
		Val = 1 + (ValADC * Max / RANGO_ADC_BAT);
		
		if(Val > Max) Val = Max;
	}
	
	return(Val);
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
#if getenv("SFR_VALID:ADCON") == TRUE
	Save_ADCON = ADCON;
#endif
	
#if getenv("SFR_VALID:ADCON0") == TRUE
	Save_ADCON0 = ADCON0;
#endif
	
#if getenv("SFR_VALID:ADCON1") == TRUE
	Save_ADCON1 = ADCON1;
#endif
}

/*
 * Restaura los valores del ADC previos a la lectura de la bateria
 */
void RestaurarConfigADC(void){
	FVRCON = Save_FVRCON;
	
#if getenv("SFR_VALID:ADCON") == TRUE
	ADCON = Save_ADCON;
#endif
	
#if getenv("SFR_VALID:ADCON0") == TRUE
	ADCON0 = Save_ADCON0;
#endif
	
#if getenv("SFR_VALID:ADCON1") == TRUE
	ADCON1 = Save_ADCON1;
#endif
}

/*
 * Configura el ADC para leer el voltaje de la bateria
 */
void ConfigurarADC(void){
#ifdef BATTERY_READ_EXTERNAL_H
	//configura ADC externo
	#if defined(BAT_VREF_INTERNO)
		setup_vref(BAT_VREF_INTERNO);	//configura VRef
		setup_adc_reference(VSS_FVR);	//0 - Fixed Voltage Reference
	#elif defined(BAT_VREF_EXTERNO)
		setup_vref(VREF_OFF);			//configura VRef
		setup_adc_reference(VSS_VREF);	//0 - VrefH
		#warning "Comprobar si funciona"
	#else	//BAT_VREF_VDD
		setup_vref(VREF_OFF);			//configura VRef
		setup_adc_reference(VSS_VDD);	//0 - Vdd
	#endif
#else
	//activa el voltaje de referencia interno (no es lo mismo que el ADC reference!)
	setup_vref(FVR_DEF);			//configura VRef
	
	//por defecto es siempre asi
	//setup_adc_reference(VSS_VDD);	//0 - Vdd
#endif
	
	setup_adc(ADC_CLOCK_INTERNAL);	//configura ADC
	set_adc_channel(BAT_ADC_CH);	//selecciona canal
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