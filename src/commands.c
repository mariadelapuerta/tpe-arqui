#include "../include/stdio.h"
#include "../include/kasm.h"
#include "../include/defs.h"

int	change_language(int);
int static get_index(char * s);

char aux_buffer1[30];
char aux_buffer2[30];
extern DESCR_INT idt[0xFF];
extern memblock memblockmap[100];
extern unsigned long freebytes;

void echo_exec(char * args)
{
	printf(args);
}

void test_exec(char * args)
{
	int num = 34;
	char * s = "Esto es una prueba";
	printf("Decimal: %d\nHexa: %x\nString: %s\nBinario: %b\n",
			num, num, s, num);
}

void language_exec(char * args)
{
	if (sscanf(args, "%s%s", aux_buffer1, aux_buffer2) > 1)
	{
		printf("Invalid arguments");
		return;
	}
	
	if (change_language(get_index(aux_buffer1)))
		printf("New keyboard language: %s", aux_buffer1);
	else
		printf("That's not a valid language");
}

void help_exec(char * args) {
	printf("\nHelp Desk\n\n");
	printf("echo [args]                Prints [args] on screen\n");
	printf("free [args]                Frees block of memory in dir [args]\n");
	printf("help                       Opens Help Desk\n");
	printf("idt                        Shows IDT information\n");
	printf("idt [del | add] [arg]      Use [del] to delete and [add] to add [arg] int\n");
	printf("language [EN | ES]         Changes keyboard disposition\n");
	printf("malloc [arg]               Allocates [arg] bytes\n");
	printf("mem                        Shows available blocks of memory\n");
	printf("test                       Tests printf function\n");
	
}

void idt_exec(char * args) {	
	int count = sscanf(args, "%s%s", aux_buffer1, aux_buffer2);
	if(strlen(args)==1){
		int i=0;
		printf("\nIDT (Interrupt Descriptor Table)\n\n");
		printf("INT -- Higher Offset --  Lower Offset -- Selector\n\n");
		DESCR_INT aux;
		for(i; i<0xFF; i++){
			aux = idt[i];
			if (aux.offset_h != 0)
				printf("%x -- %d -- %d -- %d\n", i, aux.offset_l, aux.offset_h, aux.selector);
		}
	} else if (!strcmp(aux_buffer1, "del")) {
		int hex_int = htoi(aux_buffer2);
		if(idt[hex_int].offset_h == 0) {
			printf("The INT you are trying to delete doesn't exist");
		} else {
			printf("the INT %sh has been deleted from IDT", aux_buffer2); 
			if (hex_int == 0x9)
				_mascaraPIC1(0xFE);
			setup_IDT_entry(&idt[hex_int], 0x08, 0, ACS_INT, 0);
		}
	} else if (!strcmp(aux_buffer1, "add")) {
		int hex_int = htoi(aux_buffer2);
		if(idt[hex_int].offset_h != 0) {
			printf("The INT already exists");
		} else {
			if(hex_int >= 255) {
				printf("Couldn't add INT %s", aux_buffer2);
			} else {
				printf("The INT %sh has been added to IDT", aux_buffer2);
				setup_IDT_entry(&idt[hex_int], 0x08, &_int_generic_hand, ACS_INT, 0);
			}
		}	
	}
}	

void malloc_exec(char * args) {
	malloc(atoi(args));
}

void free_exec(char * args) {
	char * aux = args+2;
	free(atoi(aux));
}

void memory_exec(char * args) {
	int i;
	printf("Total memory available: %d bytes\n", freebytes);
	for(i = 0; i<100; i++){
		if(memblockmap[i].address != 0){
			printblock(memblockmap[i]);
		}
	}
}

void prime_exec(char * args) {
	int num = atoi(args);
    if (isPrime(num) == 1) {
        printf("%d is a prime number.", num);
    } else {
        printf("%d is not a prime number.", num);
    }
 }


int static get_index(char * s)
{
	if (!strcmp(s, "ES"))
		return 1;
	if (!strcmp(s, "EN"))
		return 0;
	return -1;
}

