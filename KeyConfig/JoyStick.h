#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <dinput.h>
#include <windows.h>


const int BUTTON_NUM = 32;
const int INPUT_DEVICE_NUM = 16;


//=============================================================
//�W���C�X�e�B�b�N�̌��݂̏�Ԃ��m�邱�Ƃ��ł���{�^���ł��B
//=============================================================
struct JoyButton
{
	bool button[BUTTON_NUM];
	bool up;
	bool down;
	bool left;
	bool right;
};


//=============================================================
//�W���C�X�e�b�N���Ǘ�����N���X�ł��B
//=============================================================
class JoyStick
{
public:
	JoyStick();
	~JoyStick();
private:
	LPDIRECTINPUT8			m_lpInput;
	LPDIRECTINPUTDEVICE8	m_lpDevice[INPUT_DEVICE_NUM];
	DIDEVCAPS				m_DiDevCaps;
	DWORD m_dwJoyCount;
	DWORD m_dwJoyMaxCount;
	JoyButton *m_JoyButton;
	int m_iAllowX;		//X���W�̗V��	0�`1000�̒l�ł��B�Ⴂ�قǕq��
	int m_iAllowY;		//Y���W�̗V��	0�`1000�̒l�ł��B�Ⴂ�قǕq��

	//=============================================================
	//�R�[���o�b�N�֐��ł��B����ŃW���C�X�e�B�b�N�̏�Ԃ��擾�ł��܂��B
	//=============================================================
	static BOOL CALLBACK EnusizeesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef );
	static BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* lpddi, LPVOID lpContext );
public:

	//=============================================================
	//���[�U�[�������̃��\�b�h�𑀍삷�邱�Ƃő���ł��܂��B
	//=============================================================
	HRESULT Init(HINSTANCE hInst,HWND hwnd,int iAllowX = 1000,int iAllowY = 1000);	
	HRESULT UnInit();
	DWORD GetJoyCount();//�W���C�X�e�b�N�������m�ۂ���Ă��邩�擾���܂��B
	HRESULT UpData();//���̃��\�b�h��Joy���X�V�B���t���[���Ă�ł��������B
	JoyButton& GetJoyButton(DWORD dwNum);//JoyBottan���擾����֐�
};











#endif