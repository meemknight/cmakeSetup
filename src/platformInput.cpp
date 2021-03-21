#include "platformInput.h"

platform::Button keyBoard[platform::Button::BUTTONS_COUNT];
platform::Button leftMouse;
platform::Button rightMouse;

int platform::isKeyHeld(int key)
{
	if (key < Button::A || key >= Button::BUTTONS_COUNT) { return 0; }

	return keyBoard[key].held;
}

int platform::isKeyPressedOn(int key)
{
	if (key < Button::A || key >= Button::BUTTONS_COUNT) { return 0; }

	return keyBoard[key].pressed;
}

int platform::isKeyReleased(int key)
{
	if (key < Button::A || key >= Button::BUTTONS_COUNT) { return 0; }

	return keyBoard[key].released;
}

int platform::isLMousePressed()
{
	return leftMouse.pressed;
}

int platform::isRMousePressed()
{
	return rightMouse.pressed;
}

int platform::isLMouseReleased()
{
	return leftMouse.released;
}

int platform::isRMouseReleased()
{
	return rightMouse.released;
}


int platform::isLMouseHeld()
{
	return leftMouse.held;
}

int platform::isRMouseHeld()
{
	return rightMouse.held;
}

void platform::internal::setButtonState(int button, int newState)
{

	processEventButton(keyBoard[button], newState);

}

void platform::internal::setLeftMouseState(int newState)
{
	processEventButton(leftMouse, newState);

}

void platform::internal::setRightMouseState(int newState)
{
	processEventButton(rightMouse, newState);

}

void platform::internal::resetButtonToZero(Button &b)
{
	b.pressed = 0;
	b.held = 0;
	b.released = 0;

}

void platform::internal::updateAllButtons()
{
	for (int i = 0; i < platform::Button::BUTTONS_COUNT; i++)
	{
		updateButton(keyBoard[i]);
	}

	updateButton(leftMouse);
	updateButton(rightMouse);


}

void platform::internal::resetInputsToZero()
{

	for (int i = 0; i < platform::Button::BUTTONS_COUNT; i++)
	{
		resetButtonToZero(keyBoard[i]);
	}

	resetButtonToZero(leftMouse);
	resetButtonToZero(rightMouse);
	

}
