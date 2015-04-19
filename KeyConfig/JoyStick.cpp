#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#include "JoyStick.h"
#include <assert.h>
#include <stdio.h>


#ifndef RELEASE
#define RELEASE(x)	{if(x){(x)->Release();x=NULL;}}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	{if(x){delete []x; x = NULL;}}
#endif
//==========================================================================
//コンストラクタ
//==========================================================================

JoyStick::JoyStick()
{
	memset(this,0,sizeof(JoyStick));
}

//==========================================================================
//デストラクタ
//==========================================================================

JoyStick::~JoyStick()
{
	UnInit();
}

//==========================================================================
//初期化する関数
//==========================================================================
HRESULT JoyStick::Init(HINSTANCE hInst,HWND hwnd,int iAllowX,int iAllowY)
{
	HRESULT hr;
	m_dwJoyMaxCount = joyGetNumDevs();
	m_iAllowX = iAllowX;
	m_iAllowY = iAllowY;

	//==========================================================================
	//ここでデバイスのリセットを行っています。
	//==========================================================================
	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_lpInput, NULL );
	if( FAILED(hr) ) 
	{
		return hr;
	}

	//==========================================================================
	//ここでデバイスの列挙を行っています。
	//==========================================================================
	hr = m_lpInput->EnumDevices( DI8DEVCLASS_GAMECTRL, JoyStick::EnumJoysticksCallback,(void *)this, DIEDFL_ATTACHEDONLY );
	if( FAILED(hr))
	{
		return hr;
	}
	DIPROPDWORD diprop;
	diprop.diph.dwSize	= sizeof(diprop); 
	diprop.diph.dwHeaderSize	= sizeof(diprop.diph); 
	diprop.diph.dwObj	= 0;
	diprop.diph.dwHow	= DIPH_DEVICE;
	diprop.dwData		= DIPROPAXISMODE_ABS;
	
	//==========================================================================
	//以降各ボタンの初期化やデバイスの割り当てなどを行います。
	//==========================================================================
	m_JoyButton = new JoyButton[m_dwJoyCount];
 	for(DWORD i = 0; i < m_dwJoyCount; i++)
	{
		hr = m_lpDevice[i]->SetDataFormat( &c_dfDIJoystick );
		if( FAILED(hr) )
		{
			return hr;
		}
		hr = m_lpDevice[i]->SetCooperativeLevel( hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
		if( FAILED(hr) )
		{
			return hr;
		}
		hr = m_lpDevice[i]->EnumObjects(JoyStick::EnusizeesCallback, (void *)m_lpDevice[i], DIDFT_AXIS );
		if( FAILED(hr) )
		{
			return hr;
		}
		hr = m_lpDevice[i]->SetProperty( DIPROP_AXISMODE, &diprop.diph );
		if( FAILED(hr) )
		{
			return hr;
		}
		m_lpDevice[i]->Acquire();

		memset(&m_JoyButton[i],0,sizeof(JoyButton));//ボタンをすべて 0 にします。
	}
	return S_OK;
}


//==========================================================================
//終了する関数
//==========================================================================
HRESULT JoyStick::UnInit()
{
	for(DWORD i = 0; i < m_dwJoyCount; i++)
	{
		if( m_lpDevice[i])
		{
			m_lpDevice[i]->Unacquire(); 
		}
		RELEASE(m_lpDevice[i]);
	}
	SAFE_DELETE_ARRAY(m_JoyButton);
	RELEASE(m_lpInput);

	return S_OK;
}

//==========================================================================
//デバイスのリセットをするコールバック関数
//==========================================================================

BOOL CALLBACK JoyStick::EnusizeesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef )
{

	//==========================================================================
	//デバイスの設定
	//==========================================================================
	DIPROPRANGE diprg;
	ZeroMemory(&diprg,sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg); 
	diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
	diprg.diph.dwObj = lpddoi->dwType;
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = -1000;
	diprg.lMax = +1000;

	//==========================================================================
	//デバイスの設定
	//==========================================================================
	LPDIRECTINPUTDEVICE8 lpJoy = (LPDIRECTINPUTDEVICE8)lpvRef;
	HRESULT hr;
	hr = lpJoy->SetProperty(DIPROP_RANGE, &diprg.diph);
	if(FAILED(hr))
	{
		return DIENUM_STOP;
	}
	return DIENUM_CONTINUE;
}

//==========================================================================
//ジョイスティックを列挙するコールバック関数
//==========================================================================

BOOL CALLBACK JoyStick::EnumJoysticksCallback( const DIDEVICEINSTANCE* lpddi, LPVOID lpContext )
{
	
	//==========================================================================
	//デバイスの割り当てやらなんやら～
	//==========================================================================
	JoyStick *joyStick=(JoyStick *)lpContext;	
	joyStick->m_DiDevCaps.dwSize = sizeof(DIDEVCAPS);
	joyStick->m_lpInput->CreateDevice(lpddi->guidInstance, &joyStick->m_lpDevice[joyStick->m_dwJoyCount],NULL);
	joyStick->m_lpDevice[joyStick->GetJoyCount()]->GetCapabilities( &joyStick->m_DiDevCaps );

	//==========================================================================
	//ジョイスティック数が最大になってたら強制的にSTOPさせる
	//==========================================================================
	if(++(joyStick->m_dwJoyCount) == joyStick->m_dwJoyMaxCount)
	{
		return DIENUM_STOP;
	}
	
	//==========================================================================
	//最大になってなかったので列挙がまだ続く
	//==========================================================================
	return DIENUM_CONTINUE;
}


//==========================================================================
//ボタンを取得する関数
//==========================================================================

JoyButton& JoyStick::GetJoyButton(DWORD dwNum)
{
	if(!(0 <= dwNum && dwNum < m_dwJoyCount))
	{
		assert(!"DirectInput class = JoyStick 指定したデバイスは確保されていません ");
	}
	return m_JoyButton[dwNum];
}

//==========================================================================
//ボタンを更新する関数
//==========================================================================

HRESULT JoyStick::UpData()
{
	HRESULT	hr;
	DIJOYSTATE dijs;
	for(DWORD i = 0; i < m_dwJoyCount; i++)
	{
		hr = m_lpDevice[i]->Poll();
		if(FAILED(hr))
		{
			m_lpDevice[i]->Acquire();
			return hr;
		}
		hr = m_lpDevice[i]->GetDeviceState(sizeof(DIJOYSTATE),&dijs );
		
		//==========================================================================
		//以降ボタンの更新
		//==========================================================================
        JoyButton &joyButton = m_JoyButton[i];
		memset(&joyButton,0,sizeof(JoyButton));//0に初期化
		if(SUCCEEDED(hr))
		{
			//==========================================================================
			//十字ボタンを更新
			//==========================================================================
			if(dijs.lY <= -m_iAllowY)
			{
				joyButton.up = 1;
			}
			if(dijs.lY >= m_iAllowY)
			{
				joyButton.down = 1;
			}
			if(dijs.lX >= m_iAllowX)
			{
				joyButton.right = 1;
			}
			if(dijs.lX <= -m_iAllowX)
			{
				joyButton.left = 1;
			}
		
			//==========================================================================
			//ボタンを更新
			//==========================================================================
			for(int j = 0; j < BUTTON_NUM; j++)
			{
				if(dijs.rgbButtons[j] & 0x80)	
				{
					joyButton.button[j] = 1;
				}
			}
		}
		else if(hr == DIERR_INPUTLOST )
		{
			
			//==========================================================================
			//ボタンの再取得
			//==========================================================================
			m_lpDevice[i]->Acquire();
		}
	}
	return S_OK;
}

//==========================================================================
//ジョイステックがいくつ確保されているか取得します。
//==========================================================================
DWORD JoyStick::GetJoyCount()
{
	return m_dwJoyCount;
}



