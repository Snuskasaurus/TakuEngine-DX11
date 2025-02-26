#pragma once

#include "IncludesExternal.h"
#include "Inputmanager.h"
#include "Sun.h"
#include "Graphics/Drawable.h"

enum EGameSceneType
{
    None,
    TakumiScene,
    GridScene,
    TestPBRScene,
};

class CGameScene
{
    friend class MWorld;
    friend class MGraphic;
    friend class MInput;

public:
    const CSceneLight* GetSceneLight() const { return &SceneLight; }
    
public:
    static void ChangeGameScene(const EGameSceneType& _gameSceneType);
    
protected:
    virtual ~CGameScene() = default;

private:
    void OnCreate_Internal();
    void OnUpdate_Internal(const float& _dt);
    void OnDestroy_Internal();
    void OnKeyPressed_Internal(EKeyCode _key);
    
protected:
    virtual void OnCreate() = 0;
    virtual void OnUpdate(const float& _dt) = 0;
    virtual void OnDestroy() = 0;
    virtual void OnKeyPressed(EKeyCode _key) = 0;

private:
    void Create();
    void Update(const float& _dt);
    void Destroy();
    void NotifyKeyPressed(EKeyCode _keyCode);

public:
    CDrawable_InstancedMesh* AddInstancedMeshToDraw(const char* _meshName);
    CDrawable_InstancedMesh* AddInstancedMeshToDrawFromDrawableData(const char* _meshName);

protected:
    CSceneLight SceneLight;
    
private:
    std::vector<CDrawable_InstancedMesh*> InstancedMeshes;
};
