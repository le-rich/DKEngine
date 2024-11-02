#ifndef AUDIO_SYS_H

class AudioSystem {
public:
    AudioSystem(AudioManager& audioManager);

    void initializeSound(AudioComponent& audioComp);
    void playSound(AudioComponent& audioComp);
    void stopSound(AudioComponent& audioComp);

    void update3DAudio(AudioComponent& audioComp, const FMOD_VECTOR& newPosition, const FMOD_VECTOR& newVelocity);
    void updateListener(const ListenerComponent& listenerComp);

    void update();
    void addEntity(Entity entity);

private:
    AudioManager& audioManager;
    std::vector<Entity> entities;
};

FMOD_VECTOR calculateVelocity(const FMOD_VECTOR& oldPosition, const FMOD_VECTOR& newPosition, float deltaTime);

#endif