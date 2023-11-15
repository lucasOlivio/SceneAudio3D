#include "EngineMedia/MediaPlayer.h"
#include "components/Audio.h"
#include "components/Channel.h"
#include "components/Transform.h"

MediaPlayer* MediaPlayer::m_pInstance = nullptr;

MediaPlayer::MediaPlayer()
{
    this->m_pAudioManager = new AudioManager();

    // Singleton
    assert(!this->m_pInstance);
}

MediaPlayer::~MediaPlayer()
{
    delete this->m_pAudioManager;
}

MediaPlayer* MediaPlayer::Get()
{
    if (MediaPlayer::m_pInstance == nullptr)
    {
        MediaPlayer::m_pInstance = new MediaPlayer();
    }
    return MediaPlayer::m_pInstance;
}

bool MediaPlayer::Initialize(std::string baseAudiosPath, SceneView* pSceneView)
{
    this->m_pSceneView = pSceneView;

    bool isAudioInit = this->m_pAudioManager->Initialize(this->m_pSceneView->GetNumComponents("channel"));
    if (!isAudioInit)
    {
        return false;
    }

    bool isChannelsLoaded = this->LoadChannels();
    if (!isChannelsLoaded)
    {
        return false;
    }

    this->m_pAudioManager->SetBasePath(baseAudiosPath);

    bool isAudiosLoaded = this->LoadAudios();
    if (!isAudiosLoaded)
    {
        return false;
    }

    return true;
}

bool MediaPlayer::LoadChannels()
{
    this->m_pAudioManager->ClearAudio();

    for (this->m_pSceneView->First("channel"); !this->m_pSceneView->IsDone(); this->m_pSceneView->Next())
    {
        ChannelComponent* pChannel = this->m_pSceneView->CurrentValue<ChannelComponent>();

        bool audioLoaded = this->m_pAudioManager->LoadChannel(pChannel->GetChannel());

        if (!audioLoaded)
        {
            return false;
        }
    }

    return true;
}

bool MediaPlayer::LoadAudios()
{
    this->m_pAudioManager->ClearAudio();

    for (this->m_pSceneView->First("audio"); !this->m_pSceneView->IsDone(); this->m_pSceneView->Next())
    {
        AudioComponent* pAudio = this->m_pSceneView->CurrentValue<AudioComponent>();
        FMOD::Sound* pSound = nullptr;

        bool audioLoaded = this->m_pAudioManager->LoadAudio(pAudio->GetFileName().c_str(), 
                                                            pAudio->IsStream(),
                                                            pAudio->IsLoop(),
                                                            pAudio->GetChannelGroup(),
                                                            &pSound);

        if (!audioLoaded)
        {
            return false;
        }

        pAudio->SetSound(pSound);
    }

    return true;
}

void MediaPlayer::Destroy()
{
    this->m_pAudioManager->Destroy();
    delete this->m_pAudioManager;
}

void MediaPlayer::Update(double deltaTime)
{
    this->m_pAudioManager->Update();
}

void MediaPlayer::SetListenerAttributes(const glm::vec3& position, const glm::vec3& velocity, 
                                        const glm::vec3& forward, const glm::vec3& up)
{
    this->m_pAudioManager->SetListenerAttributes(position, velocity, forward, up);
}

void MediaPlayer::PlayAudio(EntityID entityId, glm::vec3 soundVelocity)
{
    AudioComponent* pAudio = this->m_pSceneView->GetComponent<AudioComponent>(entityId, "audio");
    TransformComponent* pTransform = this->m_pSceneView->GetComponent<TransformComponent>(entityId, "transform");

    int channelId = this->m_pAudioManager->PlayAudio(pAudio->GetFileName().c_str(),
                                     pAudio->GetChannelGroup(),
                                     pTransform->GetPosition(),
                                     soundVelocity,
                                     pAudio->GetMinDistance(),
                                     pAudio->GetMaxDistance());

    pAudio->SetChannelId(channelId);
}

void MediaPlayer::StopAudio(EntityID entityId)
{
    AudioComponent* pAudio = this->m_pSceneView->GetComponent<AudioComponent>(entityId, "audio");

    this->m_pAudioManager->StopChannel(pAudio->GetChannelId());
}

void MediaPlayer::PauseAudio(EntityID entityId, bool value)
{
    AudioComponent* pAudio = this->m_pSceneView->GetComponent<AudioComponent>(entityId, "audio");

    this->m_pAudioManager->SetPaused(pAudio->GetChannelId(), value);
}

bool MediaPlayer::SetPitch(EntityID entityId, float value)
{
    AudioComponent* pAudio = this->m_pSceneView->GetComponent<AudioComponent>(entityId, "audio");

    return this->m_pAudioManager->SetChannelPitch(pAudio->GetChannelId(), value);
}

bool MediaPlayer::IsPlaying(EntityID entityId)
{
    AudioComponent* pAudio = this->m_pSceneView->GetComponent<AudioComponent>(entityId, "audio");

    return this->m_pAudioManager->IsChannelPlaying(pAudio->GetChannelId());
}

bool MediaPlayer::IsFinished(EntityID entityId)
{
    AudioComponent* pAudio = this->m_pSceneView->GetComponent<AudioComponent>(entityId, "audio");
    int channelId = pAudio->GetChannelId();

    bool isPlaying = this->m_pAudioManager->IsChannelPlaying(channelId);
    bool isPaused = this->m_pAudioManager->IsChannelPaused(channelId);

    if (!isPlaying && !isPaused)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MediaPlayer::SetReverberation(EntityID entityId, float decay, float density, float diffusion)
{
    AudioComponent* pAudio = this->m_pSceneView->GetComponent<AudioComponent>(entityId, "audio");
    int channelId = pAudio->GetChannelId();

    this->m_pAudioManager->AddReverbFilterOnChannel(channelId);
    this->m_pAudioManager->SetReverbValuesOnChannel(channelId, decay, density, diffusion);
}

void MediaPlayer::SetDistortion(EntityID entityId, float level)
{
    AudioComponent* pAudio = this->m_pSceneView->GetComponent<AudioComponent>(entityId, "audio");
    int channelId = pAudio->GetChannelId();

    this->m_pAudioManager->AddDistortionFilterOnChannel(channelId);
    this->m_pAudioManager->SetDistortionLevelFilterValuesOnChannel(channelId, level);
}

void MediaPlayer::SetHighpass(EntityID entityId)
{
    AudioComponent* pAudio = this->m_pSceneView->GetComponent<AudioComponent>(entityId, "audio");
    int channelId = pAudio->GetChannelId();

    this->m_pAudioManager->AddHighPassFilterOnChannel(channelId);
}

void MediaPlayer::SetLowpass(EntityID entityId)
{
    AudioComponent* pAudio = this->m_pSceneView->GetComponent<AudioComponent>(entityId, "audio");
    int channelId = pAudio->GetChannelId();

    this->m_pAudioManager->AddLowPassFilterOnChannel(channelId);
}
