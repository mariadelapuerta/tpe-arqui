#include "../include/kasm.h"
#include "../include/defs.h"

extern BUFFER focus_buf;

void write_buffer(byte character)
{
	focus_buf.buf_p[focus_buf.write_offset] = character;
	focus_buf.write_offset++;
	if (focus_buf.write_offset == BUFFER_SIZE) 
		focus_buf.write_offset = 0;
	focus_buf.key_stroke = true;
}

byte read_buffer() 
{
	if (!focus_buf.key_stroke)
		return 0;
	byte aux_ret = focus_buf.buf_p[focus_buf.read_offset];
	focus_buf.read_offset++;
	if (focus_buf.read_offset == BUFFER_SIZE)
		focus_buf.read_offset = 0;
	if (focus_buf.read_offset == focus_buf.write_offset)
		focus_buf.key_stroke = false;
	return aux_ret;  
}
