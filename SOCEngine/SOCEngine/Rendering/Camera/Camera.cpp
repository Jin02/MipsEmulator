#include "Camera.h"
#include "Object.h"
#include "Director.h"
#include "TransformPipelineParam.h"

using namespace Math;
using namespace std;
using namespace Rendering::Light;
using namespace Device;
using namespace Core;
using namespace Rendering::Camera;

Camera::Camera() : Component(),
	_frustum(nullptr), _renderTarget(nullptr), _depthBuffer(nullptr)
{

}

Camera::~Camera(void)
{

}

void Camera::Initialize()
{
	_FOV = 60.0f;
	_clippingNear = 0.01f;
	_clippingFar = 100.0f;

	Size<unsigned int> windowSize = Director::GetInstance()->GetWindowSize();
	_aspect = (float)windowSize.w / (float)windowSize.h;

	_camType    = Type::Perspective;
	_clearColor = Color(0.5f, 0.5f, 1.0f);

	_frustum = new Frustum(0.0f);		

	_renderTarget = new Texture::RenderTexture;
	_renderTarget->Create(windowSize);

	//_clearFlag = ClearFlag::FlagSolidColor;
}

void Camera::Destroy()
{
	SAFE_DELETE(_frustum);
	SAFE_DELETE(_renderTarget);
}

void Camera::CalcAspect()
{
	Size<unsigned int> windowSize =  Device::Director::GetInstance()->GetWindowSize();
	_aspect = (float)windowSize.w / (float)windowSize.h;
}

void Camera::ProjectionMatrix(Math::Matrix& outMatrix)
{
	if(_camType == Type::Perspective)
	{
		float radian = _FOV * PI / 180.0f;
		Matrix::PerspectiveFovLH(outMatrix, _aspect, radian, _clippingNear, _clippingFar);
	}
	else if(_camType == Type::Orthographic)
	{
		Size<unsigned int> windowSize = Device::Director::GetInstance()->GetWindowSize();
		Matrix::OrthoLH(outMatrix, (float)(windowSize.w), (float)(windowSize.h), _clippingNear, _clippingFar);
	}
}

void Camera::ViewMatrix(Math::Matrix& outMatrix)
{
	Transform* ownerTransform = _owner->GetTransform();
	ownerTransform->WorldMatrix(outMatrix);

	Vector3 worldPos;
	worldPos.x = outMatrix._41;
	worldPos.y = outMatrix._42;
	worldPos.z = outMatrix._43;

	Vector3 p;
	p.x = -Vector3::Dot(ownerTransform->GetRight(), worldPos);
	p.y = -Vector3::Dot(ownerTransform->GetUp(), worldPos);
	p.z = -Vector3::Dot(ownerTransform->GetForward(), worldPos);

	outMatrix._41 = p.x;
	outMatrix._42 = p.y;
	outMatrix._43 = p.z;
	outMatrix._44 = 1.0f;
}

void Camera::UpdateTransformAndCheckRender(const Structure::Vector<std::string, Core::Object>& objects)
{
	TransformPipelineParam tfParam;
	ProjectionMatrix(tfParam.projMat);
	ViewMatrix(tfParam.viewMat);

	Matrix viewProj = tfParam.viewMat * tfParam.projMat;
	_frustum->Make(viewProj);

	auto& dataInobjects = objects.GetVector();
	for(auto iter = dataInobjects.begin(); iter != dataInobjects.end(); ++iter)
	{				
		GET_CONTENT_FROM_ITERATOR(iter)->Culling(_frustum);
		GET_CONTENT_FROM_ITERATOR(iter)->UpdateTransformAndCheckRender(tfParam);
	}
}

void Camera::RenderObjects(const Device::DirectX* dx, const Rendering::Manager::RenderManager* renderMgr)
{
	auto MeshRender = [&](ID3D11DeviceContext* context, Material* customMaterial, Manager::RenderManager::MeshType type)
	{
		Material* currentUseMaterial = nullptr;
		auto RenderIter = [&](Material* material, Mesh::Mesh* mesh)
		{
			if(customMaterial)
				material = customMaterial;

			if(currentUseMaterial != material)
			{
				currentUseMaterial = material;	
				currentUseMaterial->UpdateShader(context);
			}

			mesh->Render(customMaterial);
			return;
		};

		renderMgr->Iterate(RenderIter, type);
	};

	//graphics part
	{
		ID3D11DeviceContext* context = dx->GetContext();

		//depth clear
		{
			context->ClearRenderTargetView(dx->GetBackBuffer(), _clearColor.color);			
			dx->GetDepthBuffer()->Clear(1.0f, 0);
			//_renderTarget->Clear(_clearColor, dx);
			//_depthBuffer->Clear(1.0f, 0, dx);
		}

		//off alpha blending
		{
			//float blendFactor[1] = { 0.0f };
			//context->OMSetBlendState(dx->GetOpaqueBlendState(), blendFactor, 0xffffffff);
		}

		//Render
		{
			//Early-Z
			{
				ID3D11RenderTargetView* rtv = dx->GetBackBuffer();
				context->OMSetRenderTargets(1, &rtv, dx->GetDepthBuffer()->GetDepthStencilView());
			/*_depthBuffer->SetRenderTarget(dx);
				context->OMSetDepthStencilState(dx->GetDepthLessEqualState(), 0);
				NonAlphaMeshRender();

				context->RSSetState(dx->GetDisableCullingRasterizerState());
				AlphaMeshRender();
				context->RSSetState(nullptr);*/
			}

			//Light Culling
			{

			}

			//Forward Rendering
			{
				MeshRender(context, nullptr, Manager::RenderManager::MeshType::nonAlpha);
			}
		}


		IDXGISwapChain* swapChain = dx->GetSwapChain();
		swapChain->Present(0, 0);
	}
}
