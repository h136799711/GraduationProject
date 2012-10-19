#ifndef H_HCINPUT_INCLUDED
#define	H_HCINPUT_INCLUDED



#pragma comment (lib,"dinput8.lib")

#include "dinput.h"

static	char         joyname[80];  // name of joystick

class HCInput
{

public:
	/*
		初始化DInput，
		main_instance: 窗口实例
	*/
	int  DInput_Init(HINSTANCE main_instance);
	void DInput_Shutdown(void);

	int  DInput_Init_Joystick(HWND main_window_handle,int min_x=-256, int max_x=256, 
							 int min_y=-256, int max_y=256, int dead_zone = 10);

	int  DInput_Init_Mouse(HWND main_window_handle);
	int  DInput_Init_Keyboard(HWND main_window_handle);
	int  DInput_Read_Joystick(void);
	int  DInput_Read_Mouse(void);
	int  DInput_Read_Keyboard(void);
	void DInput_Release_Joystick(void);
	void DInput_Release_Mouse(void);
	void DInput_Release_Keyboard(void);
	

public:
		
	// directinput globals
	LPDIRECTINPUT8       lpdi;       // dinput object
	LPDIRECTINPUTDEVICE8 lpdikey;    // dinput keyboard
	LPDIRECTINPUTDEVICE8 lpdimouse;  // dinput mouse
	LPDIRECTINPUTDEVICE8 lpdijoy;    // dinput joystick 
	GUID                 joystickGUID; // guid for main joystick
	// these contain the target records for all di input packets
	UCHAR keyboard_state[256]; // contains keyboard state table
	DIMOUSESTATE mouse_state;  // contains state of mouse
	DIJOYSTATE joy_state;      // contains state of joystick
	int joystick_found;        // tracks if stick is plugged in




};

#endif