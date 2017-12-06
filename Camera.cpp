#include "Camera.h"

Camera::Camera()
{
}
Camera::~Camera()
{
}
//When we need to move camera through axys(X,Y) we use this method
void Camera::Move(XMFLOAT2& rPos)
{
	//:WARNING: this adjust position to % of screen
	//possibly need future changes
	rPos.x /= 50;
	rPos.y /= 50;

	mPosition.x += rPos.x;
	mPosition.y += rPos.y;
	
}
//To draw using this camera perspective we need to pass this MATRIX to
//DrawSprite.Begin method, it sets position, rotation and scale
XMMATRIX Camera::GetTransformation()
{
	//Set the rotation and scale matrix
	XMMATRIX rotation = XMMatrixRotationZ(mRotation);
	XMMATRIX scale = XMMatrixScaling(mZoom,mZoom,1);

	//Set view matrix
	mCameraPosition = XMVectorSet(mPosition.x, mPosition.y, -1.0f, 0.0f);
	mCameraUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX camView = XMMatrixLookAtLH(mCameraPosition, Target(), mCameraUp);

	//return the transformation
	if (mRotation!=0)
		return camView*scale*rotation;

	return camView*scale;
}
//This return a vector that contain the target of the camera coordinates
XMVECTOR Camera::Target()
{
	return XMVectorSet(mPosition.x, mPosition.y, 0.0f, 0.0f);
}
