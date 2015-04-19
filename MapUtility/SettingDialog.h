
#pragma once

LRESULT CALLBACK SettingDialog( HWND hWnd , UINT msg , WPARAM wParam , LPARAM lParam );
void GetRouteFolder( PTSTR str );
void GetModelFolder( PTSTR str );
void SetRouteFolder( LPCTSTR str );
void SetModelFolder( LPCTSTR str );
