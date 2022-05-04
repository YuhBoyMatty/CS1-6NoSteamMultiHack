#include "BHOP.h"
#include "Menu.h"
#include <Windows.h>

BHOP::BHOP()
	: m_bEnabled(false)
{
	uintptr_t hlAddr = (uintptr_t)GetModuleHandleA("hl.exe");

	m_pbIsInFlor = (bool*)(hlAddr + 0x1B70A54);
	m_pJmpCmd = (int*)(hlAddr + 0x6272A8);

	Section* pSecBhop = misc->AddSection("Bunny Hop", new bool(false));

	pSecBhop->AddButtom("Enabled", &m_bEnabled);
}

BHOP * BHOP::getInstance()
{
	if (m_BHOPInstance == nullptr)
	{
		m_BHOPInstance = new BHOP();
	}

	return m_BHOPInstance;
}

void BHOP::Update()
{
	if (m_bEnabled)
	{
		if (GetAsyncKeyState(VK_SPACE))
			DoBHOP();
	}
}

void BHOP::DoBHOP()
{
	if (*m_pbIsInFlor == 1)
	{
		*m_pJmpCmd = 5;
	}
	else {
		*m_pJmpCmd = 4;
	}
}
