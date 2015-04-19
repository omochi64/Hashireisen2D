#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <dinput.h>
#include <windows.h>


const int BUTTON_NUM = 32;
const int INPUT_DEVICE_NUM = 16;


//=============================================================
//ジョイスティックの現在の状態が知ることができるボタンです。
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
//ジョイステックを管理するクラスです。
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
	int m_iAllowX;		//X座標の遊び	0～1000の値です。低いほど敏感
	int m_iAllowY;		//Y座標の遊び	0～1000の値です。低いほど敏感

	//=============================================================
	//コールバック関数です。これでジョイスティックの状態を取得できます。
	//=============================================================
	static BOOL CALLBACK EnusizeesCallback( LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID lpvRef );
	static BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* lpddi, LPVOID lpContext );
public:

	//=============================================================
	//ユーザーがこれらのメソッドを操作することで操作できます。
	//=============================================================
	HRESULT Init(HINSTANCE hInst,HWND hwnd,int iAllowX = 1000,int iAllowY = 1000);	
	HRESULT UnInit();
	DWORD GetJoyCount();//ジョイステックがいくつ確保されているか取得します。
	HRESULT UpData();//このメソッドでJoyを更新。毎フレーム呼んでください。
	JoyButton& GetJoyButton(DWORD dwNum);//JoyBottanを取得する関数
};











#endif