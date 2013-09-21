/*!
 * \file: HttpHeader.cpp
 * \brief: 
 * \author: Andrew (mail4cod@gmail.com) Bulavinov
 * \created: 2010-07-08
 */

#include "HttpHeader.h"
#include <wx/tokenzr.h>
#include <wx/string.h>

HttpHeader::HttpHeader(wxString buffer)
{
    _ParseBuffer(buffer, _headers);
}

HttpHeader::~HttpHeader()
{
}

wxString HttpHeader::GetHeader(wxString name)
{
    Headers::iterator hi = _headers.find(name.Lower());
    if (hi != _headers.end()) {
        return hi->second;
    }

    return wxEmptyString;
}

void HttpHeader::SetHeader(wxString name, wxString value)
{
    _headers[name.Lower()] = value;
}

wxString HttpHeader::GetBuffer()
{
    wxString buffer;
    wxString uri(GetHeader(wxT("GET")));

    buffer = wxString::Format(wxT("GET %s HTTP/1.1\r\n"), uri.IsEmpty() ? wxT("/") : uri.GetData());

    for (Headers::iterator hi = _headers.begin(); hi != _headers.end(); ++hi) {
        if (hi->first == wxT("get")) { continue; }

        buffer.Append(wxString::Format(wxT("%s: %s\r\n"), hi->first.GetData(), hi->second.GetData()));
    }

    buffer.Append(wxT("\r\n"));

    return buffer;
}

void HttpHeader::ParseBuffer(wxString buffer)
{
    _ParseBuffer(buffer, _headers);
}

wxString HttpHeader::_CreateBuffer(Headers & headers)
{
    return wxEmptyString;
}

void HttpHeader::Clear()
{
    _headers.clear();
}

void HttpHeader::_ParseBuffer(wxString & buffer, Headers & headers)
{
    headers.clear();

    wxArrayString lines = wxStringTokenize(buffer, wxT("\n\r"));

    for (size_t i = 0; i < lines.size(); ++i) {
        wxString line = lines[i].Trim().Trim(false);
        
        if (line.IsEmpty()) { continue; }

        if (line.Left(4).Lower() == wxT("get ")) {
            line = line.Mid(4);
            size_t end = line.Find(wxT(" "));
            headers[wxT("get")] = line.Mid(0, end != wxNOT_FOUND ? end : wxString::npos);

            continue;
        }

        if (line.Left(8).Lower() == wxT("connect ")) {
            line = line.Mid(8);
            size_t end = line.Find(wxT(" "));
            headers[wxT("connect")] = line.Mid(0, end != wxNOT_FOUND ? end : wxString::npos);

            continue; 
        }

        size_t column = line.Find(wxT(": "));

        if (column != wxNOT_FOUND) {
            wxString header = line.Left(column).Trim(), value = line.Right(line.length() - column - 2).Trim(false);
            headers[header.Lower()] = value;
        }
    }
}
