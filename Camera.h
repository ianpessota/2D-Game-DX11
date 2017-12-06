/**********************************************************************
Ian Pessota
Class Camera
Basic camera for 2D projects
***********************************************************************/
#ifndef _CAMERA_H_
#define _CAMERA_H_

//INCLUDES
#include<d3d11.h> 
#include<d3dx11.h>
#include<xnamath.h> 

class Camera
{
public:
	Camera();
	~Camera();
	//Get and Set ZOOM values
	float GetZoom() const { return mZoom; };
	void SetZoom(float zoom) { mZoom = zoom; };

	//Get and Set Rotation values
	float GetRotation() const { return mRotation; }
	void SetRotation(float radians) { mRotation = radians; };

	//Get and Set Position values
	XMFLOAT2 GetPosition() const { return mPosition; };
	void SetPosition(XMFLOAT2& pos) { mPosition = pos; };

	//Move the camera along xy axis
	void Move(XMFLOAT2&);

	//return the camera transformation(position, rotation, scale)
	XMMATRIX GetTransformation();
	
protected:
	float mZoom = 1.0f;
	float mRotation = 0;
	XMFLOAT2 mPosition= NULL;
	XMVECTOR mCameraUp;
	XMVECTOR mCameraPosition;
	XMVECTOR Target();
};

#endif
