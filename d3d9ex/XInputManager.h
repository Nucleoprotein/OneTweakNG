#pragma once
class XInputManager
{
	float* vibration_address_high_frequency = NULL;
	float* vibration_address_low_frequency = NULL;
	DWORD controllerId = -1;
	std::thread xinputThread;
public:
	XInputManager(byte** base_controller_input_address_ptr);
	void Run(byte** base_controller_input_address_ptr);
	void WaitAndSetVibrationAddress(byte** base_controller_input_address_ptr);
	void VibrationLoop();
	void SetControllerVibration(const WORD& leftMotorVibration, const WORD& rightMotorVibration);
};

