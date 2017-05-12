/* 
 * File:   battery_external_adc.h
 * Author: Martin
 *
 * Created on 11 de enero de 2017, 13:21
 */

#ifndef BATTERY_READ_EXTERNAL_H
#define	BATTERY_READ_EXTERNAL_H

/*
 * ------------------------------ CALCULOS ------------------------------------
 * CALCULOS PARA LEER VOLTAJE EXTERNo (Vbat > 5v):
 * Se usa un divisor de tension:
 * 
 *	           R1                R2
 *	Vin ____/\/\/\/\____._____/\/\/\/\____ GND
 *                      |
 *                      |
 *                    Vout (VoutADC)
 * 
 * Vin = Voltaje directo de la bateria
 * Vout = Voltaje de salida del divisor de tension, conectado a un pin ADC
 * R1 / R2 = Resistencias usadas en el circuito, valores en ohmios
 * 
 * VinMax = Valor maximo permitido para Vin
 * VoutMax = es el valor maximo que puede leer el ADC, el voltaje de funcionamiento
 *			 del PIC o Vref si se utiliza
 * VoutADC = Valor del ADC leido en Vout
 * Vref = Voltaje de funcionamiento del PIC (ej. egulador de voltaje) o Vref utilizado
 * ADCMaxVal = Valor maximo del ADC. 8bits = 255 / 10bits = 1023
 * 
 * Formulas de divisor de tension:
 *	- Vout = R2 / (R1 + R2) * Vin								[OK]
 *	- Vin = (R1 + R2) / R2 * Vout								[OK]
 * 
 *  - Vout / Vin = R2 / (R1 + R2)
 *  - Vin / Vout = (R1 + R2) / R2
 * 
 * Formulas de conversion de ADC en voltaje:
 *  - Vout = VoutADC / ADCMaxVal * Vref							[OK]
 * 
 * Formula para convertir VoutADC en Vin:
 *	- Vin = ((R1 + R2) / R2) * (VoutADC / ADCMaxVal * Vref)		[OK]
 *		  = [((R1 + R2) * Vref) / (R2 * ADCMaxVal)] * VoutADC	[OK]
 *		  = BAT_PROPORCION_IN_OUT * BAT_VOLTxADC * VoutADC		[OK]
 * 
 * Formula para obtener VoutADC a partir de Vin:
 *	- VoutADC = (Vin * ADCMaxVal * R2) / ((R1 + R2) * Vref)		[OK]
 *			  = [(ADCMaxVal * R2) / ((R1 + R2) * Vref)] * Vin	[OK]
 * 
 * COMO CALCULAR LAS RESISTENCIAS R1 y R2
 *	- VinMax = voltaje maximo de la bateria + margen de seguridad
 *	- Vref = voltaje del PIC o Vref
 *	- Utilizando la formula probar diferentes valores estandar hasta encontrar la proporcion adecuada
 *		Vref / VinMax = R2 / (R1 + R2)							[OK]
 *	- Las resistencias tendran un consumo constante en el circuito,
 *	  elegir valores elevados (> 10K) para disminuir el consumo
 *		Vin / (R1 + R2) = Consumo en A							[OK]
 *		Vin / (R1 + R2) * 1000 = Consumo en mA					[OK]
 */

#ifndef BAT_ADC_CH
#ERROR "Hay que declarar el canal del ADC que se usa para leer el voltaje"
#endif

#if defined(BAT_VREF_INTERNO)
	#if BAT_VREF_INTERNO == VREF_ADC_1v024
	#define V_REF			(1.024)
	#elif BAT_VREF_INTERNO == VREF_ADC_2v048
	#define V_REF			(2.048)
	#elif BAT_VREF_INTERNO == VREF_ADC_4v096
	#define V_REF			(4.096)
	#endif
#elif defined(BAT_VREF_EXTERNO)
	#define V_REF	BAT_VREF_EXTERNO
#elif defined(BAT_VREF_VDD)
	#define V_REF	BAT_VREF_VDD
#else
#ERROR "Hay que declarar el voltaje de referencia o el voltaje al que funciona el PIC"
#endif

#ifndef BAT_R1
#ERROR "Hay que declarar R1 del divisor de tension"
#endif

#ifndef BAT_R2
#ERROR "Hay que declarar R2 del divisor de tension"
#endif

#define BAT_PROPORCION_IN_OUT	((float)(BAT_R1 + BAT_R2) / BAT_R2)	// Vin / Vout
#define BAT_PROPORCION_OUT_IN	((float)BAT_R2 / (BAT_R1 + BAT_R2))	// Vout / Vin
#define BAT_ADCxVOLT			((float)ADC_MAX_VAL / V_REF)		// cuantas uds ADC equivale 1v
#define BAT_VOLTxADC			((float)V_REF / ADC_MAX_VAL)		// cuantos volts equivalen 1 ADC
#define BAT_VIN_MAX				(BAT_PROPORCION_IN_OUT * V_REF)		// voltaje maximo que podemos leer

//Vin * (ADCMaxVal / Vref) * (R2 / (R1 + R2))
#define ADC_BAT_BAJA	(long)(V_BAT_BAJA * BAT_ADCxVOLT * BAT_PROPORCION_OUT_IN)
#define ADC_BAT_MEDIA	(long)(V_BAT_MEDIA * BAT_ADCxVOLT * BAT_PROPORCION_OUT_IN)
#define ADC_BAT_ALTA	(long)(V_BAT_ALTA * BAT_ADCxVOLT * BAT_PROPORCION_OUT_IN)
#define ADC_BAT_MIN		(long)(V_BAT_MIN * BAT_ADCxVOLT * BAT_PROPORCION_OUT_IN)
#define ADC_BAT_MAX		(long)(V_BAT_MAX * BAT_ADCxVOLT * BAT_PROPORCION_OUT_IN)


#define RANGO_ADC_BAT	(ADC_BAT_MAX - ADC_BAT_MIN)

/* DEBUG */
//mostramos los diferentes valores de la configuracion seleccionada
#ifdef BAT_DEBUG

#warning Vbat: V_BAT_NOM
#warning Vbat maximo permitido: BAT_VIN_MAX
#warning Vref: V_REF
#warning Consumo de R1 + R2: (V_BAT_NOM / (BAT_R1 + BAT_R2) * 1000) mAh

#warning Bateria BAJA a partir de: V_BAT_BAJA
#warning Bateria MEDIA a partir de: V_BAT_MEDIA
#warning Bateria ALTA a partir de: V_BAT_ALTA

#warning ADC BAJO a partir de: ADC_BAT_BAJA
#warning ADC MEDIO a partir de: ADC_BAT_MEDIA
#warning ADC ALTO a partir de: ADC_BAT_ALTA

#endif

#endif	/* BATTERY_READ_EXTERNAL_H */