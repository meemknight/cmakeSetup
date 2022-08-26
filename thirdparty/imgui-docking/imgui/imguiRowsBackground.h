#pragma once
#include <imgui.h>
#include <cstdint>
#include <cctype>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>

//https://github.com/ocornut/imgui/issues/2668

namespace ImGui
{

    void ItemRowsBackground(float lineHeight = -1.0f, const ImColor &color = ImColor(20, 20, 20, 64))
    {
        auto *drawList = ImGui::GetWindowDrawList();
        const auto &style = ImGui::GetStyle();

        if (lineHeight < 0)
        {
            lineHeight = ImGui::GetTextLineHeight();
        }
        lineHeight += style.ItemSpacing.y;

        float scrollOffsetH = ImGui::GetScrollX();
        float scrollOffsetV = ImGui::GetScrollY();
        float scrolledOutLines = floorf(scrollOffsetV / lineHeight);
        scrollOffsetV -= lineHeight * scrolledOutLines;

        ImVec2 clipRectMin(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
        ImVec2 clipRectMax(clipRectMin.x + ImGui::GetWindowWidth(), clipRectMin.y + ImGui::GetWindowHeight());

        if (ImGui::GetScrollMaxX() > 0)
        {
            clipRectMax.y -= style.ScrollbarSize;
        }

        drawList->PushClipRect(clipRectMin, clipRectMax);

        bool isOdd = (static_cast<int>(scrolledOutLines) % 2) == 0;

        float yMin = clipRectMin.y - scrollOffsetV + ImGui::GetCursorPosY();
        float yMax = clipRectMax.y - scrollOffsetV + lineHeight;
        float xMin = clipRectMin.x + scrollOffsetH + ImGui::GetWindowContentRegionMin().x;
        float xMax = clipRectMin.x + scrollOffsetH + ImGui::GetWindowContentRegionMax().x;

        for (float y = yMin; y < yMax; y += lineHeight, isOdd = !isOdd)
        {
            if (isOdd)
            {
                drawList->AddRectFilled({xMin, y - style.ItemSpacing.y}, {xMax, y + lineHeight}, color);
            }
        }

        drawList->PopClipRect();
    }

}

// Please do not forget this!
//ImGui::Begin("Example Bug");
//ItemRowsBackground();
// whatever code that draws list or tree items
//ImGui::End();
