/*
 * ***** BEGIN GPL LICENSE BLOCK *****
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
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file KX_ISceneConverter.h
 *  \ingroup ketsji
 */

#ifndef __KX_ISCENECONVERTER_H__
#define __KX_ISCENECONVERTER_H__

#include <string>
#include "EXP_Python.h"

#ifdef WITH_CXX_GUARDEDALLOC
#include "MEM_guardedalloc.h"
#endif

struct Scene;
class CListValue;

class KX_ISceneConverter 
{

public:
	KX_ISceneConverter() {}
	virtual ~KX_ISceneConverter () {}

	/*
	 * scenename: name of the scene to be converted,
	 * if the scenename is empty, convert the 'default' scene (whatever this means)
	 * destinationscene: pass an empty scene, everything goes into this
	 * dictobj: python dictionary (for pythoncontrollers)
	 */
	virtual void ConvertScene(
		class KX_Scene* destinationscene,
		class RAS_IRasterizer* rendertools,
		class RAS_ICanvas*  canvas,
		bool libloading=false)=0;

	virtual void RemoveScene(class KX_Scene *scene)=0;

	// handle any pending merges from asynchronous loads
	virtual void MergeAsyncLoads()=0;
	virtual void FinalizeAsyncLoads() = 0;

	virtual void	SetAlwaysUseExpandFraming(bool to_what) = 0;

	virtual void	SetNewFileName(const std::string& filename) = 0;
	virtual bool	TryAndLoadNewFile() = 0;

	virtual struct Scene* GetBlenderSceneForName(const std::string& name)=0;
	virtual CListValue *GetInactiveSceneNames() = 0;
	
#ifdef WITH_CXX_GUARDEDALLOC
	MEM_CXX_CLASS_ALLOC_FUNCS("GE:KX_ISceneConverter")
#endif
};

#endif  /* __KX_ISCENECONVERTER_H__ */
