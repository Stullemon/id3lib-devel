// $Id: header_frame.h,v 1.5 1999/12/27 06:09:26 scott Exp $
//  
// This program is free software; you can distribute it and/or modify it under 
// the terms discussed in the COPYING file, which should have been included  
// with this distribution.                                                  
//                                                                           
// This program is distributed in the hope that it will be useful, but WITHOUT 
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
// FITNESS FOR A PARTICULAR PURPOSE.  See the COPYING file for more details. 
//                                                                           
// The id3lib authors encourage improvements and optimisations to be sent to   
// the id3lib coordinator.  Please see the README file for details on where  
// to send such submissions. 

#ifndef ID3LIB_HEADER_FRAME_H
#define ID3LIB_HEADER_FRAME_H

#include "types.h"
#include "header.h"
#include "header_tag.h"
#include "field.h"

#define ID3FL_TAGALTER    (1 << 15)
#define ID3FL_FILEALTER   (1 << 14)
#define ID3FL_READONLY    (1 << 13)
#define ID3FL_COMPRESSION (1 <<  7)
#define ID3FL_ENCRYPTION  (1 <<  6)
#define ID3FL_GROUPING    (1 <<  5)

class ID3_FrameHeader : public ID3_Header
{
public:
  ID3_FrameHeader();
  
  virtual size_t Size(void);
  virtual size_t Parse(uchar *buffer);
  virtual size_t Render(uchar *buffer);
  virtual void  SetFrameID(ID3_FrameID id);
  virtual ID3_FrameID GetFrameID() const;
  virtual const char *GetTextID(void) const;
  virtual const ID3_FrameDef *GetFrameDef() const;
  virtual void Clear();
 
protected:
  virtual void Copy(const ID3_Header &hdr);

  ID3_FrameDef *__pFrameDef;
}
;

#endif

// $Log: header_frame.h,v $
// Revision 1.5  1999/12/27 06:09:26  scott
// (ID3_FrameAttr): Removed.
// (class ID3_FrameHeader): Added declarations for default constructor and
// Parse, GetFrameID, GetTextID, GetFrameDef, and Clear methods.  Changed
// return type for Size and Render.  Replaced __eFrameID data member with
// __pFrameDef, which encapsulates more information about the frame
// header.
//
// Revision 1.4  1999/12/26 16:40:18  scott
// (ID3FL_READONLY): Renamed from ID3FL_SIGNED.
// (class ID3_FrameHeader): Minor cleanup to interface.
//
// Revision 1.3  1999/12/17 16:05:02  scott
// Updated opening comment block.
//
// Revision 1.2  1999/12/02 22:45:28  scott
// Changed all of the #include <id3/*> to #include "*" to help ensure that
// the sources are searched for in the right places.
//
// Revision 1.1  1999/12/01 17:16:05  scott
// moved from src/id3 to include/id3
//
// Revision 1.6  1999/11/29 19:26:18  scott
// Updated the leading license information of the file to reflect new maintainer.
//
// Revision 1.5  1999/11/15 20:19:25  scott
// Made variable names more descriptive.
//
// Revision 1.4  1999/11/04 04:15:55  scott
// Added cvs Id and Log tags to beginning and end of file, respectively.
//
