#pragma once

#include <cstdint>

class BHOP
{
	inline static BHOP* m_BHOPInstance = nullptr;
	bool* m_pbIsInFlor;
	int* m_pJmpCmd;
	bool m_bEnabled;
public:
	BHOP();
	static BHOP* getInstance();

	void Update();
	void DoBHOP();
};

