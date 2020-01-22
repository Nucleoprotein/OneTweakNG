#include "stdafx.h"
#include "XInputManager.h"
#include <XInput.h>

XInputManager::XInputManager(byte** base_controller_input_address_ptr)
{
	xinputThread = std::thread(&XInputManager::Run, this, base_controller_input_address_ptr);
}

void XInputManager::Run(byte** base_controller_input_address_ptr)
{
	DWORD controllerState;
	bool hasConnected = false;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		controllerState = XInputGetState(i, &state);
		if (controllerState == ERROR_SUCCESS) {
			controllerId = i;
			hasConnected = true;
			break;
		}
	}
	if (hasConnected) {
		WaitAndSetVibrationAddress(base_controller_input_address_ptr);
		VibrationLoop();
	}
}

void XInputManager::WaitAndSetVibrationAddress(byte** base_controller_input_address_ptr)
{
	do {
		std::this_thread::sleep_for(std::chrono::milliseconds(4));
		if (base_controller_input_address_ptr && *base_controller_input_address_ptr) {
			vibration_address_low_frequency = (float*)(*base_controller_input_address_ptr + 0x40 + 0x60 - 0x4);
			vibration_address_high_frequency = vibration_address_low_frequency + 1;
		}
	} while (vibration_address_low_frequency == NULL);
}

void XInputManager::VibrationLoop()
{
	const WORD maxVibrationStrength = 65535;
	bool wasVibrating = false;
	SetControllerVibration(0, 0);
	while (true) {
		const float vibrationStrengthLowFrequency = *vibration_address_low_frequency;
		const float vibrationStrengthHighFrequency = *vibration_address_high_frequency;
		if (vibrationStrengthLowFrequency || vibrationStrengthHighFrequency) {
			SetControllerVibration(vibrationStrengthLowFrequency * maxVibrationStrength, vibrationStrengthHighFrequency * maxVibrationStrength);
			wasVibrating = true;
		}
		else if(wasVibrating) {
			SetControllerVibration(0, 0);
			wasVibrating = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(4));
	}
}

void XInputManager::SetControllerVibration(const WORD& leftMotorVibration, const WORD& rightMotorVibration)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = leftMotorVibration;
	vibration.wRightMotorSpeed = rightMotorVibration;
	XInputSetState(controllerId, &vibration);
}
