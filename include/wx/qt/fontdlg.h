/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/fontdlg.h
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_FONTDLG_H_
#define _WX_QT_FONTDLG_H_

#include <QtWidgets/QFontDialog>

class WXDLLIMPEXP_CORE wxFontDialog : public wxFontDialogBase
{
public:
    wxFontDialog();
    wxFontDialog(wxWindow *parent);
    wxFontDialog(wxWindow *parent, const wxFontData& data);
    
private:
//    QFontDialog *GetFontDialog() const { return static_cast<QFontDialog *>(m_qtWindow); }
};

#endif // _WX_QT_FONTDLG_H_
