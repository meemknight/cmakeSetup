#pragma once
#include "imgui.h"
#include <glm/vec4.hpp>

namespace ImGui
{


	bool colouredButton(const char *label, glm::vec4 color,
		const ImVec2 &size_arg);

	void addErrorSymbol();

	void addWarningSymbol();

	void helpMarker(const char *desc);

	void removeFocusToCurrentWindow();

	bool redButton(const char *label, const ImVec2 &size_arg);

	bool greenButton(const char *label, const ImVec2 &size_arg);
	
	bool blueButton(const char *label, const ImVec2 &size_arg);

	bool ColorEdit4Swatches(const char *label, float col[4], ImGuiColorEditFlags flags);

	bool BeginChildFrameColoured(ImGuiID id,
		glm::vec4 color, const ImVec2 &size, ImGuiWindowFlags extra_flags);

	void PushMakeWindowNotTransparent(float opacity = 1);

	void PopMakeWindowNotTransparent();


};
