#include "../../include/defs.h"

#define UP_ARROW			0x90
#define DOWN_ARROW			0xA0
#define LEFT_ARROW			0x96
#define RIGHT_ARROW			0x9A

#define out_video(pos, arg)	write_sys(arg, pos); \
							current_shell->screen[(pos)] = (arg)

#define	add_to_buffer(c)	current_shell->line_buffer \
							[current_shell->lbuf_offset++] = (c);
							
#define clear_buf_from(i)	int j; for(j=i;j<256;j++){\
							current_shell->line_buffer[j] = 0;
							
							
#define PROMPT				"tpeARQ:$>"

byte static		read(void);
void static		write(int, byte);
void static		write_sys(char, int);
byte static		read_sys(void);
int static		special_char_behaviour(byte);
void static		check_line(void);
void static		jump_line(void);
void			repaint_screen(void);
void			update_cursor(SHELL*);
byte static 	read_ch();

extern SHELL*	current_shell;
extern BUFFER	focus_buf;
extern int		changing_shell;
char 			aux[SCREEN_SIZE-80*2];

char * video = (char *) VID_MEM;

void int_80(int fd, byte * data)
{
	switch (fd)
	{
		case WRITE:
			write(fd, *data);
			break;
		case WRITE_ON_SCREEN:
			write(fd, *data);
			break;
		case READ:
			*data = read();
			break;
		case READCH:
			*data = read_ch();
			break;
	}
	
	update_cursor(current_shell);
}

byte static read_ch()
{
	if (!focus_buf.key_stroke)
		return -1;
	return read_sys();
}

byte static read()
{
	while (!focus_buf.key_stroke);
	
	byte chr_ret = read_sys();
	if (changing_shell)
	{
		select_shell( !(chr_ret-'0') ? 9 : chr_ret - '0' - 1);
		changing_shell = 0;
		return 0;
	}
	return chr_ret;
}

void static write(int fd, byte data)
{
	char spc_chr = special_char_behaviour(data);
	
	if (spc_chr && spc_chr != '\b')
		return;
	
	if (fd == WRITE)
	{
		if (!spc_chr) 
		{
			if (current_shell->lbuf_offset < MAX_LINE - 1) 
			{
				add_to_buffer(data);
			}	else {
					return;
			}
		} else {
			if (current_shell->lbuf_offset)
			{
				current_shell->
					line_buffer[--current_shell->lbuf_offset] = 0;
			}
			return;
		}
	}
	
	if (current_shell->cursor >= 4000) 
	{
		jump_line();
		current_shell->cursor -= 160;
	}
		
	out_video(current_shell->cursor, data);
		
	current_shell->cursor+=2;
}

int static special_char_behaviour(byte data)
{
	int i; 
	int * cursor = &(current_shell->cursor);
	switch (data) 
	{
		case 0:
			return 1;
		case '\n':
			if (current_shell->cursor + 160 >= 4000)
			{
				jump_line();
				*cursor -= ((*cursor % 160));
			} else {
				*cursor += (160 - *cursor % 160);
			}
			return '\n';
		case '\t':
			for ( i = 1; i <= 3 ; i++)
				out_video(*cursor+i*2, ' ');
			*cursor+=6;
			return '\t';
		case '\b': {
			char * scr = current_shell->screen;
			char * lbuf = current_shell->line_buffer;
			int i = 0;
			if (current_shell->lbuf_offset % 80 == 0)
			{				
				int n = strlen(PROMPT);
				if (*cursor % 160 - n*2 >= 0)
				{
					int aux = (*cursor%160);
					while (scr[*cursor+i*2] != 0)
					{
						out_video(*cursor + (i-1)*2, scr[*cursor+i*2]);
						aux = (*cursor%160);
						lbuf[aux + (i-1)] = lbuf[aux + i];
						i++;
					}
					lbuf[aux + (i-1)] = 0;
					out_video(*cursor + (i-1)*2, 0);
					*cursor -= 2;
				}
			} else {
					int aux = (*cursor%160);
					while (scr[*cursor+i*2] != 0)
					{
						out_video(*cursor + (i-1)*2, scr[*cursor+i*2]);
						aux = (*cursor%160);
						lbuf[aux+ (i-1)] = lbuf[aux + i];
						i++;
					}
					out_video(*cursor + (i-1)*2, 0);
					lbuf[aux+ (i-1)] = 0;
					*cursor -= 2;
			}
			return '\b';
			}
		case '\a':
			return 1;
		case LEFT_ARROW:
			if (*cursor % 160 - 14 != 0) {
				*cursor -= 2;
				current_shell->lbuf_offset --;
			}
			return LEFT_ARROW;
		case RIGHT_ARROW:
			if (current_shell->screen[*cursor+2] != 0) {
				*cursor += 2;
				current_shell->lbuf_offset ++;
			}
			return RIGHT_ARROW;
	}
	return 0;
}

void static check_line()
{
	if (current_shell->current_line % 25 == 0)
	{
		jump_line();
		current_shell->current_line--;
	}
}

void static jump_line()
{
	int i, j;
	
	for (i = 80*2; i < SCREEN_SIZE; i++)
		aux[i-80*2] = current_shell->screen[i];

	for (i = 0; i < SCREEN_SIZE; i++)
		current_shell->screen[i] = aux[i];

	repaint_screen();
		
}

void repaint_screen () 
{
    int i = 0;
    while (i < SCREEN_SIZE) {
		write_sys(current_shell->screen[i], i);
		i++;
		write_sys(0x0D, i);
		i++;
    }
}

void write_sys(char c, int screen_position)
{
	video[screen_position] = c;
}

byte read_sys()
{
	return read_buffer();
}
