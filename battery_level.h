/* 
 * File:   NivelBateria_v0.3.h
 * Author: Martin
 * Version: v0.3
 * Fecha: 20/Jan/2016
 * 
 * ----------------------------- CHANGE LOG -----------------------------------
 * v0.3
 * -Se añade compatibilidad con pilas y baterias de mayor voltaje usando un
 * divisor de tension en un pin analogico externo
 * -Se añade una funcion que devuelve el voltaje leido (lectura ADC convertida en V)
 * -Se documentan a fondo todas las formulas usadas
 * -Se prueba el funcionamiento con lectura de FVR y los resultados son
 * perfectos, con una presicion de 2 decimales
 * 
 * v0.2
 * -Se modifican las constantes para que se puedan marcar los voltajes de corte
 * -Se permite escoger el voltaje de FVR
 * -Se optimizan algunos codigos y se agrupa ConfigFVR en una funcion
 * 
 * v0.1
 * -Version inicial
 * 
 * --------------------------- FUNCIONAMIENTO ---------------------------------
 * 1. Antes de nada incluir el header battery_types.h
 *		#include <battery_types.h>
 * 
 * 2. Definir el tipo de bateria usada con el define BAT_TYPE, usando los
 *    defines anteriores:
 *		#define BAT_TYPE	BAT_1LIPO
 * 
 * 3. Si se usa una bateria de voltaje mayor a 5v se utiliza un pin ADC para
 *    las lecturas. Si se usa una bateria de Vbat < 5v se usa el FVR interno
 *    automaticamente, pero si se prefiere usar un pin ADC externo declarar:
 *		#define BAT_EXTERNAL_ADC
 * 
 * 4. Cuando se usa el ADC externo hay que:
 *		+Configurar el ADC en el setup inicial, declarando el pin
 *       correspondiente como analogico
 *		+Declarar:
 *			*BAT_PIC_VREF	-> voltaje de referencia para el ADC
 *							  (seguramente el del regulador de voltaje)
 *			*BAT_R1			-> resistencia R1, valor en ohmios
 *			*BAT_R2			-> resistencia R2, valor en ohmios
 *			*BAT_AD_CH		-> que canal del ADC se usa para leer el voltaje
 * 
 * 5. Incluir battery_level.c (usando vX.X cuando corresponda)
 *		#include <battery_level_v0.3.c>
 * 
 * 5. Se puede definir BAT_DEBUG para ver valores (formulas) cuando se compila
 * 
 * ---- EJEMPLO PARA LECTURA EXTERNA CON ADC Y DIVISOR DE TENSION ----
#define BAT_TYPE		BAT_23A
#define BAT_ADC_CH		2
#define BAT_PIC_VREF	5.0		//se necesita una fuente estable para que los valores sean precisos
#define BAT_R1			68000	//ser lo mas exacto posible con los valores...
#define	BAT_R2			33000	//...para que los resultados sean mas precisos
 * ---- EJEMPLO PARA LECTURA INTERNA CON FVR ----
#define BAT_TYPE		BAT_2AA
#define FVR_DEF			VREF_ADC_1v024	//opcional, por defecto usa Vref = 2.048v
 * ---- SI QUEREMOS USAR UNA BATERIA NO EXISTENTE ----
#define BAT_TYPE		BAT_CUSTOM
#define V_BAT_NOM		(9.0)	//voltaje nominal
#define V_BAT_BAJA		(7.5)	//a partir de este valor es bajo
#define V_BAT_MEDIA		(8.5)	//a partir de este valor es medio
#define V_BAT_ALTA		(9.2)	//a partir de este valor es alto
#define V_BAT_MIN		(7.5)	//este es el valor minimo que puede tener la bateria
#define V_BAT_MAX		(10.0)	//este es el valor maximo que puede tener la bateria
 * 
 * --------------------------------- FUNCIONES ---------------------------------
 * -int NivelBateria0_3() = devuelve un valor entre 0 y 3 para usar con las
 *		constantes BAT_CRITICA, BAT_BAJA, BAT_MEDIA y BAT_ALTA
 * 
 * -int NivelBateria255() = devuelve un valor entre 0 y 255 del estado de la
 *		bateria, usando V_BAT_MIN y V_BAT_MAX como referencia
 * 
 * -int NivelBateria0_X(int Max) = devuelve un valor entre 0 y Max del estado de la
 *		bateria, usando V_BAT_MIN y V_BAT_MAX como referencia
 * 
 *	float ADCenVoltaje() = devuelve el valor de la bateria en formato float
 * 
 * -float ADCenVoltaje(short read) = devuelve el valor de la bateria en formato float
 *		segun el valor de read puede devolver 2 valores:
 * 
 *		> read = VOLT_0_VREF -> devuelve el voltaje leido en el ADC
 *		> read = VOLT_0_VEXT -> devuelve el voltaje antes de pasar por el divisor
 *		de tension usando los valores de R1 y R2 como referencia para el calculo.
 *		El valor devuelto es una aproximacion, y depende de la presicion de las
 *		resistencias y otros factores.
 * 
 */

#ifndef BATTERY_LEVEL_H
#define	BATTERY_LEVEL_H

/* REGISTROS */
#byte FVRCON = getenv("SFR:FVRCON")	//registro de FVR
#byte ADCON0 = getenv("SFR:ADCON0")	//registro A/D 0
#byte ADCON1 = getenv("SFR:ADCON1")	//registro A/D 1
#bit FVREN = getenv("BIT:FVREN")	//bit FVR enabled
#bit FVRRDY = getenv("BIT:FVRRDY")	//bit FVR ready
//#bit VREGPM1	= getenv("BIT:VREGPM1")	//Voltage Regulator Power Mode Selection bit

/* CONSTANTES */
#if getenv("ADC_RESOLUTION") == 8
#define ADC_MAX_VAL		255
#elif getenv("ADC_RESOLUTION") == 10
#define ADC_MAX_VAL		1023
#else
#ERROR "Resolucion del ADC no soportada."
#endif

//Hay que declarar BAT_TYPE para poder usar la libreria
#ifndef BAT_TYPE
#ERROR "Hay que declarar el tipo de bateria que se usa"
#endif



#define BAT_CRITICA		0
#define BAT_BAJA		1
#define BAT_MEDIA		2
#define BAT_ALTA		3

#define VOLT_0_VREF		0	//se usa para leer el voltaje entre 0v y Vref (o VDD)
#define VOLT_0_VEXT		1	//lee el voltaje externo (convierte el valor leido usando los valores de R1 y R2)

/* ESTADOS DE LAS BATERIAS SEGUN LA LECTURA DE VOLTAJE */
//Bateria personalizada
#if BAT_TYPE == BAT_CUSTOM
#ifndef V_BAT_NOM
#ERROR "Hay que declarar voltaje para V_BAT_NOM"
#endif

#ifndef V_BAT_BAJA
#ERROR "Hay que declarar voltaje para V_BAT_BAJA"
#endif

#ifndef V_BAT_MEDIA
#ERROR "Hay que declarar voltaje para V_BAT_MEDIA"
#endif

#ifndef V_BAT_ALTA
#ERROR "Hay que declarar voltaje para V_BAT_ALTA"
#endif

#ifndef V_BAT_MIN
#ERROR "Hay que declarar voltaje para V_BAT_MIN"
#endif

#ifndef V_BAT_MAX
#ERROR "Hay que declarar voltaje para V_BAT_MAX"
#endif
#endif

//1 Lipo
#if BAT_TYPE == BAT_1LIPO
#define V_BAT_NOM		BAT_1LIPO_NOM
#define V_BAT_BAJA		BAT_1LIPO_BAJA
#define V_BAT_MEDIA		BAT_1LIPO_MEDIA
#define V_BAT_ALTA		BAT_1LIPO_ALTA
#define V_BAT_MIN		BAT_1LIPO_MIN
#define V_BAT_MAX		BAT_1LIPO_MAX
#endif

//1 pila 23A
#if BAT_TYPE == BAT_23A
#define V_BAT_NOM		BAT_23A_NOM
#define V_BAT_BAJA		BAT_23A_BAJA
#define V_BAT_MEDIA		BAT_23A_MEDIA
#define V_BAT_ALTA		BAT_23A_ALTA
#define V_BAT_MIN		BAT_23A_MIN
#define V_BAT_MAX		BAT_23A_MAX
#endif

//para baterias de mas de 5v siempre usamos pin ADC externo
//para baterias de menos de 5v usamos FVR interno a menos que se declare BAT_EXTERNAL_ADC
//ATENCION: Los PICs de gama "LF" tienen un voltaje maximo de 3.6V, no de 5V
//por lo que para leer valores mayores a 3.6v (en realidad para cualquier valor
//mayor a Vdd) hay que usar la lectura externa por ADC
#if V_BAT_NOM > (5.0)
	#include <battery_read_external.h>
#else
	#ifdef BAT_EXTERNAL_ADC
		#include "battery_read_external.h"
	#else
		#include "battery_read_internal.h"
	#endif
#endif

/* VARIABLES */
int Save_FVRCON = 0;
int Save_ADCON0 = 0;
int Save_ADCON1 = 0;

/* PROTOTIPOS */
//publicas
int NivelBateria0_3(void);
int NivelBateria255(void);
int NivelBateria0_X(int Max);
#ifdef BATTERY_EXTERNAL_ADC_H
float ADCenVoltaje(short read);
#else
float ADCenVoltaje(void);
#endif
//privadas
void GuardarConfigADC(void);
void RestaurarConfigADC(void);
void ConfigurarADC(void);
#if getenv("ADC_RESOLUTION") == 8
int LeerADC(void);
#elif getenv("ADC_RESOLUTION") == 10
long LeerADC(void);
#endif

#endif	/* BATTERY_LEVEL_H */