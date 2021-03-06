#ifndef __MODULEFADETOBLACK_H__
#define __MODULEFADETOBLACK_H__

#include "Module.h"

class ModuleFadeToBlack : public Module {
public:
	ModuleFadeToBlack(bool start_enabled = true);
	~ModuleFadeToBlack();

	bool Start() override;
	update_status Update() override;

public:
	void FadeToBlack(Module *module_on, Module *module_off = nullptr, float time = 1.0f);
	bool isFading() const;

private:
	Uint32 startTime = 0;
	Uint32 totalTime = 0;
	bool fadingIn = true;
	Module* moduleOn = nullptr;
	Module* moduleOff = nullptr;
};

#endif // __MODULEFADETOBLACK_H__