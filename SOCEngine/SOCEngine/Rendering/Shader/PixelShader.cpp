#include "PixelShader.h"
#include "Director.h"

namespace Rendering
{
	using namespace Shader;

	PixelShader::PixelShader(ID3DBlob* blob) : BaseShader(blob), _shader(nullptr)
	{
		_type = Type::Pixel;
	}

	PixelShader::~PixelShader(void)
	{
		SAFE_RELEASE(_shader);
	}

	bool PixelShader::CreateShader()
	{
		if(_blob == nullptr)
			return false;

		ID3D11Device* device = Device::Director::GetInstance()->GetDirectX()->GetDevice();

		HRESULT hr = device->CreatePixelShader( _blob->GetBufferPointer(), _blob->GetBufferSize(), nullptr, &_shader);
		_blob->Release();

		if( FAILED( hr ) )
			return false;

		return true;
	}

	void PixelShader::UpdateShader(ID3D11DeviceContext* context, const std::vector<BufferType>* constBuffers, const std::vector<TextureType>* textures, const std::vector<SamplerType>& samplers)
	{
		context->PSSetShader(_shader, nullptr, 0);

		if(constBuffers)
		{
			for(auto iter = constBuffers->begin(); iter != constBuffers->end(); ++iter)
			{
				ID3D11Buffer* buffer = (*iter).second->GetBuffer();
				context->PSSetConstantBuffers( (*iter).first, 1, &buffer );
			}
		}

		if(textures)
		{
			for(auto iter = textures->begin(); iter != textures->end(); ++iter)
			{
				ID3D11ShaderResourceView* srv = iter->second->GetShaderResourceView();
				context->PSSetShaderResources( iter->first, 1, &srv );
			}
		}

		for(auto iter = samplers.begin(); iter != samplers.end(); ++iter)
		{
			ID3D11SamplerState* samplerState = iter->second->GetSampler();
			context->PSSetSamplers(iter->first, 1, &samplerState);
		}
	}

	void PixelShader::ClearResource(ID3D11DeviceContext* context, const std::vector<TextureType>* textures)
	{
		if(textures)
		{
			ID3D11ShaderResourceView* nullSrv = nullptr;

			for(auto iter = textures->begin(); iter != textures->end(); ++iter)
				context->PSSetShaderResources( iter->first, 1, &nullSrv );
		}
	}
}