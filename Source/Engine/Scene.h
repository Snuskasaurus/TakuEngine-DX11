#pragma once

enum EGameSceneType
{
    None,
    TakumiScene,
};

class CGameScene
{
    friend class MWorld;

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
    void Destroy() { OnDestroy(); }
};
