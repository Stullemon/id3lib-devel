// $Id: tag.h,v 1.46 2000/10/03 02:40:24 eldamitri Exp $

// id3lib: a software library for creating and manipulating id3v1/v2 tags
// Copyright 1999, 2000  Scott Thomas Haug

// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; either version 2 of the License, or (at your
// option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
// License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

// The id3lib authors encourage improvements and optimisations to be sent to
// the id3lib coordinator.  Please see the README file for details on where to
// send such submissions.  See the AUTHORS file for a list of people who have
// contributed to id3lib.  See the ChangeLog file for a list of changes to
// id3lib.  These files are distributed with id3lib at
// http://download.sourceforge.net/id3lib/

#ifndef _ID3LIB_TAG_H_
#define _ID3LIB_TAG_H_

#include "frame.h"
#include "field.h"
#include "spec.h"

class ID3_Reader;
class ID3_TagImpl;

class ID3_Tag : public ID3_Speccable
{
  ID3_TagImpl* _impl;
public:
  ID3_Tag(const char *name = NULL);
  ID3_Tag(const ID3_Tag &tag);
  virtual ~ID3_Tag();
  
  void       Clear();
  bool       HasChanged() const;
  size_t     Size() const;
  
  bool       SetUnsync(bool bSync);
  bool       SetExtendedHeader(bool bExt);
  bool       SetPadding(bool bPad);
  
  void       AddFrame(const ID3_Frame&);
  void       AddFrame(const ID3_Frame*);
  void       AttachFrame(ID3_Frame*);
  ID3_Frame* RemoveFrame(const ID3_Frame *);
  
  bool       Parse(ID3_Reader& reader);
  size_t     Parse(const uchar*, size_t);
  size_t     Parse(const uchar header[ID3_TAGHEADERSIZE], const uchar *buffer);
  size_t     Render(uchar*, ID3_TagType = ID3TT_ID3V2) const;
  
  size_t     Link(const char *fileInfo, flags_t = (flags_t) ID3TT_ALL);
  flags_t    Update(flags_t = (flags_t) ID3TT_ALL);
  flags_t    Strip(flags_t = (flags_t) ID3TT_ALL);
  
  size_t     GetPrependedBytes() const;
  size_t     GetAppendedBytes() const;
  size_t     GetFileSize() const;
  const char* GetFileName() const;
  
  /// Finds frame with given frame id
  ID3_Frame* Find(ID3_FrameID id) const;
  
  /// Finds frame with given frame id, fld id, and integer data
  ID3_Frame* Find(ID3_FrameID id, ID3_FieldID fld, uint32 data) const;
  
  /// Finds frame with given frame id, fld id, and ascii data
  ID3_Frame* Find(ID3_FrameID id, ID3_FieldID fld, const char*) const;
  
  /// Finds frame with given frame id, fld id, and unicode data
  ID3_Frame* Find(ID3_FrameID id, ID3_FieldID fld, const unicode_t*) const;
  
  /** Returns the number of frames present in the tag object.
   ** 
   ** This includes only those frames that id3lib recognises.  This is used as
   ** the upper bound on calls to the GetFrame() and operator[]() methods.
   ** 
   ** \return The number of frames present in the tag object.
   **/
  size_t     NumFrames() const;
  ID3_Frame* GetFrameNum(index_t) const;

  ID3_Frame* operator[](index_t) const;
  ID3_Tag&   operator=( const ID3_Tag & );
  
  bool       GetUnsync() const;
  
  bool       HasTagType(uint16 tt) const;
  ID3_V2Spec GetSpec() const;
  bool       SetSpec(ID3_V2Spec);
  
  static size_t IsV2Tag(const uchar*);
  static size_t IsV2Tag(ID3_Reader&);
  
  /* Deprecated! */
  void       AddNewFrame(ID3_Frame* f);
  size_t     Link(const char *fileInfo, bool parseID3v1, bool parseLyrics3);
  void       SetCompression(bool);
  void       AddFrames(const ID3_Frame *, size_t);
  bool       HasLyrics() const;
  bool       HasV2Tag()  const;
  bool       HasV1Tag()  const;
  //@{
  /// Copies
  ID3_Tag& operator<<(const ID3_Frame &);
  /// Attaches a pointer to a frame
  ID3_Tag& operator<<(const ID3_Frame *);
  //@}
};

// deprecated!
int32 ID3_IsTagHeader(const uchar header[ID3_TAGHEADERSIZE]);


#endif /* _ID3LIB_TAG_H_ */
