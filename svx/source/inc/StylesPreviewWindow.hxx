/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; fill-column: 100 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#ifndef INCLUDED_SVX_SOURCE_INC_STYLES_PREVIEW_WINDOW_HXX
#define INCLUDED_SVX_SOURCE_INC_STYLES_PREVIEW_WINDOW_HXX

#include <vcl/InterimItemWindow.hxx>
#include <vcl/customweld.hxx>
#include <svl/style.hxx>
#include <com/sun/star/frame/XDispatchProvider.hpp>
#include <sfx2/sfxstatuslistener.hxx>

class StylesPreviewWindow_Base;

class StyleStatusListener : public SfxStatusListener
{
    StylesPreviewWindow_Base* m_pPreviewControl;

public:
    StyleStatusListener(StylesPreviewWindow_Base* pPreviewControl,
                        css::uno::Reference<css::frame::XDispatchProvider>& xDispatchProvider);

    void StateChanged(SfxItemState eState, const SfxPoolItem* pState) override;
};

class StyleItemController : public weld::CustomWidgetController
{
    static constexpr unsigned LEFT_MARGIN = 8;

    SfxStyleFamily m_eStyleFamily;
    OUString m_aStyleName;
    bool m_bSelected;
    css::uno::Reference<css::frame::XDispatchProvider> m_xDispatchProvider;

public:
    StyleItemController(const OUString& aStyleName,
                        css::uno::Reference<css::frame::XDispatchProvider>& xDispatchProvider);

    void Paint(vcl::RenderContext& rRenderContext, const tools::Rectangle& rRect) override;

    bool MouseButtonDown(const MouseEvent&) override;

    void SetStyle(const OUString& sStyleName);

    void Select(bool bSelect);

private:
    void DrawEntry(vcl::RenderContext& rRenderContext);
    void DrawText(vcl::RenderContext& rRenderContext);
    void DrawHighlight(vcl::RenderContext& rRenderContext, Color aFontBack);
    static void DrawSelection(vcl::RenderContext& rRenderContext);
    static void DrawContentBackground(vcl::RenderContext& rRenderContext,
                                      tools::Rectangle& aContentRect, Color& aColor);
};

class StylesPreviewWindow_Base
{
protected:
    static constexpr unsigned STYLES_COUNT = 4;

    std::unique_ptr<StyleItemController> m_xStyleControllers[STYLES_COUNT];
    std::unique_ptr<weld::CustomWeld> m_xStyleControllersWeld[STYLES_COUNT];

    StyleStatusListener* m_pStatusListener;
    css::uno::Reference<css::lang::XComponent> m_xStatusListener;

    std::unique_ptr<weld::Toolbar> m_xUp;
    std::unique_ptr<weld::Toolbar> m_xDown;

    std::vector<OUString> m_aDefaultStyles;
    std::vector<OUString> m_aAllStyles;

    unsigned m_nStyleIterator;
    OUString m_sSelectedStyle;

    DECL_LINK(GoUp, const OString&, void);
    DECL_LINK(GoDown, const OString&, void);

public:
    StylesPreviewWindow_Base(weld::Builder& xBuilder, std::vector<OUString>& aDefaultStyles,
                             css::uno::Reference<css::frame::XDispatchProvider>& xDispatchProvider);
    ~StylesPreviewWindow_Base();

    void Select(const OUString& rStyleName);

private:
    void Update();
    void UpdateStylesList();
    void MakeCurrentStyleVisible();
    OUString GetVisibleStyle(unsigned nPosition);
};

class StylesPreviewWindow_Impl : public InterimItemWindow, public StylesPreviewWindow_Base
{
public:
    StylesPreviewWindow_Impl(vcl::Window* pParent, std::vector<OUString>& aDefaultStyles,
                             css::uno::Reference<css::frame::XDispatchProvider>& xDispatchProvider);
    ~StylesPreviewWindow_Impl();

    void dispose();

    void SetOptimalSize();
};

#endif

/* vim:set shiftwidth=4 softtabstop=4 expandtab cinoptions=b1,g0,N-s cinkeys+=0=break: */