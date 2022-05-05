#include "SpeedHack.h"

SpeedHack* SpeedHack::getInstance(Section* pSecMisc)
{
    if (m_Instance == nullptr)
        m_Instance = new SpeedHack(pSecMisc);

    return m_Instance;
}

SpeedHack::SpeedHack(Section* pSecMisc)
{
    Section* pSecSpeedHack = pSecMisc->AddSection("Speedhack", new bool(false));

    pSecSpeedHack->AddButtom("Enabled", &m_bEnabled);
    pSecSpeedHack->AddSlider("Velocity", &m_Velocity, 1.f, 100.f);

    m_GameVelocity = (float*)((uintptr_t)GetModuleHandleA("hl.exe") + 0xACA1C8);

}

void SpeedHack::Update()
{
    if (m_bEnabled)
    {
        *m_GameVelocity = max(m_Velocity / 5.f, 1.f);
    }
    else {
        *m_GameVelocity = 1.f;
    }
}
