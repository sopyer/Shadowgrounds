// Copyright 2002-2004 Frozenbyte Ltd.

#pragma once


//------------------------------------------------------------------
// Includes
//------------------------------------------------------------------
#include "GfxDevice.h"
#include "storm3d_common_imp.h"
#include "IStorm3D_Mesh.h"
#include "storm3d_mesh_collisiontable.h"
#include "storm3d_resourcemanager.h"
#include <c2_sphere.h>
#include <vector>
#include <stdint.h>

class Storm3D_Bone;

//------------------------------------------------------------------
// Storm3D_Weight
//------------------------------------------------------------------
struct Storm3D_Weight
{
	Storm3D_Weight():
		index1(-1), index2(-1), weight1(0), weight2(0) {}

	int index1;
	int index2;
	signed char weight1;
	signed char weight2;
};


struct Storm3D_BoneChunk
{
    Storm3D_BoneChunk()
      : idx_id(0),
        vtx_id(0),
        base_index(0),
        base_vertex(0),
        index_count(0),
        vertex_count(0)
    {
    }

    std::vector<int> bone_indices;
    uint16_t idx_id;
    uint16_t vtx_id;
    int base_index;
    int index_count;
    int base_vertex;
    int vertex_count;
};


//------------------------------------------------------------------
// Storm3D_Mesh
//------------------------------------------------------------------
class Storm3D_Mesh : public IStorm3D_Mesh
{
	// Pointer to Storm3D interface
	Storm3D *Storm3D2;
	Storm3D_ResourceManager &resourceManager;

	// Material mesh uses
	Storm3D_Material *material;

	// Vertex/face data (temporary)
	int face_amount[LOD_AMOUNT];
    int base_index[LOD_AMOUNT];
	int vertex_amount;
    int base_vertex;
	int render_face_amount[LOD_AMOUNT];
	int render_vertex_amount;
	bool hasLods;

	Storm3D_Vertex		*vertexes;
	Storm3D_Face		*faces[LOD_AMOUNT];

	// Weight data
	Storm3D_Weight *bone_weights;
	// Split data
	std::vector<Storm3D_BoneChunk> bone_chunks[LOD_AMOUNT];

	// DirectX Buffers (temporary)
    uint16_t vertices_id;
    uint16_t indices_id[LOD_AMOUNT];

	int vbuf_vsize;
	FVF vbuf_fvf;
	
	// Collision table
	Storm3D_Mesh_CollisionTable collision;

	float radius;			// Distance from object center (=position) to the most distant vertex (squared for optimization)
	float sq_radius;		// Squared radius (for optimization)
	float radius2d;
	//Storm3D_Box box;		// Bounding box
	bool rb_update_needed;	// Set if box/radius need to be rebuilt

	// New update(and rebuild) system
	bool update_vx;
	bool update_vx_amount;
	bool update_fc;
	bool update_fc_amount;

	// Old... change these
	//bool lod_rebuild_needed;
	//bool lod_rebuild_needed_collision;
	bool col_rebuild_needed;

	// Precalculation (speed up)
	void CalculateRadiusAndBox();

	mutable Sphere bounding_sphere;
	mutable AABB bounding_box;
	mutable bool sphere_ok;
	mutable bool box_ok;

    void clearBoneChunks();

public:
	const Sphere &getBoundingSphere() const;
	const AABB &getBoundingBox() const;
	
	DWORD GetFVF() const { return vbuf_fvf; }
	// When vertexes/faces are changed, buffers in videomemory must be rebuided.
	void ReBuild();

	// Prepare for rendering (v3)
	void PrepareForRender(Storm3D_Scene *scene,Storm3D_Model_Object *object);	// You can set scene=NULL, object=NULL if you dont need animation
	void PrepareMaterialForRender(Storm3D_Scene *scene,Storm3D_Model_Object *object);

	// Rendering
	void RenderBuffers(Storm3D_Model_Object *object);		// Renders buffers only
	void Render(Storm3D_Scene *scene,bool mirrored,Storm3D_Model_Object *object);		// Applies material, animates, renders (use this mainly)
	void RenderWithoutMaterial(Storm3D_Scene *scene,bool mirrored,Storm3D_Model_Object *object);
	void RenderToBackground(Storm3D_Scene *scene,Storm3D_Model_Object *object);
	
	// Creation/delete
	Storm3D_Mesh(Storm3D *Storm3D2, Storm3D_ResourceManager &resourceManager);
	~Storm3D_Mesh();

	IStorm3D_Mesh *CreateNewClone();

	// Radius/box get (recreates if needed)
	float GetRadius();
	float GetSquareRadius();
	float GetRadius2D() { return radius2d; }

	// Material functions
	void UseMaterial(IStorm3D_Material *_material);
	IStorm3D_Material *GetMaterial();	// Returns NULL if no material used

	// Clear (face/vertex) 
	void DeleteAllFaces();
	void DeleteAllVertexes();

	// Dynamic geometry edit (New in v1.2B)
	Storm3D_Face *GetFaceBuffer();
	Storm3D_Vertex *GetVertexBuffer();
	const Storm3D_Face *GetFaceBufferReadOnly();
	const Storm3D_Face *GetFaceBufferReadOnly(int lodIndex);
	const Storm3D_Vertex *GetVertexBufferReadOnly();
	int GetFaceCount();
	int GetRenderFaceCount() const { return render_face_amount[0]; };
	int GetFaceCount(int lodIndex);
	int GetVertexCount();
	void ChangeFaceCount(int new_face_count);
	void ChangeVertexCount(int new_vertex_count);
	void UpdateCollisionTable();	// Use this after editing, if you want collisions to this object
	
	bool HasWeights() const;

	// Test ray collision (these return true if collided)
	bool RayTrace(const VC3 &position,const VC3 &direction_normalized,float ray_length,Storm3D_CollisionInfo &rti, bool accurate);
	bool SphereCollision(const VC3 &position,float radius,Storm3D_CollisionInfo &cinf, bool accurate);

	friend class Storm3D_Model;
	friend class Storm3D_Mesh_CollisionTable;
	friend struct Storm3D_KeyFrame_Mesh;
};



