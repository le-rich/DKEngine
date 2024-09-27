class System {


public:
    virtual ~System() = default;

    virtual void Update(float deltaTime) = 0;

    virtual void FixedUpdate(float fixedDeltaTime) = 0;

    virtual void AddEntity(int entityId) = 0;

    virtual void RemoveEntity(int entityId) = 0;
};