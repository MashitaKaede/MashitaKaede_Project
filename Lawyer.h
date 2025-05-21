#pragma once
#include "Survivor.h"

class Lawyer : public Survivor
{
private:
public:
	Lawyer();
	~Lawyer() override;
	void Init() override;
	void Term() override;
	void Update() override;
};

