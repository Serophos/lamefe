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


#include "stdafx.h"
#include "TagRenderer.h"


//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

TagRenderer::TagRenderer(LPCTSTR filename)
{

	//Lock to file
	id3Tag.Link(filename, ID3TT_ID3V2);
	
}

TagRenderer::~TagRenderer()
{

}

BOOL TagRenderer::RenderTag(LPCTSTR song,
							 LPCTSTR artist,
							 LPCTSTR album,
							 LPCTSTR comment,
							 LPCTSTR genre,
							 LPCTSTR year,
							 LPCTSTR track)
{

	ID3_Frame	*buffer = NULL;

	//title
	if(song != NULL && song != ""){

		buffer = id3Tag.Find(ID3FID_TITLE);
		if(buffer){// Remove if already exists
			id3Tag.RemoveFrame(buffer);
		}
		id3Frame.SetID(ID3FID_TITLE);
		id3Frame.Field(ID3FN_TEXT).Set(song);
		id3Tag.AddFrame(&id3Frame);
		id3Tag.Update();
	}


	//artist
	if(artist != NULL && artist != ""){

		buffer = id3Tag.Find(ID3FID_LEADARTIST);
		if(buffer){// Remove if already exists
			id3Tag.RemoveFrame(buffer);
		}
		id3Frame.SetID(ID3FID_LEADARTIST);
		id3Frame.Field(ID3FN_TEXT).Set(artist);
		id3Tag.AddFrame(&id3Frame);
		id3Tag.Update();
	}

	//album
	if(album != NULL && artist != ""){

		buffer = id3Tag.Find(ID3FID_ALBUM);
		if(buffer){// Remove if already exists
			id3Tag.RemoveFrame(buffer);
		}
		id3Frame.SetID(ID3FID_ALBUM);
		id3Frame.Field(ID3FN_TEXT).Set(album);
		id3Tag.AddFrame(&id3Frame);
		id3Tag.Update();
	}

	//comment
	if(comment != NULL && comment != ""){

		buffer = id3Tag.Find(ID3FID_COMMENT);
		if(buffer){// Remove if already exists
			id3Tag.RemoveFrame(buffer);
		}
		id3Frame.SetID(ID3FID_COMMENT);
		id3Frame.Field(ID3FN_TEXT).Set(comment);
		id3Tag.AddFrame(&id3Frame);
		id3Tag.Update();
	}

	//year
	if(year != NULL && year != ""){

		buffer = id3Tag.Find(ID3FID_YEAR);
		if(buffer){// Remove if already exists
			id3Tag.RemoveFrame(buffer);
		}
		id3Frame.SetID(ID3FID_YEAR);
		id3Frame.Field(ID3FN_TEXT).Set(year);
		id3Tag.AddFrame(&id3Frame);
		id3Tag.Update();
	}

	//track
	if(track != NULL && track != ""){

		buffer = id3Tag.Find(ID3FID_TRACKNUM);
		if(buffer){// Remove if already exists
			id3Tag.RemoveFrame(buffer);
		}
		id3Frame.SetID(ID3FID_TRACKNUM);
		id3Frame.Field(ID3FN_TEXT).Set(track);
		id3Tag.AddFrame(&id3Frame);
		id3Tag.Update();
	}

	if(genre != NULL && genre != ""){

		buffer = id3Tag.Find(ID3FID_CONTENTTYPE);
		if(buffer){// Remove if already exists
			id3Tag.RemoveFrame(buffer);
		}
		id3Frame.SetID(ID3FID_CONTENTTYPE);
		id3Frame.Field(ID3FN_TEXT).Set(genre);
		id3Tag.AddFrame(&id3Frame);
		id3Tag.Update();
	}

	

	return true;
}
