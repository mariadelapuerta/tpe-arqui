#include "../include/kc.h"

void update_cursor(SHELL*);

/***************************************************************
*k_clear_screen
*
* Borra la pantalla en modo texto color.
****************************************************************/

void k_clear_screen() 
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;
	while(i < (80*25*2))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	};
}

/***************************************************************
*setup_IDT_entry
* Inicializa un descriptor de la IDT
*
*Recibe: Puntero a elemento de la IDT
*	 Selector a cargar en el descriptor de interrupcion
*	 Puntero a rutina de atencion de interrupcion	
*	 Derechos de acceso del segmento
*	 Cero
****************************************************************/

void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->cero = cero;
}

void initialize_buf (BUFFER focus_buf)
{
	int i;
	for ( i = 0; i < BUFFER_SIZE ; i++)	
		focus_buf.buf_p[i] = 0;
	focus_buf.read_offset = 0;
	focus_buf.write_offset = 0;
	focus_buf.key_stroke = false;
}

void clean_buffer(SHELL * current_shell)
{	
	int i;
	for (i = 0; i < MAX_LINE; i++)
		current_shell->line_buffer[i] = 0;
	current_shell->lbuf_offset = 0;
}

void initialize_shell(SHELL * shell)
{
	char * msg = "tpeARQ:$>";
	int i, j;
	for ( i = 0; i < SCREEN_SIZE ; i++)	
	{
		if (i % 2 == 0)
			shell->screen[i] = 0;
		else
			shell->screen[i] = 0;
	}
	i = 0, j = 0;
	while (msg[j])
	{
		shell->screen[i] = msg[j++];
		i++;
		shell->screen[i] = 0;
		i++;
	}
	clean_buffer(shell);
	shell->cursor = (strlen(msg)-1) * 2;
	shell->current_line = 0;
	update_cursor(shell);
}

void initialize_shells(SHELL * shells)
{
	int i;
	for ( i = 0 ; i < MAX_SHELLS ; i++)	
		initialize_shell(&(shells[i]));
}

unsigned long measure_ram(void)
{
	int * pos = (int*)0x100000;
	do
	{
		pos += 0x1000;
	} while (is_available(pos));
	
	return (unsigned long)pos;
}

int is_available(int * pos)
{
	*pos = CONTROL_INT;
	if (*pos == CONTROL_INT)
		return TRUE;
	return FALSE;
}

void update_cursor(SHELL* current_shell)
{
	unsigned short position = current_shell->cursor;
	if (position != 0) {
			_outb(0x3D4, 0x0F);
			_outb(0x3D5, (unsigned char)(position/2));
			
			_outb(0x3D4, 0x0E);
			_outb(0x3D5, (unsigned char )(((position/2)>>8)));
	}
}

