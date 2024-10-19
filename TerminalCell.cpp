﻿#include "TerminalCell.h"
#include "TerminalControl.h"

void TerminalCell::SetParent(TerminalControl* newParent) {
    parent = newParent;
}

const FormatSettings& TerminalCell::GetFormatSettings() const {
    if (formatSettings) {
        return *formatSettings;
    }
    if (parent) {
        return parent->GetFormatSettings();
    }
    return FormatSettings::Default;
}

void TerminalCell::SetFormatSettings(const FormatSettings* newFormatSettings) {
    formatSettings = newFormatSettings;
}

const TerminalControl* TerminalCell::GetParent() const {
    return parent;
}

TerminalControl* TerminalCell::GetParent() {
    return parent;
}

bool TerminalCell::operator == (const TerminalCell& rhs) {
    bool isEqual =  
        std::tie(data, parent, formatSettings) == 
        std::tie(rhs.data, rhs.parent, rhs.formatSettings);
    if (!isEqual) {
        return false;
    }
    if (formatSettings && rhs.formatSettings) {
        return *formatSettings == *rhs.formatSettings;
    }
    return false;
}

bool TerminalCell::operator != (const TerminalCell& rhs) {
    return !(*this == rhs);
}

// https://habr.com/ru/articles/119436/
// 
// 
//   SetConsoleTextAttribute(hConsole,
// FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_BLUE);
// 
// https://habr.com/ru/articles/119436/
// https://habr.com/ru/companies/macloud/articles/558316/
void TerminalCell::Render() const {
    auto fmtSettings = GetFormatSettings();
    printf("\033[%d;%d;%dm%s\033[0m", 
        (int)(fmtSettings.textStyle), 
        int(fmtSettings.backgroundColor), 
        (int)fmtSettings.fontColor, 
        data.get()
    );

    // \033[38;2;<r>;<g>;<b>m           #Select RGB foreground color
    // \033[48; 2; <r>; <g>; <b>m       #Select RGB background color
    //printf("\033[38;2;201;200;59\033[48;2;201;100;59m%s", data.get());

    //printf("\033[5;%d;%dm%s\033[0m", BackgroundColor::Red, FontColor::Yellow, data.get());
}

TerminalCell::TerminalCell(const Rune& rune) 
    : data(rune) 
{}

TerminalCell::TerminalCell(const Rune& rune, const FormatSettings* formatSettings)
    : data(rune)
    , formatSettings(formatSettings)
{}


TerminalCell::TerminalCell(const TerminalCell& other)
    : data(other.data)
    , parent(other.parent)
    , formatSettings(other.formatSettings)
{}