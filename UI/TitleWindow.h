﻿#ifndef atm_UI_TitleMenu_h
#define atm_UI_TitleMenu_h
#include "ist/iui.h"

namespace atm {

class StartWindow;
class RecordWindow;
class ConfigWindow;

class TitleWindow : public iui::Panel
{
typedef iui::Panel super;
friend class RootWindow;
private:
    TitleWindow();
public:
    void draw() override;
    void setVisibility(bool v) override;
    void unselectAll();
    bool onCancel(const iui::WM_Widget &wm) override;

private:
    void onStart(Widget *);
    void onRecord(Widget *);
    void onConfig(Widget *);
    void onExit(Widget *);

    StartWindow     *m_start;
    RecordWindow    *m_record;
    ConfigWindow    *m_config;

    iui::ToggleButton *m_buttons[4];
};

} // namespace atm
#endif // atm_UI_TitleMenu_h
