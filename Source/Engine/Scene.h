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

protected:
    virtual void OnCreate() {}
    virtual void OnUpdate(const float& _dt) {}
    virtual void OnDestroy() {}
    virtual void OnKeyPressed(EKeyCode _key);

private:
    void Create();
    void Update(const float& _dt) { OnUpdate(_dt); }
    void Destroy();
    void NotifyKeyPressed(EKeyCode _keyCode) { OnKeyPressed(_keyCode); }
    
public:
    CDrawable_InstancedMesh* AddInstancedMeshToDraw(const char* _meshName);
    CDrawable_InstancedMesh* AddInstancedMeshToDrawFromDrawableData(const char* _meshName);

protected:
    CSceneLight SceneLight;
    
private:
    std::vector<CDrawable_InstancedMesh*> InstancedMeshes;
};
