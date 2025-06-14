/*
	emt.h
	8080 Machine Interpreter
		   by
		S.Takeoka

	Jan/04/1992
	Aug/18/1987
	Aug/15/1987
*/
/*
	EMT operation definition
*/
/*
	Standard BIOS Functions (someone is fake or NOP)
*/
#define	Bios_cboot	0x00	/* CPsim HALT */
#define	Bios_wboot	0x01
#define	Bios_const	0x02
#define	Bios_conin	0x03

#define	Bios_conout	0x04
#define	Bios_list	0x05		/* NOP */
#define	Bios_punch	0x06
#define	Bios_reader	0x07

#define	Bios_home	0x08		/* NOP */
#define	Bios_seldsk	0x09		/* fake with BIOS64.sys*/
#define	Bios_settrk	0x0a		/* NOP */
#define	Bios_setsec	0x0b		/* NOP */

#define	Bios_setdma	0x0c
#define	Bios_cpm_read	0x0d		/* NOP */
#define	Bios_cpm_write	0x0e		/* NOP */
#define	Bios_listst	0x0f		/* fake. always ready */

#define	Bios_sectran	0x10		/* NOP */

/*
	8080 Ver. 1.0
	Puncher,Reader Functions
*/
#define	Bios_pun_open	0x20
#define	Bios_pun_close	0x21

#define	Bios_rdr_open	0x24
#define	Bios_rdr_close	0x25

/*
	8080 Ver. 1.0
	UN?X File I/O Functions
*/
#define	Bios_unix_open	0x80
#define	Bios_unix_close	0x81
#define	Bios_unix_read	0x82
#define	Bios_unix_write	0x83

/*
	8080 Ver. 1.0
	UN?X invoke Process
*/
#define	Bios_unix_system	0x84


/*
	8080 Ver. 1.0
	BDOS Interface and Retunr to CCP without WBOOT
*/
#define	Emt_return_ccp	0xFE
#define	Emt_bdos	0xFF

