#include "ShaderBuffers.h"

#include "Graphic.h"
#include "../HResultHandler.h"
#include "../Sun.h"
#include "../World.h"
#include "../GameWindow.h"
#include "../FreeLookCamera.h"
#include "../Debug/DebugDraw.h"

//---------------------------------------------------------------------------------------------------------------------------------------------------------
void SShaderBufferHolder::CreateShaderBuffer(EShaderType _shaderType, UINT _slot, UINT _sizeStruct)
{
    this->ShaderType = _shaderType;
    this->Slot = _slot;
#if DEBUG_DO_CHECK_IN_SHADER_BUFFER
    this->SizeBuffer = _sizeStruct;
#endif
    
    
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0u;
    bufferDesc.StructureByteStride = _sizeStruct;
    bufferDesc.ByteWidth = _sizeStruct;
    
    ID3D11Device* Device = MGraphic::GetDXDevice();
    CHECK_HRESULT(Device->CreateBuffer(&bufferDesc, nullptr, &this->Buffer));
#ifdef DEBUG_ENABLE_DIRECT_DEVICE_DEBUG
    constexpr char name[] = "TAKU_SHADER_BUFFER";
    CHECK_HRESULT(this->Buffer->SetPrivateData(WKPDID_D3DDebugObjectName, ARRAYSIZE(name), name));
#endif
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void SShaderBufferHolder::Release()
{
    if (Buffer != nullptr)
        Buffer->Release();
    Buffer = nullptr;
}

// D3D11_MAPPED_SUBRESOURCE mappedResource;
// CHECK_HRESULT(_deviceContext->Map(*_objectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
// memcpy(mappedResource.pData, &buffer, sizeof(SVSConstantBuffer_Frame));
// _deviceContext->Unmap(*_objectBuffer, 0);
// _deviceContext->VSSetConstantBuffers(0u, 1u, _objectBuffer);

//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void CopyBufferDataToDeviceContext(UINT _slot, ID3D11Buffer** _buffer, const void* _bufferData, UINT _bufferDataSize, EShaderType _shaderType)
{
    ID3D11DeviceContext* deviceContext = MGraphic::GetDXDeviceContext();
    
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CHECK_HRESULT(deviceContext->Map(*_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
    memcpy(mappedResource.pData, _bufferData, _bufferDataSize);
    deviceContext->Unmap(*_buffer, 0);
    
    if (_shaderType == EShaderType::VERTEX_SHADER)
        deviceContext->VSSetConstantBuffers(_slot, 1u, _buffer);
    else if (_shaderType == EShaderType::PIXEL_SHADER)
        deviceContext->PSSetConstantBuffers(_slot, 1u, _buffer);
    else
        assert(false);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void SShaderBufferHolder::FillBuffer_VS_Object(SShaderBufferHolder* _shaderBufferHolder, const TTransform* _transforms, UINT _start, UINT _nbInstances)
{
    b01_vs_buffer_object BufferData = {};
#if DEBUG_DO_CHECK_IN_SHADER_BUFFER
    assert(_shaderBufferHolder != nullptr);
    assert(sizeof(BufferData) == _shaderBufferHolder->SizeBuffer);
#endif
    
    int iBufferStruct = 0;
    const UINT End = _start + _nbInstances;
    for (UINT iTransform = _start; iTransform < End ; ++iTransform)
    {
        const TMatrix4f objectWorldMatrix = TTransform::ToMatrix(_transforms[iTransform]);
        BufferData.world[iBufferStruct] = TMatrix4f::Transpose(objectWorldMatrix);
        iBufferStruct++;
    }

    CopyBufferDataToDeviceContext(_shaderBufferHolder->Slot, &_shaderBufferHolder->Buffer, &BufferData, sizeof(BufferData), _shaderBufferHolder->ShaderType);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void SShaderBufferHolder::FillBuffer_VS_SceneEachFrame(SShaderBufferHolder* _shaderBufferHolder, bool _isViewLight)
{
    b00_vs_buffer_sceneEachFrame BufferData = {};
#if DEBUG_DO_CHECK_IN_SHADER_BUFFER
    assert(_shaderBufferHolder != nullptr);
    assert(sizeof(BufferData) == _shaderBufferHolder->SizeBuffer);
#endif
    
    if (_isViewLight)
    {
        const CSceneLight& SceneLight = MWorld::GetWorld()->GetCurrentScene()->GetSceneLight();
        const TMatrix4f lightViewMatrix = SceneLight.GetViewMatrix();
        const TMatrix4f lightProjectionMatrix = SceneLight.GetProjectionMatrix();
        BufferData.cameraViewProjection = TMatrix4f::Transpose(lightViewMatrix * TMatrix4f::World * lightProjectionMatrix); 
    }
    else
    {
        const TMatrix4f cameraViewMatrix = MWorld::GetWorld()->FreeLookCamera.GetViewMatrix();
        const TMatrix4f cameraProjectionMatrix = MGameWindow::GetCameraProjectionMatrix();
    
        const CSceneLight& SceneLight = MWorld::GetWorld()->GetCurrentScene()->GetSceneLight();
        const TMatrix4f lightViewMatrix = SceneLight.GetViewMatrix();
        const TMatrix4f lightProjectionMatrix = SceneLight.GetProjectionMatrix();
        BufferData.cameraViewProjection = TMatrix4f::Transpose(cameraViewMatrix * TMatrix4f::World * cameraProjectionMatrix);
        BufferData.lightViewMatrix = TMatrix4f::Transpose(lightViewMatrix * TMatrix4f::World);
        BufferData.lightProjectionMatrix = TMatrix4f::Transpose(lightProjectionMatrix);
    }
    
    CopyBufferDataToDeviceContext(_shaderBufferHolder->Slot, &_shaderBufferHolder->Buffer, &BufferData, sizeof(BufferData), _shaderBufferHolder->ShaderType);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void SShaderBufferHolder::FillBuffer_VS_DebugLine(SShaderBufferHolder* _shaderBufferHolder, const SDebugLine* _debugLines, UINT _start, UINT _nbInstances)
{
    b13_vs_buffer_debug_draw_line BufferData = {};
#if DEBUG_DO_CHECK_IN_SHADER_BUFFER
    assert(_shaderBufferHolder != nullptr);
    assert(sizeof(BufferData) == _shaderBufferHolder->SizeBuffer);
#endif
    
    const TMatrix4f cameraViewMatrix = MWorld::GetWorld()->FreeLookCamera.GetViewMatrix();
    const TMatrix4f cameraProjectionMatrix = MGameWindow::GetCameraProjectionMatrix();
    
    int iBufferStruct = 0;
    const UINT End = _start + _nbInstances;

    for (UINT iTransform = _start; iTransform < End ; ++iTransform)
    {
        for (int i = 0; i < 2; ++i)
        {
            const TMatrix4f wvp = TTransform::ToMatrix(_debugLines[iTransform].Transforms[i]);
            BufferData.debugLines[iBufferStruct].wvp[i] = TMatrix4f::Transpose(wvp);
        }
        BufferData.debugLines[iBufferStruct].r = _debugLines[iBufferStruct].Color.r;
        BufferData.debugLines[iBufferStruct].g = _debugLines[iBufferStruct].Color.g;
        BufferData.debugLines[iBufferStruct].b = _debugLines[iBufferStruct].Color.b;
        
        iBufferStruct++;
    }

    CopyBufferDataToDeviceContext(_shaderBufferHolder->Slot, &_shaderBufferHolder->Buffer, &BufferData, sizeof(BufferData), _shaderBufferHolder->ShaderType);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void SShaderBufferHolder::FillBuffer_PS_SceneEachFrame(SShaderBufferHolder* _shaderBufferHolder)
{
    b00_ps_buffer_sceneEachFrame BufferData = {};
#if DEBUG_DO_CHECK_IN_SHADER_BUFFER
    assert(_shaderBufferHolder != nullptr);
    assert(sizeof(BufferData) == _shaderBufferHolder->SizeBuffer);
#endif
    
    const SLightInfo& lightInfo = MWorld::GetWorld()->GetCurrentScene()->GetSceneLight().GetSceneLightInfo();
    BufferData.camDir = MWorld::GetWorld()->FreeLookCamera.GetCameraWorldViewDir();
    BufferData.lightDir = lightInfo.Direction;
    BufferData.lightColor = { lightInfo.Color.r, lightInfo.Color.g, lightInfo.Color.b };
    BufferData.lightColorIntensity = lightInfo.ColorIntensity;
    BufferData.lightAmbientIntensity = lightInfo.AmbientIntensity;
    
#if DEBUG_DO_CHECK_IN_SHADER_BUFFER
    assert(sizeof(BufferData) == _shaderBufferHolder->SizeBuffer);
#endif
    CopyBufferDataToDeviceContext(_shaderBufferHolder->Slot, &_shaderBufferHolder->Buffer, &BufferData, sizeof(BufferData), _shaderBufferHolder->ShaderType);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
