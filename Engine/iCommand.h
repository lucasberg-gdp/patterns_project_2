#pragma once
// All commands have this interface

// This would benefit form being part of an Abstract Factory pattern.
// Because all the commands are "a type of iCommand"

class iCommand
{
public:
	enum class MoveType {
		Linear,
		EasyIn,
		EasyOut,
		EasyInAndOut
	};

	// Pure virtual objects require a virtual destructor (because C++)
	virtual ~iCommand() {};
	virtual void Start(void) = 0;
	virtual void Update(double deltaTime) = 0;
	virtual bool isFinished(void) = 0;
	virtual void OnFinished(void) = 0;
};
