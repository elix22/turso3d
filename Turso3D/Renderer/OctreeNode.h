// For conditions of distribution and use, see copyright notice in License.txt

#pragma once

#include "../Math/BoundingBox.h"
#include "../Scene/SpatialNode.h"

namespace Turso3D
{

class Camera;
class Octree;
class Ray;
struct Octant;
struct RaycastResult;

/// Base class for scene nodes that insert themselves to the octree for rendering.
class TURSO3D_API OctreeNode : public SpatialNode
{
    friend class Octree;

    OBJECT(OctreeNode);

public:
    /// Construct.
    OctreeNode();
    /// Destruct. Remove self from the octree.
    ~OctreeNode();

    /// Register attributes.
    static void RegisterObject();

    /// Prepare object for rendering. Reset framenumber and calculate distance from camera. Called by Renderer.
    virtual void OnPrepareRender(unsigned frameNumber, Camera* camera);
    /// Perform ray test on self and add possible hit to the result vector.
    virtual void OnRaycast(Vector<RaycastResult>& dest, const Ray& ray, float maxDistance);

    /// Set whether to cast shadows. Default false on both lights and geometries.
    void SetCastShadows(bool enable);
    
    /// Return world space bounding box. Update if necessary.
    const BoundingBox& WorldBoundingBox() const { if (TestFlag(NF_BOUNDING_BOX_DIRTY)) OnWorldBoundingBoxUpdate(); return worldBoundingBox; }
    /// Return whether casts shadows.
    bool CastShadows() const { return TestFlag(NF_CASTSHADOWS); }
    /// Return current octree this node resides in.
    Octree* GetOctree() const { return octree; }
    /// Return current octree octant this node resides in.
    Octant* GetOctant() const { return octant; }
    /// Return distance from camera in the current view.
    float Distance() const { return distance; }
    /// Return last frame number when was visible. The frames are counted by Renderer internally and have no significance outside it.
    unsigned LastFrameNumber() const { return lastFrameNumber; }

protected:
    /// Search for an octree from the scene root and add self to it.
    void OnSceneSet(Scene* newScene, Scene* oldScene) override;
    /// Handle the transform matrix changing.
    void OnTransformChanged() override;
    /// Recalculate the world space bounding box.
    virtual void OnWorldBoundingBoxUpdate() const;

    /// World space bounding box.
    mutable BoundingBox worldBoundingBox;
    /// Distance from camera in the current view.
    float distance;
    /// Last frame number when was visible.
    unsigned lastFrameNumber;

private:
    /// Remove from the current octree.
    void RemoveFromOctree();

    /// Current octree.
    Octree* octree;
    /// Current octree octant.
    Octant* octant;
};

}
