/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2000 John Adcock.  All rights reserved.
/////////////////////////////////////////////////////////////////////////////
//
//	This file is subject to the terms of the GNU General Public License as
//	published by the Free Software Foundation.  A copy of this license is
//	included with this software distribution in the file COPYING.  If you
//	do not have a copy, you may obtain a copy by writing to the Free
//	Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
//
//	This software is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details
//
/////////////////////////////////////////////////////////////////////////////
// ID3Tag.cpp : Implementation of CID3Tag
/////////////////////////////////////////////////////////////////////////////
// Change Log
//
// Date          Developer             Changes
//
// 05 Jan 2000   John Adcock           Original Release    
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ID3COM.h"
#include "ID3Tag.h"
#include "ID3Frame.h"
#include "EnumFields.h"
#include "misc_support.h"
#include "math.h"


/////////////////////////////////////////////////////////////////////////////
// CID3Tag

CID3Tag::CID3Tag()
{
	m_ID3Tag = new ID3_Tag();
}

CID3Tag::~CID3Tag()
{
	if(m_ID3Tag != NULL)
	{
		delete m_ID3Tag;
	}
}

STDMETHODIMP CID3Tag::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IID3Tag
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CID3Tag::Link(BSTR *FileName)
{
	USES_CONVERSION;
	
	if(FileName == NULL)
	{
		return E_INVALIDARG;
	}

	try
	{
		m_ID3Tag->Link(OLE2A(*FileName));
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
	return S_OK;
}

STDMETHODIMP CID3Tag::Clear()
{
	try
	{
		m_ID3Tag->Clear();
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
	return S_OK;
}

STDMETHODIMP CID3Tag::get_HasChanged(VARIANT_BOOL *pVal)
{
	try
	{
		*pVal = VARIANT_FALSE;
		if(m_ID3Tag->HasChanged())
		{
			*pVal = VARIANT_TRUE;
		}
		else
		{
			*pVal = VARIANT_FALSE;
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::FindFrame(eID3FrameTypes FrameID, VARIANT_BOOL CreateNewIfNotFound, IID3Frame** pVal)
{
	try
	{
		*pVal = NULL;
		ID3_Frame* pFrame = m_ID3Tag->Find((enum ID3_FrameID)FrameID);
		if(CreateNewIfNotFound == VARIANT_TRUE && pFrame == NULL)
		{
			pFrame = new ID3_Frame((enum ID3_FrameID)FrameID);
			m_ID3Tag->AddFrame(pFrame);
		}
		if(pFrame != NULL)
		{
			*pVal = CID3Frame::CreateObject(this, pFrame);
			(*pVal)->AddRef();
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get__NewEnum(IUnknown** pRetVal)
{
	*pRetVal = CEnumFields::CreateObject(this);
	(*pRetVal)->AddRef();
	return S_OK;
}

STDMETHODIMP CID3Tag::get_NumFrames(long *pVal)
{
	try
	{
		*pVal = 0;
		*pVal = m_ID3Tag->NumFrames();
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		*pVal = 0;
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get_FrameByNum(long FrameNum, IID3Frame** pVal)
{
	try
	{
		*pVal = NULL;
		ID3_Frame* Frame = m_ID3Tag->GetFrameNum(FrameNum);
		*pVal = CID3Frame::CreateObject(this, Frame);
		(*pVal)->AddRef();
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::SaveV1Tag()
{
	try
	{
		m_ID3Tag->Update(V1_TAG);
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::StripV1Tag()
{
	try
	{
		m_ID3Tag->Strip(V1_TAG);
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::SaveV2Tag()
{
	try
	{
		m_ID3Tag->Update(V2_TAG);
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::StripV2Tag()
{
	try
	{
		m_ID3Tag->Strip(V2_TAG);
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get_Artist(BSTR *pVal)
{
	USES_CONVERSION;
	try
	{
		*pVal = NULL;
		char* RetStr = NULL;
		RetStr = ID3_GetArtist(m_ID3Tag);
		*pVal = SysAllocString(A2W(RetStr));
		if(RetStr != NULL)
		{
			delete [] RetStr;
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::put_Artist(BSTR newVal)
{
	USES_CONVERSION;
	try
	{
		ID3_Frame *pFrame;
		if((pFrame = m_ID3Tag->Find(ID3FID_LEADARTIST)) == NULL &&
			(pFrame = m_ID3Tag->Find(ID3FID_BAND)) == NULL &&
			(pFrame = m_ID3Tag->Find(ID3FID_CONDUCTOR)) == NULL &&
			(pFrame = m_ID3Tag->Find(ID3FID_COMPOSER)) == NULL)
		{
			pFrame = new ID3_Frame;
			if (NULL == pFrame)
			{
				ID3_THROW(ID3E_NoMemory);
			}
			pFrame->SetID(ID3FID_LEADARTIST);
			m_ID3Tag->AddNewFrame(pFrame);
		}
		pFrame->Field(ID3FN_TEXT) = OLE2A(newVal);
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get_Album(BSTR *pVal)
{
	USES_CONVERSION;
	char* RetStr = NULL;
	try
	{
		*pVal = NULL;
		RetStr = ID3_GetAlbum(m_ID3Tag);
		*pVal = SysAllocString(A2W(RetStr));
		if(RetStr != NULL)
		{
			delete [] RetStr;
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::put_Album(BSTR newVal)
{
	USES_CONVERSION;
	try
	{
		ID3_AddAlbum(m_ID3Tag, OLE2A(newVal));
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get_Title(BSTR *pVal)
{
	USES_CONVERSION;
	char* RetStr = NULL;
	try
	{
		*pVal = NULL;
		RetStr = ID3_GetTitle(m_ID3Tag);
		*pVal = SysAllocString(A2W(RetStr));
		if(RetStr != NULL)
		{
			delete [] RetStr;
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::put_Title(BSTR newVal)
{
	USES_CONVERSION;
	try
	{
		ID3_AddTitle(m_ID3Tag, OLE2A(newVal));
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get_Comment(BSTR *pVal)
{
	USES_CONVERSION;
	try
	{
		*pVal = NULL;
		char* RetStr = NULL;
		RetStr = ID3_GetComment(m_ID3Tag);
		*pVal = SysAllocString(A2W(RetStr));
		if(RetStr != NULL)
		{
			delete [] RetStr;
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::put_Comment(BSTR newVal)
{
	USES_CONVERSION;
	try
	{
		ID3_AddComment(m_ID3Tag, OLE2A(newVal));
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get_Genre(long *pVal)
{
	try
	{
		*pVal = 255;
		*pVal = ID3_GetGenreNum(m_ID3Tag);
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::put_Genre(long newVal)
{
	try
	{
		ID3_AddGenre(m_ID3Tag, newVal);
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get_Year(BSTR *pVal)
{
	USES_CONVERSION;
	try
	{
		*pVal = 0;
		char* RetStr = NULL;
		RetStr = ID3_GetYear(m_ID3Tag);
		*pVal = SysAllocString(A2W(RetStr));
		if(RetStr != NULL)
		{
			delete [] RetStr;
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::put_Year(BSTR newVal)
{
	USES_CONVERSION;
	try
	{
		ID3_AddYear(m_ID3Tag, OLE2A(newVal));
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
	return S_OK;
}

STDMETHODIMP CID3Tag::get_Track(long *pVal)
{
	try
	{
		*pVal = -1;
		*pVal = ID3_GetTrackNum(m_ID3Tag);
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::put_Track(long newVal)
{
	try
	{
		ID3_AddTrack(m_ID3Tag, (uchar)newVal);
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get_LastPlayed(DATE *pVal)
{
	try
	{
		*pVal = 0;
		ID3_Frame *pFrame = m_ID3Tag->Find(ID3FID_USERTEXT, ID3FN_DESCRIPTION, "LastPlayed");
		if (pFrame != NULL)
		{
			char* szLastPlayed = ID3_GetString(pFrame, ID3FN_TEXT);
			if(szLastPlayed != NULL)
			{
				*pVal = atof(szLastPlayed);
				delete [] szLastPlayed;
			}
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::put_LastPlayed(DATE newVal)
{
	try
	{
		ID3_Frame *pFrame = m_ID3Tag->Find(ID3FID_USERTEXT, ID3FN_DESCRIPTION, "LastPlayed");
		if(pFrame == NULL)
		{
			pFrame = new ID3_Frame;
			if (NULL == pFrame)
			{
				ID3_THROW(ID3E_NoMemory);
			}
			pFrame->SetID(ID3FID_USERTEXT);
			m_ID3Tag->AddNewFrame(pFrame);
			pFrame->Field(ID3FN_DESCRIPTION) = "LastPlayed";
			pFrame->Field(ID3FN_TEXTENC) = ID3TE_ASCII;
		}
		char Buffer[40];
		sprintf(Buffer, "%6.7f", newVal);
		pFrame->Field(ID3FN_TEXT) = Buffer;
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get_HasV1Tag(VARIANT_BOOL *pVal)
{
	try
	{
		*pVal = VARIANT_FALSE;
		if (m_ID3Tag->HasV1Tag())
		{
			*pVal = VARIANT_TRUE;
		}	
		else
		{
			*pVal = VARIANT_FALSE;
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get_HasV2Tag(VARIANT_BOOL *pVal)
{
	try
	{
		*pVal = VARIANT_FALSE;
		if (m_ID3Tag->HasV2Tag())
		{
			*pVal = VARIANT_TRUE;
		}	
		else
		{
			*pVal = VARIANT_FALSE;
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get_HasLyrics(VARIANT_BOOL *pVal)
{
	try
	{
		*pVal = VARIANT_FALSE;
		ID3_Frame *pFrame = m_ID3Tag->Find(ID3FID_UNSYNCEDLYRICS);
		if (pFrame != NULL)
		{
			*pVal = VARIANT_TRUE;
		}	
		else
		{
			*pVal = VARIANT_FALSE;
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::FindFrameString(eID3FrameTypes FrameID, eID3FieldTypes FieldType, BSTR FindString, VARIANT_BOOL CreateNewIfNotFound, IID3Frame** pVal)
{
	try
	{
		*pVal = NULL;
		ID3_Frame* pFrame = m_ID3Tag->Find((enum ID3_FrameID)FrameID, (enum ID3_FieldID)FieldType, FindString);
		if(CreateNewIfNotFound == VARIANT_TRUE && pFrame == NULL)
		{
			pFrame = new ID3_Frame((enum ID3_FrameID)FrameID);
			m_ID3Tag->AddFrame(pFrame);
			pFrame->Field((enum ID3_FieldID)FieldType) = FindString;
		}
		if(pFrame != NULL)
		{
			*pVal = CID3Frame::CreateObject(this, pFrame);
			(*pVal)->AddRef();
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}


STDMETHODIMP CID3Tag::get_PlayCount(BSTR EMailAddress, long *pVal)
{
	try
	{
		*pVal = 0;
		ID3_Frame *pFrame = m_ID3Tag->Find(ID3FID_POPULARIMETER, ID3FN_EMAIL, EMailAddress);
		if (pFrame != NULL)
		{
			*pVal = pFrame->Field(ID3FN_COUNTER).Get();
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::put_PlayCount(BSTR EMailAddress, long newVal)
{
	try
	{
		ID3_Frame *pFrame = m_ID3Tag->Find(ID3FID_POPULARIMETER, ID3FN_EMAIL, EMailAddress);
		if(pFrame == NULL)
		{
			pFrame = new ID3_Frame;
			if (NULL == pFrame)
			{
				ID3_THROW(ID3E_NoMemory);
			}
			pFrame->SetID(ID3FID_POPULARIMETER);
			m_ID3Tag->AddNewFrame(pFrame);
			pFrame->Field(ID3FN_EMAIL) = EMailAddress;
		}
		pFrame->Field(ID3FN_COUNTER) = newVal;
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get_Popularity(BSTR EMailAddress, short *pVal)
{
	try
	{
		*pVal = 0;
		ID3_Frame *pFrame = m_ID3Tag->Find(ID3FID_POPULARIMETER, ID3FN_EMAIL, EMailAddress);
		if (pFrame != NULL)
		{
			*pVal = (short)pFrame->Field(ID3FN_RATING).Get();
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::put_Popularity(BSTR EMailAddress, short newVal)
{
	try
	{
		ID3_Frame *pFrame = m_ID3Tag->Find(ID3FID_POPULARIMETER, ID3FN_EMAIL, EMailAddress);
		if(pFrame == NULL)
		{
			pFrame = new ID3_Frame;
			if (NULL == pFrame)
			{
				ID3_THROW(ID3E_NoMemory);
			}
			pFrame->SetID(ID3FID_POPULARIMETER);
			m_ID3Tag->AddNewFrame(pFrame);
			pFrame->Field(ID3FN_EMAIL) = EMailAddress;
		}
		pFrame->Field(ID3FN_RATING) = newVal;
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get_TagCreated(DATE *pVal)
{
	try
	{
		*pVal = 0;
		ID3_Frame *pFrame = m_ID3Tag->Find(ID3FID_USERTEXT, ID3FN_DESCRIPTION, "TagCreated");
		if (pFrame != NULL)
		{
			char* szTagCreated = ID3_GetString(pFrame, ID3FN_TEXT);
			if(szTagCreated != NULL)
			{
				*pVal = atof(szTagCreated);
				delete [] szTagCreated;
			}
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::put_TagCreated(DATE newVal)
{
	try
	{
		ID3_Frame *pFrame = m_ID3Tag->Find(ID3FID_USERTEXT, ID3FN_DESCRIPTION, "TagCreated");
		if(pFrame == NULL)
		{
			pFrame = new ID3_Frame;
			if (NULL == pFrame)
			{
				ID3_THROW(ID3E_NoMemory);
			}
			pFrame->SetID(ID3FID_USERTEXT);
			m_ID3Tag->AddNewFrame(pFrame);
			pFrame->Field(ID3FN_DESCRIPTION) = "TagCreated";
			pFrame->Field(ID3FN_TEXTENC) = ID3TE_ASCII;
		}
		char Buffer[40];
		sprintf(Buffer, "%6.7f", newVal);
		pFrame->Field(ID3FN_TEXT) = Buffer;
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::get_PercentVolumeAdjust(double *pVal)
{
	try
	{
		*pVal = 100;
		ID3_Frame *pFrame = m_ID3Tag->Find(ID3FID_USERTEXT, ID3FN_DESCRIPTION, "PercentVolAdj");
		if (pFrame != NULL)
		{
			char* szVolAdjust = ID3_GetString(pFrame, ID3FN_TEXT);
			if(szVolAdjust != NULL)
			{
				*pVal = atof(szVolAdjust);
				delete [] szVolAdjust;
			}
		}
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}

STDMETHODIMP CID3Tag::put_PercentVolumeAdjust(double newVal)
{
	try
	{
		ID3_Frame *pFrame = m_ID3Tag->Find(ID3FID_USERTEXT, ID3FN_DESCRIPTION, "PercentVolAdj");
		if(pFrame == NULL)
		{
			pFrame = new ID3_Frame;
			if (NULL == pFrame)
			{
				ID3_THROW(ID3E_NoMemory);
			}
			pFrame->SetID(ID3FID_USERTEXT);
			m_ID3Tag->AddNewFrame(pFrame);
			pFrame->Field(ID3FN_DESCRIPTION) = "PercentVolAdj";
			pFrame->Field(ID3FN_TEXTENC) = ID3TE_ASCII;
		}
		char Buffer[40];
		sprintf(Buffer, "%6.7f", newVal);
		pFrame->Field(ID3FN_TEXT) = Buffer;
		return S_OK;
	}
	catch (ID3_Error err)
	{
		return AtlReportError(CLSID_ID3Tag, err.GetErrorType(), IID_IID3Tag, CUSTOM_CTL_SCODE(1000 + err.GetErrorID()));
	}
	catch (...)
	{
		return AtlReportError(CLSID_ID3Tag, "An unexpected error has occurred", IID_IID3Tag, E_UNEXPECTED);
	}
}