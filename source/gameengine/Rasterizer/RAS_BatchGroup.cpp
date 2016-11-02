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
 * Contributor(s): Tristan Porteries.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file gameengine/Rasterizer/RAS_BatchGroup.cpp
 *  \ingroup bgerast
 */

#include "RAS_BatchGroup.h"
#include "RAS_IDisplayArrayBatching.h"
#include "RAS_MaterialBucket.h"
#include "RAS_MeshUser.h"
#include "RAS_MeshSlot.h"

RAS_BatchGroup::RAS_BatchGroup()
{
}

RAS_BatchGroup::~RAS_BatchGroup()
{
	for (std::map<RAS_IPolyMaterial *, Batch>::iterator it = m_batchs.begin(), end = m_batchs.end(); it != end; ++it) {
		Batch& batch = it->second;
		batch.m_displayArrayBucket->Release();
	}
}

bool RAS_BatchGroup::Merge(RAS_BatchGroup::Batch& batch, RAS_MeshSlot *slot, const MT_Matrix4x4& mat)
{
	RAS_DisplayArrayBucket *origArrayBucket = slot->m_displayArrayBucket;
	RAS_IDisplayArray *origArray = origArrayBucket->GetDisplayArray();
	RAS_DisplayArrayBucket *arrayBucket = batch.m_displayArrayBucket;
	RAS_IDisplayArrayBatching *array = batch.m_displayArray;

	// Don't merge if the vertex format or pimitive type is not the same.
	if (origArray->GetFormat() != array->GetFormat() || origArray->GetPrimitiveType() != array->GetPrimitiveType()) {
		return false;
	}

	// Store original display array bucket.
	m_originalDisplayArrayBucketList[slot] = origArrayBucket->AddRef();

	// Merge display array.
	array->Merge(origArray, mat);

	arrayBucket->DestructStorageInfo();

	slot->SetDisplayArrayBucket(arrayBucket);

	return true;
}

bool RAS_BatchGroup::Split(RAS_MeshSlot *slot)
{
	RAS_DisplayArrayBucket *origArrayBucket = m_originalDisplayArrayBucketList[slot];

	if (!origArrayBucket) {
		return false;
	}

	slot->SetDisplayArrayBucket(origArrayBucket);
	origArrayBucket->Release();

	m_originalDisplayArrayBucketList.erase(slot);

	return true;
}

bool RAS_BatchGroup::Merge(RAS_MeshUser *meshUser, const MT_Matrix4x4& mat)
{
	const RAS_MeshSlotList& meshSlots = meshUser->GetMeshSlots();
	for (RAS_MeshSlotList::const_iterator it = meshSlots.begin(), end = meshSlots.end(); it != end; ++it) {
		RAS_MeshSlot *meshSlot = *it;
		RAS_IPolyMaterial *material = meshSlot->m_bucket->GetPolyMaterial();

		Batch& batch = m_batchs[material];
		// Create the batch if it is empty.
		if (!batch.m_displayArray && !batch.m_displayArrayBucket) {
			RAS_IDisplayArray *origarray = meshSlot->GetDisplayArray();
			batch.m_displayArray = RAS_IDisplayArrayBatching::ConstructArray(origarray->GetPrimitiveType(), origarray->GetFormat());
			batch.m_displayArrayBucket = new RAS_DisplayArrayBucket(meshSlot->m_bucket, batch.m_displayArray,
																	meshSlot->m_mesh, meshSlot->m_meshMaterial);
		}

		if (!Merge(batch, meshSlot, mat)) {
			return false;
		}
	}

	return true;
}

bool RAS_BatchGroup::Split(RAS_MeshUser *meshUser)
{
	const RAS_MeshSlotList& meshSlots = meshUser->GetMeshSlots();
	for (RAS_MeshSlotList::const_iterator it = meshSlots.begin(), end = meshSlots.end(); it != end; ++it) {
		RAS_MeshSlot *meshSlot = *it;
		if (!Split(meshSlot)) {
			return false;
		}
	}

	return true;
}
