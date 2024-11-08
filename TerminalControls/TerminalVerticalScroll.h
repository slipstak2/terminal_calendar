#pragma once

#include "TerminalControl.h"

class TerminalVerticalScroll : public TerminalControl {
    friend class TerminalVerticalScrollBar;
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::VERTICAL_SCROLL)
    DECLARE_CREATE(TerminalVerticalScroll)

    TerminalVerticalScroll(TerminalListViewPtr listView, TerminalCoord position, TerminalSize size);

    template<class ...Args>
    TerminalCell CreateScrollCell(Args... args) {
        TerminalCell cell = CreateCell(args...);
        cell.SetFormatSettings(&scrollFormatSettings);
        return cell;
    }
    
    bool IsDraggable() override;
    bool TryDraggingStart(TerminalCoord absPosition) override;
    bool TryDragging(TerminalCoord delta) override;
    bool TryDraggingStop() override;
protected:
    bool isScrollDragging = false;

protected:
    int ScrollHeight();
    int OffsetHeight();
    int ItemsPerCell();

public:
    void FlushSelf() override;
protected:
    TerminalListViewPtr listView;
    FormatSettings scrollFormatSettings = FormatSettings::ScrollDefault;
};