#pragma once
#include "Survivor.h"

class Offense : public Survivor
{
private:
public:
	Offense();
	~Offense() override;
	void Init() override;
	void Term() override;
	void Update() override;
};

