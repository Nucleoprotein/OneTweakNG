#pragma once
class XInputManager
{
	float* vibration_address_high_frequency = NULL;
	float* vibration_address_low_frequency = NULL;
	DWORD controllerId = -1;
	std::thread xinputThread;
public:
	XInputManager(uint8_t** base_controller_input_address_ptr);
	void Run(uint8_t** base_controller_input_address_ptr);
	void WaitAndSetVibrationAddress(uint8_t** base_controller_input_address_ptr);
	void VibrationLoop();
	void SetControllerVibration(const WORD& leftMotorVibration, const WORD& rightMotorVibration);
};

