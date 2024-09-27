class System {

public:
    System() = default;

    virtual ~System() = default;

    virtual void Initialize(EntityManager* entityManager) = 0;

    virtual void Update(float deltaTime) = 0;

    virtual void FixedUpdate(float fixedDeltaTime) = 0;

    virtual void SetUpdateInterval(float interval) = 0;

