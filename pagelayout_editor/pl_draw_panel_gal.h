/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2017 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PL_DRAW_PANEL_GAL_H
#define PL_DRAW_PANEL_GAL_H

#include <class_draw_panel_gal.h>
#include <ws_proxy_view_item.h>
#include <ws_painter.h>


class PL_DRAW_PANEL_GAL : public EDA_DRAW_PANEL_GAL
{
public:
    PL_DRAW_PANEL_GAL( wxWindow* aParentWindow, wxWindowID aWindowId, const wxPoint& aPosition,
                       const wxSize& aSize, KIGFX::GAL_DISPLAY_OPTIONS& aOptions,
                       GAL_TYPE aGalType = GAL_TYPE_OPENGL );

    virtual ~PL_DRAW_PANEL_GAL();

    ///> @copydoc EDA_DRAW_PANEL_GAL::GetMsgPanelInfo()
    void GetMsgPanelInfo( EDA_UNITS_T aUnits, std::vector<MSG_PANEL_ITEM>& aList ) override;

    /**
     * Build and update the list of WS_DRAW_ITEM_xxx showing the frame layout
     */
    void DisplayWorksheet();

    ///> @copydoc EDA_DRAW_PANEL_GAL::SwitchBackend
    bool SwitchBackend( GAL_TYPE aGalType ) override;

    ///> @copydoc EDA_DRAW_PANEL_GAL::SetTopLayer
    virtual void SetTopLayer( int aLayer ) override;

protected:
    ///> Sets rendering targets & dependencies for layers.
    void setDefaultLayerDeps();

    ///> Currently used worksheet
    std::unique_ptr<KIGFX::WS_PROXY_VIEW_ITEM> m_worksheet;
};


#endif /* PL_DRAW_PANEL_GAL_H */
