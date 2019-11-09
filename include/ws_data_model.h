/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013-2014 Jean-Pierre Charras, jp.charras at wanadoo.fr
 * Copyright (C) 1992-2016 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#ifndef  WS_DATA_MODEL_H
#define  WS_DATA_MODEL_H

#include <math/vector2d.h>
#include <eda_text.h>
#include <eda_text.h>
#include <bitmap_base.h>
#include <ws_data_item.h>



/**
 * WS_DATA_MODEL handles the graphic items list to draw/plot the frame and title block
 */
class WS_DATA_MODEL
{
    std::vector <WS_DATA_ITEM*> m_list;
    bool   m_allowVoidList;         // If false, the default page layout will be loaded the
                                    // first time WS_DRAW_ITEM_LIST::BuildWorkSheetGraphicList
                                    // is run (useful mainly for page layout editor)
    double m_leftMargin;            // the left page margin in mm
    double m_rightMargin;           // the right page margin in mm
    double m_topMargin;             // the top page margin in mm
    double m_bottomMargin;          // the bottom page margin in mm

public:
    double m_WSunits2Iu;            // conversion factor between
    // ws units (mils) and draw/plot units
    DPOINT m_RB_Corner;             // cordinates of the right bottom corner (in mm)
    DPOINT m_LT_Corner;             // cordinates of the left top corner (in mm)
    double m_DefaultLineWidth;      // Used when object line width is 0
    DSIZE  m_DefaultTextSize;       // Used when object text size is 0
    double m_DefaultTextThickness;  // Used when object text stroke width is 0
    bool   m_EditMode;              // Used in page layout editor to toggle variable substution
                                    // In normal mode (m_EditMode = false) the %format is
                                    // replaced by the corresponding text.
                                    // In edit mode (m_EditMode = true) the %format is
                                    // displayed "as this"

public:
    WS_DATA_MODEL();
    ~WS_DATA_MODEL()
    {
        ClearList();
    }

    /**
     * static function: returns the instance of WS_DATA_MODEL used in the application
     */
    static WS_DATA_MODEL& GetTheInstance();

    /**
     * static function: Set an alternate instance of WS_DATA_MODEL
     * mainly used in page setting dialog
     * @param aLayout = the alternate page layout; if null restore the basic page layout
     */
    static void SetAltInstance( WS_DATA_MODEL* aLayout = NULL );

    // Accessors:
    double GetLeftMargin() { return m_leftMargin; }
    void SetLeftMargin( double aMargin ) { m_leftMargin = aMargin; }

    double GetRightMargin() { return m_rightMargin; }
    void SetRightMargin( double aMargin ) { m_rightMargin = aMargin; }

    double GetTopMargin() { return m_topMargin; }
    void SetTopMargin( double aMargin ) { m_topMargin = aMargin; }

    double GetBottomMargin() { return m_bottomMargin; }
    void SetBottomMargin( double aMargin ) { m_bottomMargin = aMargin; }

    void SetupDrawEnvironment( const PAGE_INFO& aPageInfo, double aMilsToIU );

    /**
     * In Kicad applications, a page layout description is needed
     * So if the list is empty, a default description is loaded,
     * the first time a page layout is drawn.
     * However, in page layout editor, an empty list is acceptable.
     * AllowVoidList allows or not the empty list
     */
    void AllowVoidList( bool Allow ) { m_allowVoidList = Allow; }

    /**
     * @return true if an empty list is allowed
     * (mainly allowed for page layout editor).
     */
    bool VoidListAllowed() { return m_allowVoidList; }

    /**
     * erase the list of items
     */
    void ClearList();

    /**
     * Save the description in a file
     * @param aFullFileName the filename of the file to created
     */
    void Save( const wxString& aFullFileName );

    /**
     * Save the description in a buffer
     * @param aOutputString = a wxString to store the S expr string
     */
    void SaveInString( wxString& aOutputString );

    /**
     * Fill the given string with an S-expr serialization of the WS_DATA_ITEMs
     */
    void SaveInString( std::vector<WS_DATA_ITEM*> aItemsList, wxString& aOutputString );

    void Append( WS_DATA_ITEM* aItem );
    void Remove( WS_DATA_ITEM* aItem );

    /**
     * @return the index of aItem, or -1 if does not exist
     */
    int GetItemIndex( WS_DATA_ITEM* aItem ) const;

    /**
     * @return the item from its index aIdx, or NULL if does not exist
     */
    WS_DATA_ITEM* GetItem( unsigned aIdx ) const;

    /**
     * @return a reference to the items.
     */
    std::vector<WS_DATA_ITEM*>& GetItems() { return m_list; }

    /**
     * @return the item count
     */
    unsigned GetCount() const { return m_list.size(); }

    void SetDefaultLayout();
    void SetEmptyLayout();

    /**
     * Returns a string containing the empty layout shape
     */
    static wxString EmptyLayout();

    /**
     * Returns a string containing the empty layout shape
     */
    static wxString DefaultLayout();

    /**
     * Populates the list with a custom layout, or
     * the default layout, if no custom layout available
     * @param aFullFileName = the custom page layout description file.
     * if empty, loads the file defined by KICAD_WKSFILE
     * and if its is not defined, uses the default internal description
     * @param Append = if true: do not delete old layout, and load only
       aFullFileName.
     */
    void SetPageLayout( const wxString& aFullFileName = wxEmptyString, bool Append = false );

    /**
     * Populates the list from a S expr description stored in a string
     * @param aPageLayout = the S expr string
     * @param aAppend Do not delete old layout if true and append \a aPageLayout
     *               the existing one.
       @param aSource is the layout source description.
     */
    void SetPageLayout( const char* aPageLayout, bool aAppend = false,
                        const wxString& aSource = wxT( "Sexpr_string" )  );

    /**
     * @return a short filename  from a full filename:
     * if the path is the current project path, or if the path
     * is the same as kicad.pro (in template), returns the shortname
     * else do nothing and returns a full filename
     * @param aFullFileName = the full filename, which can be a relative
     * @param aProjectPath = the curr project absolute path (can be empty)
     */
    static const wxString MakeShortFileName( const wxString& aFullFileName,
                                             const wxString& aProjectPath );

    /**
     * Static function
     * @return a full filename from a short filename.
     * @param aShortFileName = the short filename, which can be a relative
     * @param aProjectPath = the curr project absolute path (can be empty)
     * or absolute path, and can include env variable reference ( ${envvar} expression )
     * if the short filename path is relative, it is expected relative to the project path
     * or (if aProjectPath is empty or if the file does not exist)
     * relative to kicad.pro (in template)
     * If aShortFileName is absolute return aShortFileName
     */
    static const wxString MakeFullFileName( const wxString& aShortFileName,
                                            const wxString& aProjectPath );
};

#endif      // WS_DATA_MODEL_H
