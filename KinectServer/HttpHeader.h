/*!
 * \file: HttpHeader.h
 * \brief: 
 * \author: Andrew (mail4cod@gmail.com) Bulavinov
 * \created: 2010-07-08
 */

#ifndef _HTTPHEADER_H_
#define _HTTPHEADER_H_

#include <wx/wx.h>
#include <map>

class HttpHeader {
    public:
        HttpHeader(wxString buffer = wxEmptyString);
        
        ~HttpHeader();

        wxString    GetHeader(wxString name);

        void        SetHeader(wxString name, wxString value);

        wxString    GetBuffer();

        void        ParseBuffer(wxString buffer);

        void        Clear();

    private:
        typedef std::map<wxString, wxString> Headers;

        Headers     _headers;

        void        _ParseBuffer(wxString & buffer, Headers & headers);

        wxString    _CreateBuffer(Headers & headers);
};

#endif /* _HTTPHEADER_H_ */

