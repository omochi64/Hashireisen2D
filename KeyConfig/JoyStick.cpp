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
//�R���X�g���N�^
//==========================================================================

JoyStick::JoyStick()
{
	memset(this,0,sizeof(JoyStick));
}

//==========================================================================
//�f�X�g���N�^
//==========================================================================

JoyStick::~JoyStick()
{
	UnInit();
}

//==========================================================================
//����������֐�
//==========================================================================
HRESULT JoyStick::Init(HINSTANCE hInst,HWND hwnd,int iAllowX,int iAllowY)
{
	HRESULT hr;
	m_dwJoyMaxCount = joyGetNumDevs();
	m_iAllowX = iAllowX;
	m_iAllowY = iAllowY;

	//==========================================================================
	//�����Ńf�o�C�X�̃��Z�b�g���s���Ă��܂��B
	//==========================================================================
	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_lpInput, NULL );
	if( FAILED(hr) ) 
	{
		return hr;
	}

	//==========================================================================
	//�����Ńf�o�C�X�̗񋓂��s���Ă��܂��B
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
	//�ȍ~�e�{�^���̏�������f�o�C�X�̊��蓖�ĂȂǂ��s���܂��B
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

		memset(&m_JoyButton[i],0,sizeof(JoyButton));//�{�^�������ׂ� 0 �ɂ��܂��B
	}
	return S_OK;
}


//==========================================================================
//�I������֐�
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
//�f�o�C�X�̃��Z�b�g������R�[���o�b�N�֐�
//==========================================================================

BOOL CALLBACK JoyStick::EnusizeesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef )
{

	//==========================================================================
	//�f�o�C�X�̐ݒ�
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
	//�f�o�C�X�̐ݒ�
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
//�W���C�X�e�B�b�N��񋓂���R�[���o�b�N�֐�
//==========================================================================

BOOL CALLBACK JoyStick::EnumJoysticksCallback( const DIDEVICEINSTANCE* lpddi, LPVOID lpContext )
{
	
	//==========================================================================
	//�f�o�C�X�̊��蓖�Ă��Ȃ���`
	//==========================================================================
	JoyStick *joyStick=(JoyStick *)lpContext;	
	joyStick->m_DiDevCaps.dwSize = sizeof(DIDEVCAPS);
	joyStick->m_lpInput->CreateDevice(lpddi->guidInstance, &joyStick->m_lpDevice[joyStick->m_dwJoyCount],NULL);
	joyStick->m_lpDevice[joyStick->GetJoyCount()]->GetCapabilities( &joyStick->m_DiDevCaps );

	//==========================================================================
	//�W���C�X�e�B�b�N�����ő�ɂȂ��Ă��狭���I��STOP������
	//==========================================================================
	if(++(joyStick->m_dwJoyCount) == joyStick->m_dwJoyMaxCount)
	{
		return DIENUM_STOP;
	}
	
	//==========================================================================
	//�ő�ɂȂ��ĂȂ������̂ŗ񋓂��܂�����
	//==========================================================================
	return DIENUM_CONTINUE;
}


//==========================================================================
//�{�^�����擾����֐�
//==========================================================================

JoyButton& JoyStick::GetJoyButton(DWORD dwNum)
{
	if(!(0 <= dwNum && dwNum < m_dwJoyCount))
	{
		assert(!"DirectInput class = JoyStick �w�肵���f�o�C�X�͊m�ۂ���Ă��܂��� ");
	}
	return m_JoyButton[dwNum];
}

//==========================================================================
//�{�^�����X�V����֐�
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
		//�ȍ~�{�^���̍X�V
		//==========================================================================
        JoyButton &joyButton = m_JoyButton[i];
		memset(&joyButton,0,sizeof(JoyButton));//0�ɏ�����
		if(SUCCEEDED(hr))
		{
			//==========================================================================
			//�\���{�^�����X�V
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
			//�{�^�����X�V
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
			//�{�^���̍Ď擾
			//==========================================================================
			m_lpDevice[i]->Acquire();
		}
	}
	return S_OK;
}

//==========================================================================
//�W���C�X�e�b�N�������m�ۂ���Ă��邩�擾���܂��B
//==========================================================================
DWORD JoyStick::GetJoyCount()
{
	return m_dwJoyCount;
}



