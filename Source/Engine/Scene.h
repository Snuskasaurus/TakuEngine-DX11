#pragma once

#include "IncludesExternal.h"
#include "Inputmanager.h"
#include "Sun.h"
#include "Graphics/Drawable.h"

enum EGameSceneType
{
    None,
    GridScene,
    GameTerrainScene,
};

constexpr EGameSceneType GC_DEFAULT_SCENE = EGameSceneType::GameTerrainScene;

class CGameScene
{
    friend class MWorld;
    friend class MGraphic;
    friend class MInput;

public:
    const CSceneLight& GetSceneLight() const { return SceneLight; }
    
public:
    static void ChangeGameScene(const EGameSceneType& _gameSceneType);
    
protected:
    virtual ~CGameScene() = default;

private:
    void OnCreate_Internal();
    void OnUpdate_Internal(const float& _dt);
    void OnDestroy_Internal();
    void OnKeyPressed_Internal(EKeyCode _key);
    void OnKeyReleased_Internal(EKeyCode _key);
    
protected:
    virtual void OnEvent_Create() {}
    virtual void OnEvent_Update(const float& _dt) {}
    virtual void OnEvent_Destroy() {}
    virtual void OnEvent_KeyPressed(EKeyCode _key) {}
    virtual void OnEvent_KeyReleased(EKeyCode _key) {}

private:
    void Create();
    void UpdateScene(const float& _dt);
    void Destroy();

public:
    CDrawable_InstancedMesh* AddInstancedMeshToDraw_DEPRECATED(const char* _meshName);
    CDrawable_InstancedMesh* AddInstancedMeshToDrawFromDrawableData(const char* _meshName);

protected:
    CSceneLight SceneLight;
    
private:
    std::vector<CDrawable_InstancedMesh*> InstancedMeshes;
};
