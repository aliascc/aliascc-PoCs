
/********************
*   Game Includes   *
*********************/
#include "Camera.h"

/********************
*   Function Defs   *
*********************/
Camera::Camera()
{
}

void Camera::Initialize(const XMFLOAT3& position, const XMFLOAT3& target, const XMFLOAT3& UP, const XMUINT2& dimensions, float fov, float znear, float zfar)
{
    m_Position      = position;
    m_Target        = target;
    m_UPVector      = UP;
    m_ZNear         = znear;
    m_ZFar          = zfar;
    m_FoV           = fov;
    m_Dimensions    = dimensions;

    //////////////////////////////////////////
    //Create View Matrix
    CreateView();

    //////////////////////////////////////////
    //Create Projection Matrix
    CreateProjection();
}

Camera::~Camera()
{
}

void Camera::CreateView()
{
    CreateLookAtLH(m_Position, m_Target, m_UPVector, m_ViewMatrix);
}

void Camera::CreateProjection()
{
    CreatePrespectiveFovLH(XMConvertToRadians(m_FoV), (float)m_Dimensions.x / (float)m_Dimensions.y, m_ZNear, m_ZFar, m_ProjectionMatrix);
}
