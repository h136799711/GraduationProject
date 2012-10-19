#include "HCInput.h"

//static char joyname[80];

BOOL CALLBACK DInput_Enum_Joysticks(LPCDIDEVICEINSTANCE lpddi,
									LPVOID guid_ptr) 
{
	// this function enumerates the joysticks, but
	// stops at the first one and returns the
	// instance guid of it, so we can create it
	
	*(GUID*)guid_ptr = lpddi->guidInstance; 
	
	// copy name into global
	strcpy(joyname, (char *)lpddi->tszProductName);
	
	// stop enumeration after one iteration
	return(DIENUM_STOP);
	
} // end DInput_Enum_Joysticks

int HCInput::DInput_Init(HINSTANCE main_instance)
{
	
	if (FAILED(DirectInput8Create(main_instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&lpdi,NULL)))
		return(0);
	return 1;
}

void HCInput::DInput_Shutdown()
{
	if(lpdi || lpdi == NULL)
		lpdi->Release();
}

int HCInput::DInput_Init_Joystick(HWND main_window_handle,int min_x,int max_x,int min_y,int max_y,int dead_zone)
{
	lpdi->EnumDevices(DI8DEVCLASS_GAMECTRL,
		DInput_Enum_Joysticks,
		&joystickGUID,
		DIEDFL_ATTACHEDONLY);
	LPDIRECTINPUTDEVICE lpdijoy_temp = NULL;
	
	if(FAILED(lpdi->CreateDevice(joystickGUID,&lpdijoy,NULL)))
		return 0;
	
	// set cooperation level
	if (FAILED(lpdijoy->SetCooperativeLevel(main_window_handle, 
		DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		return(0);
	
	// set data format
	if (lpdijoy->SetDataFormat(&c_dfDIJoystick)!=DI_OK)
		return(0);
	
	// set the range of the joystick
	DIPROPRANGE joy_axis_range;
	
	// first x axis
	joy_axis_range.lMin = min_x;
	joy_axis_range.lMax = max_x;
	
	joy_axis_range.diph.dwSize       = sizeof(DIPROPRANGE); 
	joy_axis_range.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	joy_axis_range.diph.dwObj        = DIJOFS_X;
	joy_axis_range.diph.dwHow        = DIPH_BYOFFSET;
	
	lpdijoy->SetProperty(DIPROP_RANGE,&joy_axis_range.diph);
	
	// now y-axis
	joy_axis_range.lMin = min_y;
	joy_axis_range.lMax = max_y;
	
	joy_axis_range.diph.dwSize       = sizeof(DIPROPRANGE); 
	joy_axis_range.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	joy_axis_range.diph.dwObj        = DIJOFS_Y;
	joy_axis_range.diph.dwHow        = DIPH_BYOFFSET;
	
	lpdijoy->SetProperty(DIPROP_RANGE,&joy_axis_range.diph);
	
	
	// and now the dead band
	DIPROPDWORD dead_band; // here's our property word
	
	// scale dead zone by 100
	dead_zone*=100;
	
	dead_band.diph.dwSize       = sizeof(dead_band);
	dead_band.diph.dwHeaderSize = sizeof(dead_band.diph);
	dead_band.diph.dwObj        = DIJOFS_X;
	dead_band.diph.dwHow        = DIPH_BYOFFSET;
	
	// deadband will be used on both sides of the range +/-
	dead_band.dwData            = dead_zone;
	
	// finally set the property
	lpdijoy->SetProperty(DIPROP_DEADZONE,&dead_band.diph);
	
	dead_band.diph.dwSize       = sizeof(dead_band);
	dead_band.diph.dwHeaderSize = sizeof(dead_band.diph);
	dead_band.diph.dwObj        = DIJOFS_Y;
	dead_band.diph.dwHow        = DIPH_BYOFFSET;
	
	// deadband will be used on both sides of the range +/-
	dead_band.dwData            = dead_zone;
	
	
	// finally set the property
	lpdijoy->SetProperty(DIPROP_DEADZONE,&dead_band.diph);
	
	// acquire the joystick
	if (lpdijoy->Acquire()!=DI_OK)
		return(0);
	
	// set found flag
	joystick_found = 1;
	
	
	
	return 1;
}

int HCInput::DInput_Init_Mouse(HWND main_window_handle)
{
	
	// create a mouse device 
	if (lpdi->CreateDevice(GUID_SysMouse, &lpdimouse, NULL)!=DI_OK)
		return(0);
	
	// set cooperation level
	// change to EXCLUSIVE FORGROUND for better control
	if (lpdimouse->SetCooperativeLevel(main_window_handle, 
		DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)!=DI_OK)
		return(0);
	
	// set data format
	if (lpdimouse->SetDataFormat(&c_dfDIMouse)!=DI_OK)
		return(0);
	
	// acquire the mouse
	if (lpdimouse->Acquire()!=DI_OK)
		return(0);
	
	return 1;
}

int HCInput::DInput_Init_Keyboard(HWND main_window_handle)
{
	
	// create the keyboard device  
	if (lpdi->CreateDevice(GUID_SysKeyboard, &lpdikey, NULL)!=DI_OK)
		return(0);
	
	// set cooperation level
	if (lpdikey->SetCooperativeLevel(main_window_handle, 
		DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)!=DI_OK)
		return(0);
	
	// set data format
	if (lpdikey->SetDataFormat(&c_dfDIKeyboard)!=DI_OK)
		return(0);
	
	// acquire the keyboard
	if (lpdikey->Acquire()!=DI_OK)
		return(0);
	
	return 1;
}

void HCInput::DInput_Release_Joystick()
{
	if(lpdijoy)
	{
		lpdijoy->Unacquire();
		lpdijoy->Release();
	}
	
}

void HCInput::DInput_Release_Keyboard()
{
	
	if (lpdikey)
    {
		lpdikey->Unacquire();
		lpdikey->Release();
    } // end if
}

void HCInput::DInput_Release_Mouse()
{
	if(lpdimouse)
	{
		lpdimouse->Unacquire();
		lpdimouse->Release();
	}
}

int HCInput::DInput_Read_Joystick()
{
	
	// make sure the joystick was initialized
	if (!joystick_found)
		return(0);
	
	if (lpdijoy)
	{
		// this is needed for joysticks only    
		if (lpdijoy->Poll()!=DI_OK)
			return(0);
		
		if (lpdijoy->GetDeviceState(sizeof(DIJOYSTATE), (LPVOID)&joy_state)!=DI_OK)
			return(0);
	}
	else
	{
		// joystick isn't plugged in, zero out state
		memset(&joy_state,0,sizeof(joy_state));
		
		// return error
		return(0);
	} // end else
	return 1;
}

int HCInput::DInput_Read_Keyboard()
{
	
	
	// this function reads the state of the keyboard
	
	if (lpdikey)
    {
		if (lpdikey->GetDeviceState(256, (LPVOID)keyboard_state)!=DI_OK)
			return(0);
    }
	else
    {
		// keyboard isn't plugged in, zero out state
		memset(keyboard_state,0,sizeof(keyboard_state));
		
		// return error
		return(0);
    } // end else
	
	return 1;
}

int HCInput::DInput_Read_Mouse()
{
	// this function reads  the mouse state
	
	if (lpdimouse)    
	{
		if (lpdimouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouse_state)!=DI_OK)
			return(0);
	}
	else
	{
		// mouse isn't plugged in, zero out state
		memset(&mouse_state,0,sizeof(mouse_state));
		
		// return error
		return(0);
	} // end else
	
	return 1;
}




