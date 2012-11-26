#include "../../include/defs.h"

#define NON_PRINT			0x0
#define	MAX_KEYS			0x59
#define LEFT_SHIFT_MAKE 	0x2a
#define RIGHT_SHIFT_MAKE 	0x36
#define CAPS_LOCK	 		0x3a
#define ALT_MAKE 			0x38
#define ACCENT_MAKE			0x28
#define LEFT_SHIFT_BREAK 	0xaa
#define RIGHT_SHIFT_BREAK 	0xb6
#define ALT_BREAK 			0xb8
#define	KEYBOARD_AMOUNT		2

void		breakInt(int);
byte 		makeInt(int);

int 		shiftActivated = 0;
int 		capsLockActivated = 0;
int 		altActivated = 0;
int			changing_shell = 0;
int			current_keyboard = 0;
int			accent = 0;

char		keyboards[2][MAX_KEYS][2] = {
	
				{
	//MAKECODE							SPECIAL KEY
	/*00*/		{NON_PRINT,NON_PRINT},
	/*01*/		{NON_PRINT,NON_PRINT},	//Esc
	/*02*/		{'1','!'},	
	/*03*/		{'2','@'},	
	/*04*/		{'3','#'},	
	/*05*/		{'4','$'},	
	/*06*/		{'5','%'},	
	/*07*/		{'6','^'},	
	/*08*/		{'7','&'},	
	/*09*/		{'8','*'},	
	/*0A*/		{'9','('},	
	/*0B*/		{'0',')'},	
	/*0C*/		{'-','_'},	
	/*0D*/		{'=','+'},	
	/*0E*/		{'\b','\b'},			//Backspace
	/*0F*/		{NON_PRINT,NON_PRINT},	//Tab
	/*10*/		{'q','Q'},	
	/*11*/		{'w','W'},	
	/*12*/		{'e','E'},	
	/*13*/		{'r','R'},	
	/*14*/		{'t','T'},	
	/*15*/		{'y','Y'},	
	/*16*/		{'u','U'},	
	/*17*/		{'i','I'},	
	/*18*/		{'o','O'},	
	/*19*/		{'p','P'},	
	/*1A*/		{'[','{'},	
	/*1B*/		{']','}'},	
	/*1C*/		{'\n','\n'},			//Enter	
	/*1D*/		{NON_PRINT,NON_PRINT},	//Left Ctrl
	/*1E*/		{'a','A'},	
	/*1F*/		{'s','S'},	
	/*20*/		{'d','D'},	
	/*21*/		{'f','F'},	
	/*22*/		{'g','G'},	
	/*23*/		{'h','H'},	
	/*24*/		{'j','J'},	
	/*25*/		{'k','K'},	
	/*26*/		{'l','L'},	
	/*27*/		{';',':'},		
	/*28*/		{'\'','"'},	
	/*29*/		{'`','~'},	
	/*2A*/		{NON_PRINT,NON_PRINT},	//Left Shift
	/*2B*/		{'\\','|'},	
	/*2C*/		{'z','Z'},	
	/*2D*/		{'x','X'},	
	/*2E*/		{'c','C'},	
	/*2F*/		{'v','V'},	
	/*30*/		{'b','B'},	
	/*31*/		{'n','N'},	
	/*32*/		{'m','M'},	
	/*33*/		{',','<'},	
	/*34*/		{'.','>'},	
	/*35*/		{'/','?'},	
	/*36*/		{NON_PRINT,NON_PRINT},	//Right Shift
	/*37*/		{'*','*'},				//Keypad *
	/*38*/		{NON_PRINT,NON_PRINT},	//Left Alt
	/*39*/		{' ',' '},				//Space
	/*3A*/		{NON_PRINT,NON_PRINT},	//Caps Lock
	/*3B*/		{NON_PRINT,NON_PRINT},	//F1
	/*3C*/		{NON_PRINT,NON_PRINT},	//F2
	/*3D*/		{NON_PRINT,NON_PRINT},	//F3
	/*3E*/		{NON_PRINT,NON_PRINT},	//F4
	/*3F*/		{NON_PRINT,NON_PRINT},	//F5
	/*40*/		{NON_PRINT,NON_PRINT},	//F6
	/*41*/		{NON_PRINT,NON_PRINT},	//F7
	/*42*/		{NON_PRINT,NON_PRINT},	//F8
	/*43*/		{NON_PRINT,NON_PRINT},	//F9
	/*44*/		{NON_PRINT,NON_PRINT},	//F10
	/*45*/		{NON_PRINT,NON_PRINT},	//Num Lock
	/*46*/		{NON_PRINT,NON_PRINT},	//Scroll Lock
	/*47*/		{NON_PRINT,NON_PRINT},	//Keypad 7
	/*48*/		{NON_PRINT,NON_PRINT},	//Keypad 8 // Up- Arrow
	/*49*/		{NON_PRINT,NON_PRINT},	//Keypad 9
	/*4A*/		{NON_PRINT,NON_PRINT},	//Keypad -
	/*4B*/		{0x4b * 2,0x4b * 2},	//Keypad 4 // Left- Arrow
	/*4C*/		{NON_PRINT,NON_PRINT},	//Keypad 5
	/*4D*/		{0x4d * 2,0x4d * 2},	//Keypad 6 // Right- Arrow
	/*4E*/		{NON_PRINT,NON_PRINT},	//Keypad +
	/*4F*/		{NON_PRINT,NON_PRINT},	//Keypad 1
	/*50*/		{NON_PRINT,NON_PRINT},	//Keypad 2 // Down-Arrow
	/*51*/		{NON_PRINT,NON_PRINT},	//Keypad 3
	/*52*/		{NON_PRINT,NON_PRINT},	//Keypad 0
	/*53*/		{NON_PRINT,NON_PRINT},	//Keypad . // Supr
	/*54*/		{NON_PRINT,NON_PRINT},	//Sys Req (AT)
	/*55*/		{'+','*'},	
	/*56*/		{'+','*'},	
	/*57*/		{NON_PRINT,NON_PRINT},	//F11
	/*58*/		{NON_PRINT,NON_PRINT}},	//F12
	
			{
				//MAKECODE							SPECIAL KEY
	/*00*/		{NON_PRINT,NON_PRINT},
	/*01*/		{NON_PRINT,NON_PRINT},	//Esc
	/*02*/		{'1','!'},	
	/*03*/		{'2','"'},	
	/*04*/		{'3','#'},	
	/*05*/		{'4','$'},	
	/*06*/		{'5','%'},	
	/*07*/		{'6','&'},	
	/*08*/		{'7','/'},	
	/*09*/		{'8','('},	
	/*0A*/		{'9',')'},	
	/*0B*/		{'0','='},	
	/*0C*/		{' ','?'},	
	/*0D*/		{' ',' '},	
	/*0E*/		{'\b','\b'},			//Backspace
	/*0F*/		{NON_PRINT,NON_PRINT},	//Tab
	/*10*/		{'q','Q'},	
	/*11*/		{'w','W'},	
	/*12*/		{'e','E'},	
	/*13*/		{'r','R'},	
	/*14*/		{'t','T'},	
	/*15*/		{'y','Y'},	
	/*16*/		{'u','U'},	
	/*17*/		{'i','I'},	
	/*18*/		{'o','O'},	
	/*19*/		{'p','P'},	
	/*1A*/		{'`','['},	
	/*1B*/		{'+',']'},	
	/*1C*/		{'\n','\n'},			//Enter	
	/*1D*/		{NON_PRINT,NON_PRINT},	//Left Ctrl
	/*1E*/		{'a','A'},	
	/*1F*/		{'s','S'},	
	/*20*/		{'d','D'},	
	/*21*/		{'f','F'},	
	/*22*/		{'g','G'},	
	/*23*/		{'h','H'},	
	/*24*/		{'j','J'},	
	/*25*/		{'k','K'},	
	/*26*/		{'l','L'},	
	/*27*/		{0xa4,0xa5},		
	/*28*/		{'\'','}'},	
	/*29*/		{'<','>'},	
	/*2A*/		{NON_PRINT,NON_PRINT},	//Left Shift
	/*2B*/		{'<','>'},	
	/*2C*/		{'z','Z'},	
	/*2D*/		{'x','X'},	
	/*2E*/		{'c','C'},	
	/*2F*/		{'v','V'},	
	/*30*/		{'b','B'},	
	/*31*/		{'n','N'},	
	/*32*/		{'m','M'},	
	/*33*/		{',',';'},	
	/*34*/		{'.',':'},	
	/*35*/		{'-','_'},	
	/*36*/		{NON_PRINT,NON_PRINT},	//Right Shift
	/*37*/		{'*','*'},				//Keypad *
	/*38*/		{NON_PRINT,NON_PRINT},	//Left Alt
	/*39*/		{' ',' '},				//Space
	/*3A*/		{NON_PRINT,NON_PRINT},	//Caps Lock
	/*3B*/		{NON_PRINT,NON_PRINT},	//F1
	/*3C*/		{NON_PRINT,NON_PRINT},	//F2
	/*3D*/		{NON_PRINT,NON_PRINT},	//F3
	/*3E*/		{NON_PRINT,NON_PRINT},	//F4
	/*3F*/		{NON_PRINT,NON_PRINT},	//F5
	/*40*/		{NON_PRINT,NON_PRINT},	//F6
	/*41*/		{NON_PRINT,NON_PRINT},	//F7
	/*42*/		{NON_PRINT,NON_PRINT},	//F8
	/*43*/		{NON_PRINT,NON_PRINT},	//F9
	/*44*/		{NON_PRINT,NON_PRINT},	//F10
	/*45*/		{NON_PRINT,NON_PRINT},	//Num Lock
	/*46*/		{NON_PRINT,NON_PRINT},	//Scroll Lock
	/*47*/		{NON_PRINT,NON_PRINT},	//Keypad 7
	/*48*/		{NON_PRINT,NON_PRINT},	//Keypad 8 // Up- Arrow
	/*49*/		{NON_PRINT,NON_PRINT},	//Keypad 9
	/*4A*/		{NON_PRINT,NON_PRINT},	//Keypad -
	/*4B*/		{0x4b * 2,0x4b * 2},	//Keypad 4 // Left- Arrow
	/*4C*/		{NON_PRINT,NON_PRINT},	//Keypad 5
	/*4D*/		{0x4d * 2,0x4d * 2},	//Keypad 6 // Right- Arrow
	/*4E*/		{NON_PRINT,NON_PRINT},	//Keypad +
	/*4F*/		{NON_PRINT,NON_PRINT},	//Keypad 1
	/*50*/		{NON_PRINT,NON_PRINT},	//Keypad 2 // Down-Arrow
	/*51*/		{NON_PRINT,NON_PRINT},	//Keypad 3
	/*52*/		{NON_PRINT,NON_PRINT},	//Keypad 0
	/*53*/		{NON_PRINT,NON_PRINT},	//Keypad . // Supr
	/*54*/		{NON_PRINT,NON_PRINT},	//Sys Req (AT)
	/*55*/		{'+','*'},	
	/*56*/		{'+','*'},	
	/*57*/		{NON_PRINT,NON_PRINT},	//F11
	/*58*/		{NON_PRINT,NON_PRINT}}	//F12
	};

void int_09 (int scan_code)
{
	byte aux_chr = 0;
	int ctrl_chr = scan_code & 0x80;
  
	if (ctrl_chr)
		breakInt(scan_code);
	else
		aux_chr = makeInt(scan_code);
		
	if (aux_chr)
		write_buffer(aux_chr);
}

byte makeInt(int scan_code) 
{
	switch(scan_code) 
	{
		case RIGHT_SHIFT_MAKE:
			shiftActivated = 1;
			return 0;
		case LEFT_SHIFT_MAKE:
			shiftActivated = 1;
			return 0;
		case CAPS_LOCK:
			capsLockActivated = !capsLockActivated;
			return 0;
		case ALT_MAKE:
			altActivated = 1;
			return 0;
		case ACCENT_MAKE:
			if (current_keyboard == 1)
			{
				accent = 1;
				return 0;
			}
	}
	if (altActivated && 
		scan_code >= 0x02 && scan_code < 0x02 + MAX_SHELLS)
		changing_shell = 1;  
	if (accent && current_keyboard == 1)
	{
		if (shiftActivated ^ capsLockActivated)
		{
			accent = 0;			
		} else if (is_vowel(scan_code)) {
			accent = 0;
			return accent_vowel(scan_code);
		} else {
			write_buffer(keyboards[current_keyboard][ACCENT_MAKE][0]);
			accent = 0;
		}
	}
	if (is_letter(scan_code))
	{
		return keyboards[current_keyboard]
					[scan_code][shiftActivated ^ capsLockActivated];
	}
	return keyboards[current_keyboard][scan_code][shiftActivated];
}

void breakInt(int scan_code) 
{
  switch (scan_code)
  {
    case LEFT_SHIFT_BREAK:
      shiftActivated = 0;
    case RIGHT_SHIFT_BREAK:
      shiftActivated = 0;
    case ALT_BREAK:
      altActivated = 0;
  }
}

int is_letter(int scan_code)
{
	return ((scan_code >= 0x10 && scan_code <= 0x19) ||
			(scan_code >= 0x1e && scan_code <= 0x26) ||
			(scan_code >= 0x2c && scan_code <= 0x32));
}

int change_language(int index)
{
	if (index >= KEYBOARD_AMOUNT || index == -1)
		return 0;
	current_keyboard = index;
	return 1;
}

int is_vowel(int scan_code)
{
	return scan_code == 0x1e || scan_code == 0x16 || scan_code == 0x17
			|| scan_code == 0x18 || scan_code == 0x12;
}

int accent_vowel(int scan_code)
{
	switch (scan_code){
		case 0x1e:
			return 160;
		case 0x12:
			return 130;
		case 0x17:
			return 161;
		case 0x18:
			return 162;
		case 0x16:
			return 163;
	}
	return 0;
}
