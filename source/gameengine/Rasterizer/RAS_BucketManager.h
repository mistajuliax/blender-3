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

/** \file RAS_BucketManager.h
 *  \ingroup bgerast
 */

#ifndef __RAS_BUCKETMANAGER_H__
#define __RAS_BUCKETMANAGER_H__

#include "MT_Transform.h"
#include "RAS_MaterialBucket.h"

#include <vector>

class SCA_IScene;

class RAS_BucketManager
{
public:
	typedef std::vector<RAS_MaterialBucket *> BucketList;
	class SortedMeshSlot
	{
	public:
		/// depth
		MT_Scalar m_z;

		union {
			RAS_MeshSlot *m_ms;
			RAS_MeshSlotUpwardNode *m_node;
		};

		SortedMeshSlot() = default;
		SortedMeshSlot(RAS_MeshSlot *ms, const MT_Vector3& pnorm);
		SortedMeshSlot(RAS_MeshSlotUpwardNode *node, const MT_Vector3& pnorm);
	};

	struct backtofront
	{
		bool operator()(const SortedMeshSlot &a, const SortedMeshSlot &b);
	};
	struct fronttoback
	{
		bool operator()(const SortedMeshSlot &a, const SortedMeshSlot &b);
	};

protected:
	enum BucketType {
		SOLID_BUCKET = 0,
		ALPHA_BUCKET,
		SOLID_INSTANCING_BUCKET,
		ALPHA_INSTANCING_BUCKET,
		ALPHA_DEPTH_BUCKET,
		ALPHA_DEPTH_INSTANCING_BUCKET,
		SOLID_SHADOW_BUCKET,
		ALPHA_SHADOW_BUCKET,
		SOLID_SHADOW_INSTANCING_BUCKET,
		ALPHA_SHADOW_INSTANCING_BUCKET,
		TEXT_BUCKET,
		ALL_BUCKET,
		NUM_BUCKET_TYPE,
	};

	BucketList m_buckets[NUM_BUCKET_TYPE];

	RAS_ManagerDownwardNode m_downwardNode;
	RAS_ManagerUpwardNode m_upwardNode;

public:
	RAS_BucketManager();
	virtual ~RAS_BucketManager();

	void Renderbuckets(const MT_Transform & cameratrans, RAS_IRasterizer *rasty);

	RAS_MaterialBucket *FindBucket(RAS_IPolyMaterial *material, bool &bucketCreated);

	void ReleaseDisplayLists(RAS_IPolyMaterial *material = NULL);
	void ReleaseMaterials(RAS_IPolyMaterial *material = NULL);

	// freeing scenes only
	void RemoveMaterial(RAS_IPolyMaterial *mat);

	// for merging
	void MergeBucketManager(RAS_BucketManager *other, SCA_IScene *scene);
	BucketList& GetBuckets()
	{
		return m_buckets[ALL_BUCKET];
	}

private:
	void RenderBasicBuckets(const MT_Transform& cameratrans, RAS_IRasterizer *rasty, BucketType bucketType);
	void RenderSortedBuckets(const MT_Transform& cameratrans, RAS_IRasterizer *rasty, BucketType bucketType);


#ifdef WITH_CXX_GUARDEDALLOC
	MEM_CXX_CLASS_ALLOC_FUNCS("GE:RAS_BucketManager")
#endif
};

#endif // __RAS_BUCKETMANAGER_H__
