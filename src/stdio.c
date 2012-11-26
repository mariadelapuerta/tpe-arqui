#include "../include/stdarg.h"
#include "../include/defs.h"
#define BUFSIZ	256
#define CLEAR_BUFFER(b)	int k; for(k=0;k<BUFSIZ;k++)b[k]=0;
#define SWAP(a,b)		(*b)= (*a)^(*b); (*a)=(*a)^(*b);(*b)=(*a)^(*b);

extern SHELL* current_shell;

char buffer[BUFSIZ];
char aux_buffer[BUFSIZ];
char *values = "0123456789ABCDEF";
extern unsigned long freebytes;
int lastblockpos = 0;
extern memblock memblockmap[100];

int isdigit(char i)
{
	return (i >= '0' && i <= '9');
}

void putc(char c)
{
	_write(WRITE_ON_SCREEN, &c);
}

void putchar(char c)
{
	_write(WRITE, &c);
}

char getch()
{
	char c;
	_read(READCH, &c);
	return c;
}

char getc()
{
	char c;
	_read(READ, &c);
	return c;
}

int print(char * str)
{
	int i = 0;
	while (str[i])
		putc(str[i++]);
	return i;
}

int pow(int base, int exp)
{
	int acum = 1;
	int i = 0;
	while (i++ < exp)
		acum *= base;
	return acum;
}

int atoi(char * s)
{
	int i = 0, acum = 0, j = 0;
	while (s[i++]);
	i-=2;
	while (s[j])
	{
		acum += (s[j++] - '0') * pow(10, i);
		i--;
	}
	return acum;
}

void printItoa(int num, int base) 
{
	CLEAR_BUFFER(aux_buffer)
	
	int i = 0, len;
	while (num >= base)
	{
		aux_buffer[i++] = values[(num % base)];
		num /= base;
	}
	aux_buffer[i++] = values[num];
	len = i - 1;
	int index = 0;
	while (index < i/2) 
	{
		SWAP(&(aux_buffer[index]), &(aux_buffer[len]))
		index++;
		len--;
	}
	aux_buffer[i] = 0;
	print(aux_buffer);
}

void printUtoa(int num, int base) 
{
	CLEAR_BUFFER(aux_buffer)
	
	unsigned int i = 0, len;
	while (num >= base)
	{
		aux_buffer[i++] = values[(num % base)];
		num /= base;
	}
	aux_buffer[i++] = values[num];
	len = i - 1;
	int index = 0;
	while (index < i/2) 
	{
		SWAP(&(aux_buffer[index]), &(aux_buffer[len]))
		index++;
		len--;
	}
	aux_buffer[i] = 0;
	print(aux_buffer);
}

int htoi(char * s) {
    int  z, n;
    n = 0;

    for (z = 0; s[z] >= '0' && s[z] <= 'F'; ++z) {
        if (s[z] >= 'A' && s[z] <= 'F') {
            n = 10 + 16 * n + (s[z] - 'A');
        } else {
            n = 16 * n + (s[z] - '0');
		}
	}
	return n;
            
}

void static print_tokenise(char c, va_list * args)
{
	switch (c)
	{
		case 'd':
			printItoa(va_arg(*args, int), 10);
			break;
		case 's':
			print(va_arg(*args, char*));
			break;
		case 'x':
			print("0x");
			printItoa(va_arg(*args, int), 16);
			break;
		case 'b':
			printItoa(va_arg(*args, int), 2);
			break;
		case 'u':
			printUtoa(va_arg(*args, int), 10);
			break;
	}
}

void printf(const char* format, ...)
{
	va_list args;
	int i = 0, j;

	va_start(args,format);
	
	do
	{
		j = 0;
		while (format[i] != '%' && format[i] != 0)
		{
			buffer[j++] = format[i];
			i++;
		}
		print(buffer);
		CLEAR_BUFFER(buffer);
		if (!format[i])
			break;
		print_tokenise(format[++i], &args);
		i++;
	} while (format[i]);
	
	va_end(args);
}

int static scan_tokenise(char c, va_list * args, char * buf, int s_index)
{
	CLEAR_BUFFER(aux_buffer);
	int i = s_index, j = 0;
	
	char * s;
	int * d;
	unsigned int * u;
		
	switch (c)
	{
		case 'd':
			
			d = (int*)va_arg(*args, int*);
			while (isdigit(buf[i]))
				aux_buffer[i] = buf[i++];

			if (i != s_index)
				*d = atoi(aux_buffer);
			
			CLEAR_BUFFER(aux_buffer);
			return i;
			
		case 's':
		
			s = (char *)va_arg(*args, char*);
			while (buf[i] && buf[i] != ' ')
				aux_buffer[j++] = buf[i++];

			if (i != s_index) 
			{
				j = 0;
				while (aux_buffer[j])
					s[j] = aux_buffer[j++];
				s[j] = 0;
			}
			
			return i;
			break;
			
		case 'S':	
			while (buf[i])
				aux_buffer[j++] = buf[i++];
				
			s = (char *)va_arg(*args, char*);
			j = 0;
			while (aux_buffer[j])
				s[j] = aux_buffer[j++];
			s[j] = 0;
			return s_index + 1; //siempre asigna, 
							  //si estaba en el final asigno 
				//un string en null. sino asingo el resto del string
			break;
	}	
}

/* MODO DE USO
 * 
 * Para sacar del buffer dos strings separados por espacio
 * 
 * sscanf("comando arg1 arg2", "%s%S", cmds, arguments);
 * 
 * Te deja 	en cmds: "comando"
 * 	   en arguments: "arg1 arg2"	
 * 
 * */
int sscanf(char * buf, const char * fmt, ...)
{
	va_list args;
	int i = 0, j = 0, count = 0, aux = 0;

	va_start(args,fmt);
	
	do
	{
		while (fmt[i] != '%')
		{
			printf("%d", i);
			if (fmt[i] != buf[j] || buf[j] == 0)
				return count;
			i++;
			j++;
		}
		aux = j;
		j = scan_tokenise(fmt[++i], &args, buf, j);
		count += (j - aux) != 0; 
		i++;
		j++;
	} while (fmt[i] != 0); 
	
	va_end(args);
	
	return count;
}



int isPrime(int num) {
    if (num == 1) {
        return 0;
    }
    if ((num == 2) || (num == 3)) {
        return 1;
    } else {
        int i;
        for (i = 2; i < num/2; i++) {
            if (num % i == 0) {
                return 0;
            }
        }
    }
    return 1;
}

void* malloc(int size) {
	if (size <= 0) {
		printf("Cannot allocate bytes");
	} else {
		int i = 0;
		if (freebytes < size) {
			printf("Not enough free ram to allocate, free some bytes first");
		} else {
			while (i < 100) {
				if (memblockmap[i].free == 1) {      //if it is free
					if(memblockmap[i].size==size) {  //if it has the same size
						memblockmap[i].free = 0;     //set as used
						lastblockpos = i;
						printf("Re-allocated %d bytes at 0x%d", size, memblockmap[i].address);
						return memblockmap[i].address;
					} else {
						if(memblockmap[i].size==0) {   //free and size=0
							if(i==0) {
								memblockmap[i].address = (int*) 0x00100000;
							} else {
								memblockmap[i].address = memblockmap[i-1].address 
														+ memblockmap[i-1].size;
							}
							memblockmap[i].free = 0;
							memblockmap[i].size = size;
							freebytes = freebytes-size;
							lastblockpos = i;
							printf("Allocated %d bytes at 0x%d", size, memblockmap[i].address);
							return memblockmap[i].address;
						}
					}
				}
				i++;
			}
		}
		return NULL;
	}
}

void free(void * ptr) {
	int i = 0;
	while (i < 100) {
		if (memblockmap[i].address == ptr) {
            freebytes = freebytes + memblockmap[i].size;
			if (i == lastblockpos) {
				memblockmap[i].address = 0x00100000;
				memblockmap[i].size = 0;
				lastblockpos = lastblockpos -1;
			}
			printf("You have freed the allocated memory at 0x%d", memblockmap[i].address);
			memblockmap[i].free = 1;
			return;
		}
		i++;
	}
	printf("Pointer %d was never allocated, unable to free", ptr);
	
}

void printblock(memblock m) {
    if(m.size != 0) {
        printf("start address: 0x%d ", m.address);
        printf("\n");
        printf("block size: %d", m.size);
        printf("\n");
        if(m.free == 0)
            printf("free: no\n");
        else
            printf("free: yes\n");
    }
}
