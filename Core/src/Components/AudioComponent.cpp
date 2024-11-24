// #include "Components/AudioComponent.h"
// #include "Managers/AudioManager.h"
//
//
// AudioComponent::AudioComponent(Entity* mEntity, const char* audioFilePath)
//     : Component(mEntity), sound(nullptr), channel(nullptr), isPlaying(false) {
//     this->audioFilePath = new char[std::strlen(audioFilePath) + 1];
//     std::strcpy(this->audioFilePath, audioFilePath);
// }
//
// AudioComponent::~AudioComponent() {
//     Stop();
//     if (sound) {
//         sound->release();
//     }
//     delete[] audioFilePath;
// }
//
//
// // void AudioComponent::LoadSound(const char* filePath) {
// //     if (sound) {
// //         sound->release();
// //     }
// //     AudioManager* audioManager = mEntity->GetAudioManager();
// //     if (audioManager) {
// //         sound = audioManager->LoadSound(filePath);
// //     } else {
// //         std::cerr << "AudioManager not available in entity!" << std::endl;
// //     }
// // }
// //
// //
// // void AudioComponent::Play() {
// //     if (!sound) {
// //         LoadSound(audioFilePath);
// //     }
// //     if (sound) {
// //         AudioManager* audioManager = mEntity->GetAudioManager();
// //         if (audioManager) {
// //             audioManager->GetSystem()->playSound(sound, nullptr, false, &channel);
// //             if (channel) {
// //                 channel->set3DAttributes(&mEntity->GetPosition(), nullptr);
// //                 isPlaying = true;
// //             }
// //         } else {
// //             std::cerr << "AudioManager not available in entity!" << std::endl;
// //         }
// //     }
// // }
//
// void AudioComponent::Stop() {
//     if (channel) {
//         channel->stop();
//         channel = nullptr;
//         isPlaying = false;
//     }
// }
//
// // void AudioComponent::Update() {
// //     if (channel && isPlaying) {
// //         channel->set3DAttributes(&mEntity->GetPosition(), nullptr);
// //     }
// // }
// //
// // void AudioComponent::SetPosition(const FMOD_VECTOR& position) {
// //     // mEntity->SetPosition(position); // Assuming the entity has this method
// //     if (channel) {
// //         channel->set3DAttributes(&position, nullptr);
// //     }
// // }
//
// bool AudioComponent::IsPlaying() const {
//     return isPlaying;
// }
//
// Component* AudioComponent::clone() const {
//     return new AudioComponent(*this);
// }
//
//
// // class AudioComponent : public Component
// // {
// // public:
// //     AudioComponent(Entity* entity, const char* audioFilePath);
// //
// //     virtual ~AudioComponent();
// //
// //     void LoadSound(const char* filePath);
// //     void Play();
// //     void Stop(); 
// //     void Update();
// //     void SetPosition(const FMOD_VECTOR& position);
// //     Component* clone() const override;
// //
// //     bool IsPlaying() const;
// //
// // private:
// //     char* audioFilePath;
// //     FMOD::Sound* sound;
// //     FMOD::Channel* channel;
// //     bool isPlaying;
// // };
//
//
//
// // #include "AudioComponent.h"
// // #include "AudioManager.h"
// // #include <iostream>
// // #include "Entity.h"
// //
// // AudioComponent::AudioComponent(Entity* entity, const char* filePath)
// //     : Component(entity), sound(nullptr), channel(nullptr), isPlaying(false), audioFilePath(nullptr)
// // {
// //     componentType = ComponentType::Audio;  // Adjust component type
// //     LoadSound(filePath);  // Load the sound file
// // }
// //
// //
// // AudioComponent::~AudioComponent()
// // {
// //     if (audioFilePath) {
// //         delete[] audioFilePath;  // Free the memory if it was allocated
// //     }
// //     if (sound) {
// //         sound->release();  // Release FMOD sound object
// //     }
// // }
// //
// // void AudioComponent::LoadSound(const char* filePath)
// // {
// //     // If a file path was previously assigned, free it
// //     if (audioFilePath) {
// //         delete[] audioFilePath;
// //     }
// //
// //     // Allocate memory for the new file path and copy it
// //     audioFilePath = new char[strlen(filePath) + 1];
// //     strcpy(audioFilePath, filePath);
// //
// //     if (sound) {
// //         sound->release();  // Release any previously loaded sound
// //     }
// //
// //     FMOD::System* system = AudioManager::getInstance()->getSystem();
// //     system->createSound(filePath, FMOD_DEFAULT, nullptr, &sound);
// // }
// //
// // void AudioComponent::PlaySound()
// // {
// //     if (sound) {
// //         FMOD::System* system = AudioManager::getInstance()->getSystem();
// //         system->playSound(sound, nullptr, false, &channel);
// //         isPlaying = true;
// //     }
// // }
// //
// // void AudioComponent::StopSound()
// // {
// //     if (channel) {
// //         channel->stop();
// //         isPlaying = false;
// //     }
// // }
// //
// // void AudioComponent::Update()
// // {
// //     if (channel) {
// //         channel->isPlaying(&isPlaying);  // Update isPlaying status
// //     }
// // }
// //
// // Component* AudioComponent::clone() const
// // {
// //     return new AudioComponent(entity, audioFilePath);  // Clone the component
// // }
// //
// // bool AudioComponent::IsPlaying() const
// // {
// //     return isPlaying;  // Return whether the sound is playing
// // }
