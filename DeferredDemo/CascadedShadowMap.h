#pragma once

#include <D3DX11.h>
#include <xnamath.h>
#include <vector>

static const int MAXCASCADECNT = 8;

class Camera;
class AABB;


struct CSMConfig
{
	UINT mCascadesCnt;      //Number of cascades
	UINT mCascadeResolution;   //Shadow map resolution for each cascade
	float mSceneCoveragePct;   // Percentage of the whole view( in z direction ) to apply CSM
	float mSubFrustumCoveragePct[MAXCASCADECNT];   //Percentage of coverage( in z direction ) for each cascades
}; 

class CascadedShadowMap
{
public:
	CascadedShadowMap(ID3D11Device *, ID3D11DeviceContext * , CSMConfig * , Camera *viewCamera, Camera * lightCamera);
	~CascadedShadowMap();

	//Init
	void Init();

	//Partition View Frustrum
	void BuildCascadeViewProjs(AABB * aabb);
	void BindShadowMapDSV();
	ID3D11ShaderResourceView * DepthShaderResourceView();

private:
	
	//Used for ComputeTightNearFar function
	struct Triangle
	{
		XMVECTOR pt[3];
		BOOL culled;
	};

	//Compute tight near and far planes for each orthographic projection by insecting subfrustum AABB with scene AABB in light camera space
	void ComputeTightNearFar(float& fNearPlane, float& fFarPlane, const AABB &subFrustumAABB, const std::vector<XMVECTOR> &sceneAABBCorners) const;

	//Configuration of CSM
	CSMConfig * m_pConfig;

	//Scene view camera and light view camera
	Camera * m_pViewCamera;
	Camera * m_pLightCamera;
	
	//AABB of the scene 
	AABB * m_pSceneAABB;

	//Light camera orthographics projection matrices for each cascade
	XMMATRIX mCascadesOrthoProj[MAXCASCADECNT];
	XMMATRIX mCascadesView;
	D3D11_VIEWPORT mCascadesVP[MAXCASCADECNT];

	ID3D11Device *m_pDevice;
	ID3D11DeviceContext * m_pContext;

	ID3D11Texture2D *m_pShadowCascadesTex;
	ID3D11DepthStencilView * m_pCSMDSV;
	ID3D11ShaderResourceView * m_pCSMSRV;
};

