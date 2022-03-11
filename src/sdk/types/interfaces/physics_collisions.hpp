#pragma once

#include "../../other.hpp"

namespace sdk::interfaces {
	struct polyhedron { };
	struct convertconvexparams_t { };
	struct truncatedcone_t { };
	struct vcollide {
		unsigned short m_solid_count : 15;
		unsigned short m_is_packed   : 1;
		unsigned short m_desc_size;
		// VPhysicsSolids
		void** solids;
		char* m_key_values;
		void* m_user_data;
	};
	struct physics_collison {
	public:

		virtual ~physics_collison( void ) { }

		// produce a convex element from verts (convex hull around verts)
		virtual void* ConvexFromVerts( math::v3f** pVerts, int vertCount ) = 0;
		// produce a convex element from planes (csg of planes)
		virtual void* ConvexFromPlanes( float* pPlanes, int planeCount, float mergeDistance ) = 0;
		// calculate volume of a convex element
		virtual float ConvexVolume( void* pConvex ) = 0;

		virtual float ConvexSurfaceArea( void* pConvex ) = 0;
		// store game-specific data in a convex solid
		virtual void SetConvexGameData( void* pConvex, unsigned int gameData ) = 0;
		// If not converted, free the convex elements with this call
		virtual void ConvexFree( void* pConvex )                                   = 0;
		virtual void* BBoxToConvex( const math::v3f& mins, const math::v3f& maxs ) = 0;
		// produce a convex element from a convex polyhedron
		virtual void* ConvexFromConvexPolyhedron( const polyhedron& ConvexPolyhedron ) = 0;
		// produce a set of convex triangles from a convex polygon, normal is assumed to be on the side with forward point ordering, which should be
		// clockwise, output will need to be able to hold exactly (iPointCount-2) convexes
		virtual void ConvexesFromConvexPolygon( const math::v3f& vPolyNormal, const math::v3f* pPoints, int iPointCount, void** pOutput ) = 0;

		// concave objects
		// create a triangle soup
		virtual void* PolysoupCreate( void ) = 0;
		// destroy the container and memory
		virtual void PolysoupDestroy( void* pSoup ) = 0;
		// add a triangle to the soup
		virtual void PolysoupAddTriangle( void* pSoup, const math::v3f& a, const math::v3f& b, const math::v3f& c, int materialIndex7bits ) = 0;
		// convert the convex into a compiled collision model
		virtual void* ConvertPolysoupToCollide( void* pSoup, bool useMOPP ) = 0;

		// Convert an array of convex elements to a compiled collision model (this deletes the convex elements)
		virtual void* ConvertConvexToCollide( void** pConvex, int convexCount )                                                   = 0;
		virtual void* ConvertConvexToCollideParams( void** pConvex, int convexCount, const convertconvexparams_t& convertParams ) = 0;
		// Free a collide that was created with ConvertConvexToCollide()
		virtual void DestroyCollide( void* pCollide ) = 0;

		// Get the memory size in bytes of the collision model for serialization
		virtual int CollideSize( void* pCollide ) = 0;
		// serialize the collide to a block of memory
		virtual int CollideWrite( char* pDest, void* pCollide, bool bSwap = false ) = 0;
		// unserialize the collide from a block of memory
		virtual void* UnserializeCollide( char* pBuffer, int size, int index ) = 0;

		// compute the volume of a collide
		virtual float CollideVolume( void* pCollide ) = 0;
		// compute surface area for tools
		virtual float CollideSurfaceArea( void* pCollide ) = 0;

		// Get the support map for a collide in the given direction
		virtual math::v3f CollideGetExtent( const void* pCollide, const math::v3f& collideOrigin, const math::v3f& collideAngles,
		                                    const math::v3f& direction ) = 0;

		// Get an AABB for an oriented collision model
		virtual void CollideGetAABB( math::v3f* pMins, math::v3f* pMaxs, const void* pCollide, const math::v3f& collideOrigin,
		                             const math::v3f& collideAngles ) = 0;

		virtual void CollideGetMassCenter( void* pCollide, math::v3f* pOutMassCenter )   = 0;
		virtual void CollideSetMassCenter( void* pCollide, const math::v3f& massCenter ) = 0;
		// get the approximate cross-sectional area projected orthographically on the bbox of the collide
		// NOTE: These are fractional areas - unitless.  Basically this is the fraction of the OBB on each axis that
		// would be visible if the object were rendered orthographically.
		// NOTE: This has been precomputed when the collide was built or this function will return 1,1,1
		virtual math::v3f CollideGetOrthographicAreas( const void* pCollide )              = 0;
		virtual void CollideSetOrthographicAreas( void* pCollide, const math::v3f& areas ) = 0;

		// query the vcollide index in the physics model for the instance
		virtual int CollideIndex( const void* pCollide ) = 0;

		// Convert a bbox to a collide
		virtual void* BBoxToCollide( const math::v3f& mins, const math::v3f& maxs )                                      = 0;
		virtual int GetConvexesUsedInCollideable( const void* pCollideable, void** pOutputArray, int iOutputArrayLimit ) = 0;

		// Trace an AABB against a collide
		virtual void pad_1( ) = 0;
		virtual void pad_2( ) = 0;
		virtual void pad_3( ) = 0;

		// Trace one collide against another
		virtual void TraceCollide( const math::v3f& start, const math::v3f& end, const void* pSweepCollide, const math::v3f& sweepAngles,
		                           const void* pCollide, const math::v3f& collideOrigin, const math::v3f& collideAngles, void* ptr ) = 0;

		// relatively slow test for box vs. truncated cone
		virtual bool IsBoxIntersectingCone( const math::v3f& boxAbsMins, const math::v3f& boxAbsMaxs, const truncatedcone_t& cone ) = 0;

		// loads a set of solids into a vcollide_t
		virtual void VCollideLoad( vcollide* pOutput, int solidCount, const char* pBuffer, int size, bool swap = false ) = 0;
		// destroyts the set of solids created by VCollideLoad
		virtual void VCollideUnload( vcollide* pVCollide ) = 0;
	};
} // namespace sdk::interfaces