/*
** Copyright (C) 2002 Thees Winkler
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/


#if !defined(AFX_TAGRENDERER_H__87D9D742_7EBA_427B_AB86_4DDF0DC0B79D__INCLUDED_)
#define AFX_TAGRENDERER_H__87D9D742_7EBA_427B_AB86_4DDF0DC0B79D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <id3/tag.h>

//#ifdef _WIN32
//  #define CCONV _stdcall
//#endif


class TagRenderer  
{
public:
	TagRenderer(LPCTSTR filename);
	virtual ~TagRenderer();
	BOOL RenderTag(LPCTSTR song,
					 LPCTSTR artist,
					 LPCTSTR album,
					 LPCTSTR comment,
					 LPCTSTR genre,
					 LPCTSTR year,
					 LPCTSTR track);

private:

	ID3_Tag		id3Tag;
	ID3_Frame	id3Frame;
	ID3_Field	*id3Field;
};



#endif // !defined(AFX_TAGRENDERER_H__87D9D742_7EBA_427B_AB86_4DDF0DC0B79D__INCLUDED_)
