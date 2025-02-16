#pragma once

#include "IncludesExternal.h"
#include "Graphics/Drawable.h"

enum EGameSceneType
{
    None,
    TakumiScene,
};

class CGameScene
{
    friend class MWorld;
    friend class MGraphic;

public:
    static void ChangeGameScene(const EGameSceneType& _gameSceneType);
    
protected:
    ~CGameScene() = default;
    virtual void OnCreate() {};
    virtual void OnUpdate(const float& _dt) {};
    virtual void OnDestroy() {};
    
private:
    void Create() { OnCreate(); }
    void Update(const float& _dt) { OnUpdate(_dt); }
    void Destroy();

protected:
    CDrawable_InstancedMesh* AddInstancedMeshToDraw(const TTransform& _transform, const char* _meshName);
    
private:
    std::vector<CDrawable_InstancedMesh*> InstancedMeshes;
};
