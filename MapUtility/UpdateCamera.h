
#pragma once

namespace Camera
{
	extern	float	g_dist;
	extern	float	g_camRotationY;
	extern	float	g_camRotationXZ;
	extern	Math::Vector3D	g_camAt;
	extern	Math::Vector3D	g_camUp;
	extern Math::Vector3D g_camBasicDir;

	void UpdateCamera( Scene::ICamera *pCamera );
	void SetCameraToDefault( Scene::ICamera *pCamera );
	void SetCameraOnZAxisPlusDirection( Scene::ICamera *pCamera );
	void SetCameraOnYAxisPlusDirection( Scene::ICamera *pCamera );
	void SetCameraOnXAxisPlusDirection( Scene::ICamera *pCamera );
	void SetCameraOnZAxisMinusDirection( Scene::ICamera *pCamera );
	void SetCameraOnYAxisMinusDirection( Scene::ICamera *pCamera );
	void SetCameraOnXAxisMinusDirection( Scene::ICamera *pCamera );

};
