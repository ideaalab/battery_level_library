/* 
 * File:   battery_internal_fvr.h
 * Author: Martin
 *
 * Created on 11 de enero de 2017, 13:23
 */

#ifndef BATTERY_READ_INTERNAL_H
#define	BATTERY_READ_INTERNAL_H
/*
 * ------------------------------ CALCULOS ------------------------------------
 * CALCULOS USANDO REFERENCIA DE VOLTAJE INTERNA (Vbat < 5v):
 * -Se usa el voltaje de referencia interno, que siempre es fijo independiente_
 * mente del valor de VDD/Vbat.
 * -El ADC puede leer este valor, que sera mayor cuanto menor sea Vbat, y menor
 * cuanto mayor sea Vbat (disminuye cuando Vbat es mayor).
 * 
 * ADC = Valor de la lectura del ADC
 * FVR = 2.048v (puede ser otro)
 * ADCMaxVal = Valor maximo del ADC. 8bits = 255 / 10bits = 1023
 * 
 * Calculo para saber Vbat segun el valor del ADC
 *  -  Vbat =   ADCMaxVal * FVR / ADC
 *  - ~Vbat = ~(ADCMaxVal * FVR / ADC)
 * 
 * Calculo para saber el ADC segun el voltaje de la bateria
 *	-  ADC =   1023 * FVR / Vbat
 *	- ~ADC = ~(1023 * FVR / Vbat)
 * 
 * -Para unificar la libreria y los calculos con las lecturas externas de ADC, 
 * donde el valor de ADC aumenta con Vbat, en esta libreria se "invierten" o
 * "complementan" los valores del ADC para que incrementen cuando aumenta Vbat.
 * 
 * Si se complementa a ambos lados del igual, el resultado sigue siendo correcto:
 * a = b -> ~a = ~b
 * 
 * El complemento en decimal es restar el valor de 255 (8bits) o 1023 (10bits):
 * Valor						Complemento
 * 214 (8bits)					->	41 (255 - 214)
 * 214 (10bits)					->	809 (1023 - 214)
 * 
 * El complemento en binario es negar cada bit del valor:
 *  Valor						Complemento
 * (214) 0b11010110				->	(41) 0b00101001
 * (214) 0b00000000 11010110	->	(65321) 0b11111111 00101001	<- !!!
 * 
 * El complemento de un int16 complementa todos sus bits. En el caso del ADC de
 * 10bits solo queremos complementar los 10 primeros bits e ignorar los ultimos 6
 * Para eso usamos una mascara de 10bits 0x3FF (0b00000011 11111111):
 *		0b11111111 00101001 (65321 / 0xFF29)
 *	&	0b00000011 11111111 (1032  / 0x03FF)
 *		-------------------
 *	=	0b00000011 00101001 (809   / 0x0329)
 * 
 */

/* CONFIGURACION FVR */
#ifndef FVR_DEF
//#define FVR_DEF	VREF_ADC_1v024
#define FVR_DEF		VREF_ADC_2v048
//#define FVR_DEF	VREF_ADC_4v096
#endif

#if FVR_DEF == VREF_ADC_1v024
#define V_FVR			(1.024)
#elif FVR_DEF == VREF_ADC_2v048
#define V_FVR			(2.048)
#elif FVR_DEF == VREF_ADC_4v096
#define V_FVR			(4.096)
#else
#ERROR "Error de FVR ¿El PIC no tiene FVR?"
#endif

#define BAT_ADC_CH		FVR_CHANNEL

/* CALCULOS DE BATERIA Y ADC */
#define ADCMAX_X_FVR	(ADC_MAX_VAL * V_FVR)

#if getenv("ADC_RESOLUTION") == 8
#define ADC_BAT_BAJA	~(int)(ADCMAX_X_FVR / V_BAT_BAJA)
#define ADC_BAT_MEDIA	~(int)(ADCMAX_X_FVR / V_BAT_MEDIA)
#define ADC_BAT_ALTA	~(int)(ADCMAX_X_FVR / V_BAT_ALTA)
#define ADC_BAT_MIN		(~(int)(ADCMAX_X_FVR / V_BAT_MIN))
#define ADC_BAT_MAX		(~(int)(ADCMAX_X_FVR / V_BAT_MAX))
#elif getenv("ADC_RESOLUTION") == 10
#define ADC_BAT_BAJA	(~(long)(ADCMAX_X_FVR / V_BAT_BAJA) & 0x3FF)
#define ADC_BAT_MEDIA	(~(long)(ADCMAX_X_FVR / V_BAT_MEDIA) & 0x3FF)
#define ADC_BAT_ALTA	(~(long)(ADCMAX_X_FVR / V_BAT_ALTA) & 0x3FF)
#define ADC_BAT_MIN		(~(long)(ADCMAX_X_FVR / V_BAT_MIN) & 0x3FF)
#define ADC_BAT_MAX		(~(long)(ADCMAX_X_FVR / V_BAT_MAX) & 0x3FF)
#endif

#define RANGO_ADC_BAT	(ADC_BAT_MAX - ADC_BAT_MIN)

/* DEBUG */
//mostramos los diferentes valores de la configuracion seleccionada
#ifdef BAT_DEBUG

#warning Vbat: BAT_TYPE
#warning Vref: V_FVR

#warning Bateria BAJA a partir de: V_BAT_BAJA
#warning Bateria MEDIA a partir de: V_BAT_MEDIA
#warning Bateria ALTA a partir de: V_BAT_ALTA

#warning ADC BAJO a partir de: ADC_BAT_BAJA
#warning ADC MEDIO a partir de: ADC_BAT_MEDIA
#warning ADC ALTO a partir de: ADC_BAT_ALTA

#endif

#endif	/* BATTERY_READ_INTERNAL_H */

