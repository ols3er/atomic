﻿#include "stdafx.h"
#include "Game/AtomicApplication.h"
#include "Game/AtomicGame.h"
#include "Util.h"
#include "ConfigWindow.h"
#include "RootWindow.h"
#include "UISelector.h"

namespace atm {

void EnumerateAvailableResolutiuons(const std::function<void (const uvec2 &)> &f)
{
    std::vector<uvec2> resolutions;
#ifdef WIN32
    ::DEVMODE dm;
    for(int i=0; ::EnumDisplaySettings(0, i, &dm); ++i) {
        if(dm.dmBitsPerPel>16) {
            resolutions.push_back( uvec2(dm.dmPelsWidth, dm.dmPelsHeight) );
        }
    }
    std::sort(resolutions.begin(), resolutions.end(), [](const uvec2 &a, const uvec2 &b){
        return a.x==b.x ? a.y<b.y : a.x<b.x;
    });
    resolutions.erase(std::unique(resolutions.begin(), resolutions.end()), resolutions.end());
#else // WIN32 
    resolutions.push_back(uvec2(640, 480));
    resolutions.push_back(uvec2(800, 600));
    resolutions.push_back(uvec2(1024, 768));
    resolutions.push_back(uvec2(1280, 800));
    resolutions.push_back(uvec2(1280, 960));
    resolutions.push_back(uvec2(1280, 1024));
#endif // WIN32 
    std::for_each(resolutions.begin(), resolutions.end(), f);
}

ConfigWindow::ConfigWindow()
{
    setVisibility(false);

    using std::placeholders::_1;
    float32 vspace = 30.0f;
    float32 hspace = 270;
    float32 labelw = 100.0f;
    iui::Size size(150, 25);
    iui::Position pos;

                iuiNew(iui::Label)(this, L"name", iui::Rect(pos, iui::Size(labelw, 25)));
    m_bu_name = iuiNew(iui::ToggleButton)(this, L"", iui::Rect(pos+iui::Position(labelw, 0), size), std::bind(&ConfigWindow::onNameW, this, _1));
    m_vw_name = iuiNew(EditboxWindow)(this, L"", iui::Rect(pos+iui::Position(hspace, 0), iui::Size(200, 30)), std::bind(&ConfigWindow::onNameV, this, _1));
    m_buttons.push_back(m_bu_name);
    m_windows.push_back(m_vw_name);

    pos += iui::Position(0.0f, vspace);

                iuiNew(iui::Label)(this, L"resolution", iui::Rect(pos, iui::Size(labelw, 25)));
    m_bu_reso = iuiNew(iui::ToggleButton)(this, L"", iui::Rect(pos+iui::Position(labelw, 0), size), std::bind(&ConfigWindow::onResolutionW, this, _1));
    m_vw_reso = iuiNew(ListWindow)(this, iui::Rect(pos+iui::Position(hspace, 0), iui::Size(300, 180)), std::bind(&ConfigWindow::onResolutionV, this, _1));
    m_buttons.push_back(m_bu_reso);
    m_windows.push_back(m_vw_reso);

    pos += iui::Position(0.0f, vspace);
    
                iuiNew(iui::Label)(this, L"port", iui::Rect(pos, iui::Size(labelw, 25)));
    m_bu_port = iuiNew(iui::ToggleButton)(this, L"", iui::Rect(pos+iui::Position(labelw, 0), size), std::bind(&ConfigWindow::onPortW, this, _1));
    m_vw_port = iuiNew(EditboxWindow)(this, L"", iui::Rect(pos+iui::Position(hspace, 0), iui::Size(300, 180)), std::bind(&ConfigWindow::onPortV, this, _1));
    m_buttons.push_back(m_bu_port);
    m_windows.push_back(m_vw_port);

    pos += iui::Position(0.0f, vspace);
        
                  iuiNew(iui::Label)(this, L"graphics", iui::Rect(pos, iui::Size(labelw, 25)));
    m_bu_glevel = iuiNew(iui::ToggleButton)(this, L"", iui::Rect(pos+iui::Position(labelw, 0), size), std::bind(&ConfigWindow::onRenderW, this, _1));
    m_vw_glevel = iuiNew(ListWindow)(this, iui::Rect(pos+iui::Position(hspace, 0), iui::Size(300, 180)), std::bind(&ConfigWindow::onRenderV, this, _1));
    m_buttons.push_back(m_bu_glevel);
    m_windows.push_back(m_vw_glevel);

    pos += iui::Position(0.0f, vspace);

    m_lb_desc = iuiNew(iui::Label)(this, L"", iui::Rect(pos, iui::Size(600.0f, 300.0f)));


    EnumerateAvailableResolutiuons([&](const uvec2 &res){
        wchar_t tmp[64];
        istSPrintf(tmp, L"%dx%d", res.x, res.y);
        m_vw_reso->getList()->addListItem(tmp, nullptr);
    });

    {
        typedef std::tuple<const wchar_t*, size_t> pair;
        pair graphics_options[] = {
            pair(L"low",     0),
            pair(L"medium",  1),
            pair(L"high",    2),
            pair(L"custom",  3),
        };
        each(graphics_options, [&](const pair &p){
            m_vw_glevel->getList()->addListItem(std::get<0>(p), (void*)std::get<1>(p));
        });
    }

    hideAll();
    sync();
}

void ConfigWindow::setVisibility( bool v, bool e )
{
    super::setVisibility(v,e);
    if(v) {
        sync();
    }
}

bool ConfigWindow::onCancel(const iui::WM_Widget &wm)
{
    atmGetUISelector()->popSelection();
    return true;
}

void ConfigWindow::sync()
{
    wchar_t tmp[128];

    m_bu_name->setText(atmGetConfig()->name);
    m_vw_name->getEdit()->setText(atmGetConfig()->name, false);

    istSPrintf(tmp, L"%dx%d", atmGetConfig()->window_size.x, atmGetConfig()->window_size.y);
    m_bu_reso->setText(tmp);

    istSPrintf(tmp, L"%d", atmGetConfig()->leveleditor_port);
    m_bu_port->setText(tmp);
    m_vw_port->getEdit()->setText(tmp, false);
}

void ConfigWindow::hideAll()
{
    each(m_windows, [](iui::Panel *p){ p->setVisibility(false); });
    each(m_buttons, [](iui::ToggleButton *p){ p->setPressed(false, false); });
}

void ConfigWindow::onNameW(Widget *w)
{
    hideAll();
    m_bu_name->setPressed(true, false);
    m_vw_name->setVisibility(true);
    atmGetUISelector()->pushSelection(m_vw_name);
}

void ConfigWindow::onResolutionW(Widget *)
{
    hideAll();
    m_bu_reso->setPressed(true, false);
    m_vw_reso->setVisibility(true);
    atmGetUISelector()->pushSelection(m_vw_reso);
}

void ConfigWindow::onPortW(Widget *)
{
    hideAll();
    m_bu_port->setPressed(true, false);
    m_vw_port->setVisibility(true);
    atmGetUISelector()->pushSelection(m_vw_port);
}

void ConfigWindow::onRenderW(Widget *)
{
    hideAll();
    m_bu_glevel->setPressed(true, false);
    m_vw_glevel->setVisibility(true);
    atmGetUISelector()->pushSelection(m_vw_glevel);
}


void ConfigWindow::onBGMW(Widget *)
{
}

void ConfigWindow::onSEW(Widget *)
{
}



void ConfigWindow::onNameV(Widget *w)
{
    size_t max_len = _countof(atmGetConfig()->name)-1;
    if(w->getText().size()>max_len) {
        iui::String str = w->getText();
        str.resize(max_len);
        w->setText(str);
    }
    wcscpy(atmGetConfig()->name, w->getText().c_str());
    hideAll();
    sync();
    atmGetUISelector()->popSelection();
}

void ConfigWindow::onResolutionV(Widget *w)
{
    auto *ls = static_cast<iui::List*>(w);
    if(const iui::ListItem *item = ls->getSelectedItem()) {
        ivec2 res;
        if(swscanf(item->getText().c_str(), L"%dx%d", &res.x,&res.y)==2) {
            atmGetConfig()->window_size = res;
        }
    }
    hideAll();
    sync();
    atmGetUISelector()->popSelection();
}

void ConfigWindow::onPortV(Widget *w)
{
    uint32 tmp = 0;
    if(swscanf(w->getText().c_str(), L"%d", &tmp)==0) {
        atmGetConfig()->leveleditor_port = tmp;
    }
    hideAll();
    sync();
    atmGetUISelector()->popSelection();
}

void ConfigWindow::onRenderV(Widget *w)
{
    auto *ls = static_cast<iui::List*>(w);
    if(const iui::ListItem *item = ls->getSelectedItem()) {
        AtomicConfig &conf = *atmGetConfig();
        size_t l = (size_t)item->getUserData();
        switch(l) {
        case 0: // low
            conf.lighting = atmE_Lighting_Low;
            conf.bg_level = atmE_BGResolution_x4;
            conf.posteffect_bloom = false;
            conf.show_bloodstain = false;
            break;
        case 1: // medium
            conf.lighting = atmE_Lighting_Medium;
            conf.bg_level = atmE_BGResolution_x2;
            conf.posteffect_bloom = true;
            conf.show_bloodstain = false;
            break;
        case 2: // high
            conf.lighting = atmE_Lighting_High;
            conf.bg_level = atmE_BGResolution_x1;
            conf.posteffect_bloom = true;
            conf.show_bloodstain = true;
            break;
        case 3: // custom
            break;
        }
    }
    hideAll();
    sync();
    atmGetUISelector()->popSelection();
}

void ConfigWindow::onBGMV(Widget *)
{
}

void ConfigWindow::onSEV(Widget *)
{
}

} // namespace atm
