#pragma once

#include "utils/common.h"
#include "TerminalRectangle.h"
#include "TerminalCell.h"
#include "TextFormat/FormatSettings.h"
#include <functional>

#define DECLARE_KIND(base, kind)                                    \
    static const TerminalControl::Kind KIND = kind;                 \
    TerminalControl::Kind GetKind() const override { return KIND; } \
    bool IsKindOf(TerminalControl::Kind _kind) const override { return _kind != KIND ? base::IsKindOf(_kind) : true; }

using ClickCallback = std::function<bool()>;

class TerminalControl : public TerminalRectangle {
    friend class TerminalCanvas;
public:
    enum class Kind {
        CONTROL,
        COMPOSITE_CONTROL,
        LABEL_BASE,
        LABEL,
        LABEL_DATA_PROVIDER,
        LABEL_SWITCHER,
        BUTTON,
        RADIO_BUTTON,
        CHECK_BOX,
        LIST_VIEW,
        BORDER_LIST_VIEW,
        GROUP_BOX,
        BORDER_CONTROL,
        VERTICAL_SCROLL,
        VERTICAL_SCROLL_BAR,
        WINDOW
    };
    static const TerminalControl::Kind KIND = Kind::CONTROL;
    virtual Kind GetKind() const { return KIND; };
    virtual bool IsKindOf(Kind kind) const { return kind == KIND; }
public:
    TerminalControl(TerminalCoord position);
    TerminalControl(TerminalCoord position, TerminalSize size);
    TerminalControl(TerminalCoord position, TerminalSize size, FormatSettings formatSettings);
    virtual void AddControl(TerminalControlPtr control);

    void SetSize(TerminalSize newSize);
    void SetPosition(TerminalCoord newPosition);

    void SetParentWindow(TerminalWindow* parentWindow);

    template<class ...Args>
    TerminalCell CreateCell(Args... args) {
        TerminalCell cell(std::forward<Args>(args)...);
        cell.SetParent(this);
        return cell;
    }
    template<class ...Args>
    TerminalCell CreateBackgroundCell(Args... args) {
        TerminalCell cell(std::forward<Args>(args)...);
        cell.SetParent(this);
        cell.SetFormatSettings(&FormatSettings::Default);
        return cell;
    }
    const TerminalCell& Get(short row, short col);

    template<class T>
    const T* As() const {
        return this->IsKindOf(T::KIND) ? static_cast<const T*>(this) : nullptr;
    }

    template<class T>
    T* As() {
        return this->IsKindOf(T::KIND) ? static_cast<T*>(this) : nullptr;
    }

    void AddClickCallback(ClickCallback clickCallback) {
        clickCallbacks.push_back(std::move(clickCallback));
    }

    bool ApplyMouseLeftClick() {
        bool isApply = false;
        for (auto clickCallback : clickCallbacks) {
            isApply |= clickCallback();
        }
        return isApply;
    }
    TerminalWindow* GetParentWindow() {
        return parentWindow;
    }

    TerminalControl* GetParent() {
        return parent;
    }

    const FormatSettings& GetFormatSettings() {
        return formatSettings;
    }
    void SetFormatSettings(FormatSettings newFormatSettings) {
        formatSettings = newFormatSettings;
    }

    void SetVisible(bool isVisible) {
        this->isVisible = isVisible;
    }

    bool IsVisible() const {
        return isVisible;
    }

    std::vector<TerminalControlPtr>& GetControls();

public:
    void Flush();
    int tag = 0;
protected:
    void FlushControls();
    virtual void FlushSelf() = 0;

protected:
    TerminalControl* parent = nullptr;
    TerminalWindow* parentWindow = nullptr;
    std::vector<TerminalControlPtr> controls;
    std::vector<std::vector<TerminalCell>> data;
    TerminalCell backgroundCell = CreateBackgroundCell('~');

    std::vector<ClickCallback> clickCallbacks;

    FormatSettings formatSettings = FormatSettings::Default;
    bool isVisible = true;
};