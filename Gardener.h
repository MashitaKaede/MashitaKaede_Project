#pragma once
#include "Survivor.h"

class Gardener : public Survivor
{
private:
public:
	Gardener();
	~Gardener() override;
	void Init() override;
	void Term() override;
	void Update() override;	
};

