#pragma once
#include "TerminalCompositeControl.h"
#include "ListViewDataProvider.h"

class TerminalListView;
using TerminalListViewChangedItemsCountCallback = std::function<void(const TerminalListView* listView, int curItemsCount, int prvItemsCount)>;
using TerminalListViewChangedOffsetCallback = std::function<void(const TerminalListView* listView, int curOffset, int prvOffset)>;

class TerminalListView : public TerminalCompositeControl {
    friend class TerminalVerticalScroll;
public:
    DECLARE_KIND(TerminalControl, TerminalControl::Kind::LIST_VIEW)
    DECLARE_CREATE(TerminalListView)

public:
    TerminalListView(TerminalCoord position, TerminalSize size);
    void AddItem(const std::string& value);
    bool RemoveLastItem();

    bool NeedScroll();
    bool HasUp();
    bool HasDown();

    int TotalItems() const;
    bool ChangeOffset(int delta);
    void FlushSelf() override;

protected:
    int MaxViewOffset();
    int NormalizeOffset(int offset);
protected:
    ListViewDataProvider provider;
    int viewOffset = 0;

public:
    void AddChangeItemsCallback(TerminalListViewChangedItemsCountCallback changeItemsCountCallback);
    void AddChangeOffsetCallback(TerminalListViewChangedOffsetCallback changeOffsetCallback);
protected:
    void OnChangeItemsCount(int curItemsCount, int prvItemsCount);
    void OnChangeOffset(int curOffset, int prvOffset);

    std::vector<TerminalListViewChangedItemsCountCallback> changeItemsCountCallbacks;
    std::vector<TerminalListViewChangedOffsetCallback> changeOffsetCallbacks;
};