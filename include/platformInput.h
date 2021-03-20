#pragma once


namespace platform 
{
	struct Button
	{
		char pressed = 0;
		char held = 0;
		char released = 0;

		enum
		{
			A = 0,
			B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			NR0, NR1, NR2, NR3, NR4, NR5, NR6, NR7, NR8, NR9,
			Space,
			Enter,
			Escape,
			Up,
			Down,
			Left,
			Right,
			BUTTONS_COUNT, //
		};

		static constexpr int buttonValues[BUTTONS_COUNT] =
		{
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
			'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			VK_SPACE, VK_RETURN, VK_ESCAPE, VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT
		};

		void merge(const Button &b)
		{
			this->pressed |= b.pressed;
			this->released |= b.released;
			this->held |= b.held;
		}
	};

	inline void processEventButton(Button &b, bool newState)
	{

		if (newState)
		{
			if (b.held)
			{
				b.pressed = false;
			}
			else
			{
				b.pressed = true;
			}

			b.held = true;
			b.released = false;
		}
		else
		{
			b.held = false;
			b.pressed = false;
			b.released = true;
		}


	}


	//Button::key
	int isKeyHeld(int key);

	int isKeyPressedOn(int key);

	int isKeyReleased(int key);


};