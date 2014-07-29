/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/statbox.h
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_STATBOX_H_
#define _WX_QT_STATBOX_H_

#include <QtWidgets/QGroupBox>

class WXDLLIMPEXP_CORE wxStaticBox : public wxStaticBoxBase
{
public:
    wxStaticBox();
    
    wxStaticBox(wxWindow *parent, wxWindowID id,
                const wxString& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxStaticBoxNameStr);

    bool Create(wxWindow *parent, wxWindowID id,
                const wxString& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxStaticBoxNameStr);

    virtual void GetBordersForSizer(int *borderTop, int *borderOther) const;

private:
    QGroupBox *GetQGroupBox() const { return static_cast<QGroupBox *>(m_qtWindow); }

    wxDECLARE_DYNAMIC_CLASS_NO_COPY( wxStaticBox );
};

#endif // _WX_QT_STATBOX_H_
