#include "imguiTools.h"
#include <IconsForkAwesome.h>
#include "imgui_internal.h"




bool ImGui::colouredButton(const char *label, glm::vec4 color, const ImVec2 &size_arg)
{
	::ImGui::PushStyleColor(ImGuiCol_Button, {color.r, color.g, color.b, color.a});
	auto rez = ::ImGui::Button(label, size_arg);
	::ImGui::PopStyleColor();
	return rez;
}

void ImGui::addErrorSymbol()
{
	::ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
	::ImGui::Text(ICON_FK_TIMES_CIRCLE " ");
	::ImGui::PopStyleColor();
}

void ImGui::addWarningSymbol()
{
	::ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
	::ImGui::Text(ICON_FK_EXCLAMATION_TRIANGLE " ");
	::ImGui::PopStyleColor();
}

void ImGui::helpMarker(const char *desc)
{
	::ImGui::TextDisabled("(?)");
	if (::ImGui::IsItemHovered())
	{
		::ImGui::BeginTooltip();
		::ImGui::PushTextWrapPos(::ImGui::GetFontSize() * 35.0f);
		::ImGui::TextUnformatted(desc);
		::ImGui::PopTextWrapPos();
		::ImGui::EndTooltip();
	}
}

void ImGui::removeFocusToCurrentWindow()
{
	auto *g = ::ImGui::GetCurrentContext();
	if (g)
	{
		g->NavWindow = 0;
	}
}

bool ImGui::redButton(const char *label, const ImVec2 &size_arg)
{
	return colouredButton(label, {1,0,0,1}, size_arg);
}

bool ImGui::greenButton(const char *label, const ImVec2 &size_arg)
{
	return colouredButton(label, {0,1,0,1}, size_arg);
}

void addColorButton(const char *id, const ImVec4 &col, float outCol[4])
{
	if (ImGui::ColorButton(id, col))
	{
		outCol[0] = col.x;
		outCol[1] = col.y;
		outCol[2] = col.z;
		outCol[3] = col.w;
	}
}

bool ImGui::ColorEdit4Swatches(const char *label, float col[4], ImGuiColorEditFlags flags)
{
	bool rez = ::ImGui::ColorEdit4(label, col);

	::ImGui::BeginGroup();
	::ImGui::PushID(label);

	if (::ImGui::BeginPopup("picker"))
	{
		addColorButton("0", {0,0,0,1}, col); ImGui::SameLine();
		addColorButton("1", {1,0,0,1}, col); ImGui::SameLine();
		addColorButton("2", {0,1,0,1}, col); ImGui::SameLine();
		addColorButton("3", {0,0,1,1}, col); ImGui::SameLine();
		addColorButton("4", {1,1,0,1}, col); ImGui::SameLine();
		addColorButton("5", {1,0,1,1}, col); ImGui::SameLine();
		addColorButton("6", {0,1,1,1}, col); ImGui::SameLine();
		addColorButton("7", {1,1,1,1}, col);

		::ImGui::EndPopup();
	}

	::ImGui::PopID();
	::ImGui::EndGroup();

	return rez;
}

bool ImGui::BeginChildFrameColoured(ImGuiID id, glm::vec4 color, const ImVec2 &size, ImGuiWindowFlags extra_flags)
{
	ImGuiContext &g = *::ImGui::GetCurrentContext();
	const ImGuiStyle &style = ::ImGui::GetStyle();
	::ImGui::PushStyleColor(ImGuiCol_ChildBg, {color.x,color.y,color.z,color.w});
	::ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, style.FrameRounding);
	::ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, style.FrameBorderSize);
	::ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, style.FramePadding);
	bool ret = ::ImGui::BeginChild(id, size, true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysUseWindowPadding | extra_flags);
	::ImGui::PopStyleVar(3);
	::ImGui::PopStyleColor();
	return ret;
}

void ImGui::PushMakeWindowNotTransparent(float opacity)
{
	ImGuiStyle &style = ::ImGui::GetStyle();
	auto background = style.Colors[ImGuiCol_WindowBg];
	background.w = opacity;

	ImGui::PushStyleColor(ImGuiCol_WindowBg, background);
}

void ImGui::PopMakeWindowNotTransparent()
{
	ImGui::PopStyleColor();
}

bool ImGui::blueButton(const char *label, const ImVec2 &size_arg)
{
	return colouredButton(label, {0,0,1,1}, size_arg);
}
