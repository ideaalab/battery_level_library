/* 
 * Author: Martin
 * Fecha: 20/Jan/2016
 * 
 * ----------------------------- CHANGE LOG -----------------------------------
 * v0.3
 * -Se a?ade compatibilidad con pilas y baterias de mayor voltaje usando un
 * divisor de tension en un pin analogico externo
 * -Se a?ade una funcion que devuelve el voltaje leido (lectura ADC convertida en V)
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
 * 2. Incluir battery_level.c al proyecto DESPUES de las constantes de
 *	  configuracion que se muestran abajo
 *		#include <battery_level.c>
 * 
 * 3. Definir el tipo de bateria usada con el define BAT_TYPE, usando los
 *    defines dentro de battery_types.h
 *		#define BAT_TYPE	BAT_1LIPO
 * 
 * LECTURA INTERNA: si se alimenta el PIC con una bateria de < 5v se puede
 * leer su voltaje sin usar pines.
 * 
 * 4. El voltaje de referencia del FVR por defecto es 2.048v, pero si se quiere
 *    usar otro declarar uno de estos (mirar los valores disponibles en el ".h"
 *	  del PIC usado):
 *		#define FVR_DEF		VREF_ADC_v1024
 *		#define FVR_DEF		VREF_ADC_2v048
 *		#define FVR_DEF		VREF_ADC_4v096
 * 
 * LECTURA EXTERNA: cuando el voltaje a leer supera el voltaje del PIC tenemos
 * que usar un divisor de voltaje externo y un pin ADC para leerlo.
 * 
 * 4. Cuando se usa el ADC externo hay que:
 *		+Si se quiere usar Vref interno del PIC declarar BAT_VREF_INTERNO con la
 *		constantes del PIC (mirar los valores disponibles en el ".h" del PIC usado)
 *			ej:
 *			#define BAT_VREF_INTERNO	VREF_ADC_4v096
 * 
 *		+Si se quiere usar Vref externo de un pin declarar BAT_VREF_EXTERNO con el
 *		voltaje que se usa como referencia (el pin correspondiente a Vref high
 *		debe estar conectado al voltaje de referencia)
 *			ej:
 *			#define BAT_VREF_EXTERNO	4.2
 * 
 *		+Si se usa el voltaje Vdd como referencia declarar BAT_PIC_VREF	con el
 *		voltaje de referencia para el ADC
 *			ej:
 *			#define BAT_VREF_VDD		5.0
 * 
 * 5. Declarar BAT_R1 y BAT_R2 con los valores de resistencia en ohmios
 *		ej:
 *		#define BAT_R1				82000
 *		#define BAT_R2				30000
 * 
 * 6. Declarar BAT_ADC_CH con el canal del ADC se usa para leer el voltaje
 *		ej:
 *		#define BAT_ADC_CH			3
 * 
 * VARIANTES:
 *	+Si queremos leer el valor de una bateria de voltaje < 5v, pero que no esta
 *	alimentando el PIC tenemos que usar un pin ADC para leerlo. Para voltajes
 *	< 5v la libreria escoge automaticamente la lectura interna, pero hay que
 *  cambiar a lectura externa con el siguiente define:
 *		#define BAT_EXTERNAL_ADC
 * 
 *	+Si se quiere usar la lectura con pin ADC pero sin divisor de voltaje usar
 *	estos valores para R1 y R2:
 *		#define BAT_R1				0
 *		#define BAT_R2				1
 * 
 *	+Se puede definir BAT_DEBUG para ver valores (formulas) cuando se compila
 * 
 * CONSIDERANCIONES:
 * +Usar resistencias de 0.1% de tolerancia para el divisor de tension. Asi se
 * obtienen valores mas precisos.
 * +Tener cuidado de no sobrepasar Vref en el pin ADC.
 * +Si queremos leer un voltaje ajeno a nuestro circuito tenemos que unir GND
 *	del circuito externo con GND de nuestro circuito.
 * +Ejemplos de resistencias (en la precision no se tiene en cuenta la tolerancia
 *	de las resistencias):
 * 
 *		Voltajes hasta 15.3v, con Vref = Vdd = 5v
 *			-R1 = 33K ohm
 *			-R2 = 68K ohm
 *			-Presicion: 0.06v (ADC 8bits) / 0.015v (ADC 10bits)
 * 
 *		Voltajes hasta 14.6v, con Vref interno = 4.096v
 *			-R1 = 39K ohm
 *			-R2 = 100K ohm
 *			-Presicion: 0.057v (ADC 8bits) / 0.014v (ADC 10bits)
 * 
 *		Voltajes hasta 15.3v, con Vref interno = 4.096v
 *			-R1 = 30K ohm
 *			-R2 = 82K ohm
 *			-Presicion: 0.06v (ADC 8bits) / 0.015v (ADC 10bits)
 * 
 * ---- EJEMPLO PARA LECTURA EXTERNA CON ADC Y DIVISOR DE TENSION ----
#define BAT_TYPE		BAT_23A
#define BAT_ADC_CH		2
#define BAT_VREF_VDD	5.0		//se necesita una fuente estable para que los valores sean precisos
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
 *		Es el voltaje directo de la bateria.
 * 
 */

#ifndef BATTERY_LEVEL_H
#define	BATTERY_LEVEL_H

/* REGISTROS */
#byte FVRCON = getenv("SFR:FVRCON")	//registro de FVR
//segun el PIC tenga una ADC de 8 o 10 bits utiliza uno o mas registros para almacenar la lectura
#if getenv("SFR_VALID:ADCON") == TRUE
#byte ADCON = getenv("SFR:ADCON")	//registro A/D 0
#endif
#if getenv("SFR_VALID:ADCON0") == TRUE
#byte ADCON0 = getenv("SFR:ADCON0")	//registro A/D 0
#endif
#if getenv("SFR_VALID:ADCON1") == TRUE
#byte ADCON1 = getenv("SFR:ADCON1")	//registro A/D 1
#endif
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

#define BAT_CRITICA		0	//por debajo de 
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
//segun el PIC tenga una ADC de 8 o 10 bits utiliza uno o mas registros para almacenar la lectura
#if getenv("SFR_VALID:ADCON") == TRUE
int Save_ADCON = 0;
#endif
#if getenv("SFR_VALID:ADCON0") == TRUE
int Save_ADCON0 = 0;
#endif
#if getenv("SFR_VALID:ADCON1") == TRUE
int Save_ADCON1 = 0;
#endif

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