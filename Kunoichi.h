#pragma once
#include "Survivor.h"

class Kunoichi : public Survivor
{
private:
public:
	Kunoichi();
	~Kunoichi() override;
	void Init() override;
	void Term() override;
	void Update() override;
};

