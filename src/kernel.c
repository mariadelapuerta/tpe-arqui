#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/stdio.h"

DESCR_INT idt[0xFF];			/* IDT de 10 entradas*/
IDTR idtr;						/* IDTR */
memblock memblockmap[100];
unsigned long freebytes;

BUFFER 	focus_buf;
SHELL	shells[MAX_SHELLS];

int		current_shell_index = 0;
SHELL*	current_shell;

extern char * video;
extern byte received_char;
extern int sending;

int pos = 0;

void int_08() {
}

/**********************************************
kmain() 
Punto de entrada de código C.
*************************************************/

kmain() 
{

/* Borra la pantalla. */ 

	freebytes = measure_ram();
	k_clear_screen();


/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */

	setup_IDT_entry (&idt[0x08], 0x08, 
					(dword)&_int_08_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x09], 0x08, 
					(dword)&_int_09_hand, ACS_INT, 0);
	setup_IDT_entry (&idt[0x80], 0x08, 
					(dword)&_int_80_hand, ACS_INT, 0);
	
/* Carga de IDTR    */

	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

	_Cli();
	
	memblock emptyblock;

	emptyblock.address = 0;
	emptyblock.size = 0;
	emptyblock.free = 1; //1=free, 0=used

	int i;
	for (i = 0; i < 100; ++i) {
		memblockmap[i] = emptyblock;
	}
	
/* Habilito interrupcion de timer tick*/

	_mascaraPIC1(0xFC);
	_mascaraPIC2(0xFF);
        
	_Sti();
	
	initialize_buf(focus_buf);
	initialize_shells(shells);
	
	select_shell(current_shell_index);

	while(1)
		run_shell();	
}

