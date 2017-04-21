#ifndef BATTERY_TYPES_H
#define	BATTERY_TYPES_H

/* 
 * Quedan muchos valores de baterias por declarar, pero no se han
 * hecho pruebas aun, asi que se dejan sin completar de momento.
 */

/* TIPOS DE BATERIA */

/* Bateria definida por el usuario */
//Cuando se usa esta bateria hay que declarar manualmente los valores comentados
//en nuestro propio header, no aqui!
#define BAT_CUSTOM		0
/*#define V_BAT_NOM		(0.0)
#define V_BAT_BAJA		(0.0)
#define V_BAT_MEDIA		(0.0)
#define V_BAT_ALTA		(0.0)
#define V_BAT_MIN		(0.0)
#define V_BAT_MAX		(0.0)*/

/* ---------------------------------- LiPo ---------------------------------- */
// Las baterias de LiPo comienzan con 1
/* 1 LiPo */
#define BAT_1LIPO		11
#define BAT_1LIPO_NOM	(3.7)	//voltaje nominal
#define BAT_1LIPO_BAJA	(3.2)	//a partir de este valor es bajo
#define BAT_1LIPO_MEDIA	(3.5)	//a partir de este valor es medio
#define BAT_1LIPO_ALTA	(4.0)	//a partir de este valor es alto
#define BAT_1LIPO_MIN	(3.2)	//este es el valor minimo que puede tener la bateria
#define BAT_1LIPO_MAX	(4.2)	//este es el valor maximo que puede tener la bateria

/* 2 LiPo */
#define BAT_2LIPO		12
#define BAT_2LIPO_NOM	(7.4)	//voltaje nominal
#define BAT_2LIPO_BAJA	(0.0)	//a partir de este valor es bajo
#define BAT_2LIPO_MEDIA	(0.0)	//a partir de este valor es medio
#define BAT_2LIPO_ALTA	(0.0)	//a partir de este valor es alto
#define BAT_2LIPO_MIN	(0.0)	//este es el valor minimo que puede tener la bateria
#define BAT_2LIPO_MAX	(0.0)	//este es el valor maximo que puede tener la bateria

/* 3 LiPo */
#define BAT_3LIPO		13
#define BAT_3LIPO_NOM	(11.1)	//voltaje nominal
#define BAT_3LIPO_BAJA	(0.0)	//a partir de este valor es bajo
#define BAT_3LIPO_MEDIA	(0.0)	//a partir de este valor es medio
#define BAT_3LIPO_ALTA	(0.0)	//a partir de este valor es alto
#define BAT_3LIPO_MIN	(0.0)	//este es el valor minimo que puede tener la bateria
#define BAT_3LIPO_MAX	(0.0)	//este es el valor maximo que puede tener la bateria

/* ----------------------------------- AA ----------------------------------- */
// Las baterias AA comienzan con 2
/* 1 AA */
#define BAT_1AA			21
#define BAT_1AA_NOM		(1.5)	//voltaje nominal
#define BAT_1AA_BAJA	(0.0)	//a partir de este valor es bajo
#define BAT_1AA_MEDIA	(0.0)	//a partir de este valor es medio
#define BAT_1AA_ALTA	(0.0)	//a partir de este valor es alto
#define BAT_1AA_MIN		(0.0)	//este es el valor minimo que puede tener la bateria
#define BAT_1AA_MAX		(0.0)	//este es el valor maximo que puede tener la bateria

/* 2 AA */
#define BAT_2AA			22
#define BAT_2AA_NOM		(3.0)	//voltaje nominal
#define BAT_2AA_BAJA	(0.0)	//a partir de este valor es bajo
#define BAT_2AA_MEDIA	(0.0)	//a partir de este valor es medio
#define BAT_2AA_ALTA	(0.0)	//a partir de este valor es alto
#define BAT_2AA_MIN		(0.0)	//este es el valor minimo que puede tener la bateria
#define BAT_2AA_MAX		(0.0)	//este es el valor maximo que puede tener la bateria

/* 3 AA */
#define BAT_3AA			23
#define BAT_3AA_NOM		(4.5)	//voltaje nominal
#define BAT_3AA_BAJA	(0.0)	//a partir de este valor es bajo
#define BAT_3AA_MEDIA	(0.0)	//a partir de este valor es medio
#define BAT_3AA_ALTA	(0.0)	//a partir de este valor es alto
#define BAT_3AA_MIN		(0.0)	//este es el valor minimo que puede tener la bateria
#define BAT_3AA_MAX		(0.0)	//este es el valor maximo que puede tener la bateria

/* 4 AA */
#define BAT_4AA			24
#define BAT_4AA_NOM		(6.0)	//voltaje nominal
#define BAT_4AA_BAJA	(0.0)	//a partir de este valor es bajo
#define BAT_4AA_MEDIA	(0.0)	//a partir de este valor es medio
#define BAT_4AA_ALTA	(0.0)	//a partir de este valor es alto
#define BAT_4AA_MIN		(0.0)	//este es el valor minimo que puede tener la bateria
#define BAT_4AA_MAX		(0.0)	//este es el valor maximo que puede tener la bateria

/* ---------------------------------- AAA ----------------------------------- */
// Las baterias AAA comienzan con 3
/* 1 AAA */
#define BAT_1AAA		31
#define BAT_1AAA_NOM	(1.5)	//voltaje nominal
#define BAT_1AAA_BAJA	(0.0)	//a partir de este valor es bajo
#define BAT_1AAA_MEDIA	(0.0)	//a partir de este valor es medio
#define BAT_1AAA_ALTA	(0.0)	//a partir de este valor es alto
#define BAT_1AAA_MIN	(0.0)	//este es el valor minimo que puede tener la bateria
#define BAT_1AAA_MAX	(0.0)	//este es el valor maximo que puede tener la bateria

/* 2 AAA */
#define BAT_2AAA		32
#define BAT_2AAA_NOM	(3.0)	//voltaje nominal
#define BAT_2AAA_BAJA	(0.0)	//a partir de este valor es bajo
#define BAT_2AAA_MEDIA	(0.0)	//a partir de este valor es medio
#define BAT_2AAA_ALTA	(0.0)	//a partir de este valor es alto
#define BAT_2AAA_MIN	(0.0)	//este es el valor minimo que puede tener la bateria
#define BAT_2AAA_MAX	(0.0)	//este es el valor maximo que puede tener la bateria

/* 3 AAA */
#define BAT_3AAA		33
#define BAT_3AAA_NOM	(4.5)	//voltaje nominal
#define BAT_3AAA_BAJA	(0.0)	//a partir de este valor es bajo
#define BAT_3AAA_MEDIA	(0.0)	//a partir de este valor es medio
#define BAT_3AAA_ALTA	(0.0)	//a partir de este valor es alto
#define BAT_3AAA_MIN	(0.0)	//este es el valor minimo que puede tener la bateria
#define BAT_3AAA_MAX	(0.0)	//este es el valor maximo que puede tener la bateria

/* 4 AAA */
#define BAT_4AAA		34
#define BAT_4AAA_NOM	(6.0)	//voltaje nominal
#define BAT_4AAA_BAJA	(0.0)	//a partir de este valor es bajo
#define BAT_4AAA_MEDIA	(0.0)	//a partir de este valor es medio
#define BAT_4AAA_ALTA	(0.0)	//a partir de este valor es alto
#define BAT_4AAA_MIN	(0.0)	//este es el valor minimo que puede tener la bateria
#define BAT_4AAA_MAX	(0.0)	//este es el valor maximo que puede tener la bateria

/* ---------------------------------- NiMh ---------------------------------- */
// Las baterias NiMh comienzan con 4
/* 1 NiMh */
#define BAT_1NIMH		41
#define BAT_1NIMH_NOM	(1.2)

/* 2 NiMh */
#define BAT_2NIMH		42
#define BAT_2NIMH_NOM	(2.4)

/* 3 NiMh */
#define BAT_3NIMH		43
#define BAT_3NIMH_NOM	(3.6)

/* 4 NiMh */
#define BAT_4NIMH		44
#define BAT_4NIMH_NOM	(4.8)

/* ---------------------------------- NiCd ---------------------------------- */
// Las baterias NiCd comienzan con 5

/* -------------------------------- 23A/27A --------------------------------- */
// Las baterias de 9v comienzan con 6
#define BAT_9V			61
#define BAT_9V_NOM		(9.0)
#define BAT_9V_BAJA		(0.0)	//a partir de este valor es bajo
#define BAT_9V_MEDIA	(0.0)	//a partir de este valor es medio
#define BAT_9V_ALTA		(0.0)	//a partir de este valor es alto
#define BAT_9V_MIN		(0.0)	//este es el valor minimo que puede tener la bateria
#define BAT_9V_MAX		(0.0)	//este es el valor maximo que puede tener la bateria

/* -------------------------------- 23A/27A --------------------------------- */
// Las baterias 23A/27A comienzan con 7
#define BAT_23A			71
#define BAT_23A_NOM		(12.0)
#define BAT_23A_BAJA	(11.0)	//a partir de este valor es bajo
#define BAT_23A_MEDIA	(11.5)	//a partir de este valor es medio
#define BAT_23A_ALTA	(12.1)	//a partir de este valor es alto
#define BAT_23A_MIN		(11.0)	//este es el valor minimo que puede tener la bateria
#define BAT_23A_MAX		(12.5)	//este es el valor maximo que puede tener la bateria

#define BAT_27A			72
#define BAT_27A_NOM		(12.0)


/* ---------------------------- Baterias de plomo --------------------------- */
// Las baterias de plomo comienzan con 8
#define BAT_PB_6V		81
#define BAT_PB_6V_NOM	(6.0)

#define BAT_PB_12V		82
#define BAT_PB_12V_NOM	(12.0)

#endif	/* BATTERY_TYPES_H */