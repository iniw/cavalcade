#include "../hooks.hpp"

void AngleMatrix( const math::v3f& angles, math::matrix_3x4& matrix ) {
	auto SinCos = []( float radians, float* sine, float* cosine ) {
		*sine   = sin( radians );
		*cosine = cos( radians );
	};
	constexpr auto DEG2RAD = []( const f32 x ) -> f32 { return x * ( M_PI / 180.F ); };

	float sr, sp, sy, cr, cp, cy;

	SinCos( DEG2RAD( angles[ 1 ] ), &sy, &cy );
	SinCos( DEG2RAD( angles[ 0 ] ), &sp, &cp );
	SinCos( DEG2RAD( angles[ 2 ] ), &sr, &cr );

	// matrix = (YAW * PITCH) * ROLL
	matrix[ 0 ][ 0 ] = cp * cy;
	matrix[ 1 ][ 0 ] = cp * sy;
	matrix[ 2 ][ 0 ] = -sp;

	float crcy       = cr * cy;
	float crsy       = cr * sy;
	float srcy       = sr * cy;
	float srsy       = sr * sy;
	matrix[ 0 ][ 1 ] = sp * srcy - crsy;
	matrix[ 1 ][ 1 ] = sp * srsy + crcy;
	matrix[ 2 ][ 1 ] = sr * cp;

	matrix[ 0 ][ 2 ] = sp * crcy + srsy;
	matrix[ 1 ][ 2 ] = sp * crsy - srcy;
	matrix[ 2 ][ 2 ] = cr * cp;

	matrix[ 0 ][ 3 ] = 0.0f;
	matrix[ 1 ][ 3 ] = 0.0f;
	matrix[ 2 ][ 3 ] = 0.0f;
}
// static float g_vMotionBlurValues[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };

void UpdateScreenEffectTexture( int textureIndex, int x, int y, int w, int h, bool bDestFullScreen = false, Rect_t* pActualRect = NULL ) {
	static auto GetFullFrameFrameBufferTexture =
		g_mem[ CLIENT_DLL ].get_address< sdk::texture*( __thiscall* )( int ) >( HASH_CT( "GetFullFrameFrameBufferTexture" ) );
	Rect_t srcRect;
	srcRect.x      = x;
	srcRect.y      = y;
	srcRect.width  = w;
	srcRect.height = h;

	auto pRenderContext = g_csgo.m_material_system->get_render_context( );
	auto pTexture       = GetFullFrameFrameBufferTexture( textureIndex );
	int nSrcWidth, nSrcHeight;
	pRenderContext->get_render_target_dimensions( nSrcWidth, nSrcHeight );
	int nDestWidth  = pTexture->get_actual_width( );
	int nDestHeight = pTexture->get_actual_height( );

	Rect_t destRect = srcRect;
	if ( !bDestFullScreen && ( nSrcWidth > nDestWidth || nSrcHeight > nDestHeight ) ) {
		// the source and target sizes aren't necessarily the same (specifically in dx7 where
		// nonpow2 rendertargets aren't supported), so lets figure it out here.
		float scaleX    = ( float )nDestWidth / ( float )nSrcWidth;
		float scaleY    = ( float )nDestHeight / ( float )nSrcHeight;
		destRect.x      = srcRect.x * scaleX;
		destRect.y      = srcRect.y * scaleY;
		destRect.width  = srcRect.width * scaleX;
		destRect.height = srcRect.height * scaleY;
		destRect.x      = std::clamp( destRect.x, 0, nDestWidth );
		destRect.y      = std::clamp( destRect.y, 0, nDestHeight );
		destRect.width  = std::clamp( destRect.width, 0, nDestWidth - destRect.x );
		destRect.height = std::clamp( destRect.height, 0, nDestHeight - destRect.y );
	}

	pRenderContext->copy_Render_target_to_texture_ex( pTexture, 0, &srcRect, bDestFullScreen ? NULL : &destRect );
	pRenderContext->set_frame_buffer_copy_texture( pTexture, textureIndex );

	if ( pActualRect ) {
		pActualRect->x      = destRect.x;
		pActualRect->y      = destRect.y;
		pActualRect->width  = destRect.width;
		pActualRect->height = destRect.height;
	}
}

// swoopae i know you're here you better fuck off
static void do_motionblur( ) {
	static auto address        = g_mem[ CLIENT_DLL ].get_address< uintptr_t >( HASH_CT( "g_pWorld" ) ) + 1;
	auto world                 = **( sdk::base_entity*** )( address );
	static auto mbaddress      = g_mem[ CLIENT_DLL ].get_address< uintptr_t >( HASH_CT( "g_MotionBlurValues" ) ) + 1;
	float* g_vMotionBlurValues = *( float** )mbaddress;

	// g_io.log( "{:x}", ( uintptr_t )world );
	//======================================================================================================//
	// Get these convars here to make it easier to remove them later and to default each client differently //
	//======================================================================================================//
	float flMotionBlurRotationIntensity = 1.0 * 0.15f; // The default is to not blur past 15% of the range
	float flMotionBlurRollIntensity     = 0.3f; // * mat_motion_blur_roll_intensity.GetFloat(); // The default is to not blur past 30% of the range
	float flMotionBlurFallingIntensity  = 1.0;
	float flMotionBlurFallingMin        = 10.0;
	float flMotionBlurFallingMax        = 20.0;
	float flMotionBlurGlobalStrength    = 1.0;

	if ( true ) {
		//=====================//
		// Previous frame data //
		//=====================//
		static float s_flLastTimeUpdate      = 0.0f;
		static float s_flPreviousPitch       = 0.0f;
		static float s_flPreviousYaw         = 0.0f;
		static float s_vPreviousPositon[ 3 ] = { 0.0f, 0.0f, 0.0f };
		static math::matrix_3x4 s_mPreviousFrameBasisVectors;
		static float s_flNoRotationalMotionBlurUntil = 0.0f;
		// float vPreviousSideVec[3] = { s_mPreviousFrameBasisVectors[0][1], s_mPreviousFrameBasisVectors[1][1], s_mPreviousFrameBasisVectors[2][1] };
		// float vPreviousForwardVec[3] = { s_mPreviousFrameBasisVectors[0][0], s_mPreviousFrameBasisVectors[1][0], s_mPreviousFrameBasisVectors[2][0]
		// }; float vPreviousUpVec[3] = { s_mPreviousFrameBasisVectors[0][2], s_mPreviousFrameBasisVectors[1][2], s_mPreviousFrameBasisVectors[2][2]
		// };

		float flTimeElapsed = g_csgo.m_globals->m_realtime - s_flLastTimeUpdate;

		//===================================//
		// Get current pitch & wrap to +-180 //
		//===================================//
		float flCurrentPitch = g_ctx.m_view.angles[ 0 ];
		while ( flCurrentPitch > 180.0f )
			flCurrentPitch -= 360.0f;
		while ( flCurrentPitch < -180.0f )
			flCurrentPitch += 360.0f;

		//=================================//
		// Get current yaw & wrap to +-180 //
		//=================================//
		float flCurrentYaw = g_ctx.m_view.angles[ 1 ];
		while ( flCurrentYaw > 180.0f )
			flCurrentYaw -= 360.0f;
		while ( flCurrentYaw < -180.0f )
			flCurrentYaw += 360.0f;

		//===========================//
		// Get current basis vectors //
		//===========================//
		math::matrix_3x4 mCurrentBasisVectors;
		AngleMatrix( g_ctx.m_view.angles, mCurrentBasisVectors );

		float vCurrentSideVec[ 3 ]    = { mCurrentBasisVectors[ 0 ][ 1 ], mCurrentBasisVectors[ 1 ][ 1 ], mCurrentBasisVectors[ 2 ][ 1 ] };
		float vCurrentForwardVec[ 3 ] = { mCurrentBasisVectors[ 0 ][ 0 ], mCurrentBasisVectors[ 1 ][ 0 ], mCurrentBasisVectors[ 2 ][ 0 ] };
		// float vCurrentUpVec[3] = { mCurrentBasisVectors[0][2], mCurrentBasisVectors[1][2], mCurrentBasisVectors[2][2] };

		//======================//
		// Get current position //
		//======================//
		float vCurrentPosition[ 3 ] = { g_ctx.m_view.origin[ 0 ], g_ctx.m_view.origin[ 1 ], g_ctx.m_view.origin[ 2 ] };

		//===============================================================//
		// Evaluate change in position to determine if we need to update //
		//===============================================================//
		math::v3f vPositionChange = math::v3f( s_vPreviousPositon[ 0 ], s_vPreviousPositon[ 1 ], s_vPreviousPositon[ 2 ] ) -
		                            math::v3f( vCurrentPosition[ 0 ], vCurrentPosition[ 1 ], vCurrentPosition[ 2 ] );

		if ( ( vPositionChange.length( ) > 30.0f ) && ( flTimeElapsed >= 0.5f ) ) {
			//=======================================================//
			// If we moved a far distance in one frame and more than //
			// half a second elapsed, disable motion blur this frame //
			//=======================================================//
			// engine->Con_NPrintf( 8, " Pos change && time > 0.5 seconds %f ", gpGlobals->realtime );

			g_vMotionBlurValues[ 0 ] = 0.0f;
			g_vMotionBlurValues[ 1 ] = 0.0f;
			g_vMotionBlurValues[ 2 ] = 0.0f;
			g_vMotionBlurValues[ 3 ] = 0.0f;
		} else if ( flTimeElapsed > ( 1.0f / 15.0f ) ) {
			//==========================================//
			// If slower than 15 fps, don't motion blur //
			//==========================================//
			g_vMotionBlurValues[ 0 ] = 0.0f;
			g_vMotionBlurValues[ 1 ] = 0.0f;
			g_vMotionBlurValues[ 2 ] = 0.0f;
			g_vMotionBlurValues[ 3 ] = 0.0f;
		} else if ( vPositionChange.length( ) > 50.0f ) {
			//================================================================================//
			// We moved a far distance in a frame, use the same motion blur as last frame	  //
			// because I think we just went through a portal (should we ifdef this behavior?) //
			//================================================================================//
			// engine->Con_NPrintf( 8, " Position changed %f units @ %.2f time ", VectorLength( vPositionChange ), gpGlobals->realtime );

			s_flNoRotationalMotionBlurUntil = g_csgo.m_globals->m_realtime + 1.0f; // Wait a second until the portal craziness calms down
		} else {
			//====================//
			// Normal update path //
			//====================//
			// Compute horizontal and vertical fov
			float flHorizontalFov = g_ctx.m_view.fov;
			float flVerticalFov   = ( g_ctx.m_view.aspect_ratio <= 0.0f ) ? ( g_ctx.m_view.fov ) : ( g_ctx.m_view.fov / g_ctx.m_view.aspect_ratio );
			// engine->Con_NPrintf( 2, "Horizontal Fov: %6.2f   Vertical Fov: %6.2f", flHorizontalFov, flVerticalFov );

			//=====================//
			// Forward motion blur //
			//=====================//
			float flViewDotMotion =
				math::v3f( vCurrentForwardVec[ 0 ], vCurrentForwardVec[ 1 ], vCurrentForwardVec[ 2 ] ).dot_product( vPositionChange );
			// if ( mat_motion_blur_forward_enabled.GetBool( ) ) // Want forward and falling
			g_vMotionBlurValues[ 2 ] = flViewDotMotion;
			// else                                                                              // Falling only
			// g_vMotionBlurValues[ 2 ] = flViewDotMotion * fabs( vCurrentForwardVec[ 2 ] ); // Only want this if we're looking up or down;

			//====================================//
			// Yaw (Compensate for circle strafe) //
			//====================================//
			float flSideDotMotion   = math::v3f( vCurrentSideVec[ 0 ], vCurrentSideVec[ 1 ], vCurrentSideVec[ 2 ] ).dot_product( vPositionChange );
			float flYawDiffOriginal = s_flPreviousYaw - flCurrentYaw;
			if ( ( ( s_flPreviousYaw - flCurrentYaw > 180.0f ) || ( s_flPreviousYaw - flCurrentYaw < -180.0f ) ) &&
			     ( ( s_flPreviousYaw + flCurrentYaw > -180.0f ) && ( s_flPreviousYaw + flCurrentYaw < 180.0f ) ) )
				flYawDiffOriginal = s_flPreviousYaw + flCurrentYaw;

			float flYawDiffAdjusted = flYawDiffOriginal + ( flSideDotMotion / 3.0f ); // Yes, 3.0 is a magic number, sue me

			// Make sure the adjustment only lessens the effect, not magnify it or reverse it
			if ( flYawDiffOriginal < 0.0f )
				flYawDiffAdjusted = std::clamp( flYawDiffAdjusted, flYawDiffOriginal, 0.0f );
			else
				flYawDiffAdjusted = std::clamp( flYawDiffAdjusted, 0.0f, flYawDiffOriginal );

			// Use pitch to dampen yaw
			float flUndampenedYaw    = flYawDiffAdjusted / flHorizontalFov;
			g_vMotionBlurValues[ 0 ] = flUndampenedYaw * ( 1.0f - ( fabs( flCurrentPitch ) / 90.0f ) ); // Dampen horizontal yaw blur based on pitch

			// engine->Con_NPrintf( 4, "flSideDotMotion: %6.2f   yaw diff: %6.2f  ( %6.2f, %6.2f )", flSideDotMotion, ( s_flPreviousYaw - flCurrentYaw
			// ), flYawDiffOriginal, flYawDiffAdjusted );

			//=======================================//
			// Pitch (Compensate for forward motion) //
			//=======================================//
			float flPitchCompensateMask = 1.0f - ( ( 1.0f - fabs( vCurrentForwardVec[ 2 ] ) ) * ( 1.0f - fabs( vCurrentForwardVec[ 2 ] ) ) );
			float flPitchDiffOriginal   = s_flPreviousPitch - flCurrentPitch;
			float flPitchDiffAdjusted   = flPitchDiffOriginal;

			if ( flCurrentPitch > 0.0f )
				flPitchDiffAdjusted =
					flPitchDiffOriginal - ( ( flViewDotMotion / 2.0f ) * flPitchCompensateMask ); // Yes, 2.0 is a magic number, sue me
			else
				flPitchDiffAdjusted =
					flPitchDiffOriginal + ( ( flViewDotMotion / 2.0f ) * flPitchCompensateMask ); // Yes, 2.0 is a magic number, sue me

			// Make sure the adjustment only lessens the effect, not magnify it or reverse it
			if ( flPitchDiffOriginal < 0.0f )
				flPitchDiffAdjusted = std::clamp( flPitchDiffAdjusted, flPitchDiffOriginal, 0.0f );
			else
				flPitchDiffAdjusted = std::clamp( flPitchDiffAdjusted, 0.0f, flPitchDiffOriginal );

			g_vMotionBlurValues[ 1 ] = flPitchDiffAdjusted / flVerticalFov;

			// engine->Con_NPrintf( 5, "flViewDotMotion %6.2f, flPitchCompensateMask %6.2f, flPitchDiffOriginal %6.2f, flPitchDiffAdjusted %6.2f,
			// g_vMotionBlurValues[1] %6.2f", flViewDotMotion, flPitchCompensateMask, flPitchDiffOriginal, flPitchDiffAdjusted,
			// g_vMotionBlurValues[1]);

			//========================================================//
			// Roll (Enabled when we're looking down and yaw changes) //
			//========================================================//
			g_vMotionBlurValues[ 3 ] = flUndampenedYaw; // Roll starts out as undampened yaw intensity and is then scaled by pitch
			g_vMotionBlurValues[ 3 ] *= ( fabs( flCurrentPitch ) / 90.0f ) * ( fabs( flCurrentPitch ) / 90.0f ) *
			                            ( fabs( flCurrentPitch ) / 90.0f ); // Dampen roll based on pitch^3

			// engine->Con_NPrintf( 4, "[2] before scale and bias: %6.2f", g_vMotionBlurValues[2] );
			// engine->Con_NPrintf( 5, "[3] before scale and bias: %6.2f", g_vMotionBlurValues[3] );

			//==============================================================//
			// Time-adjust falling effect until we can do something smarter //
			//==============================================================//
			if ( flTimeElapsed > 0.0f )
				g_vMotionBlurValues[ 2 ] /= flTimeElapsed * 30.0f; // 1/30th of a second?
			else
				g_vMotionBlurValues[ 2 ] = 0.0f;

			// Scale and bias values after time adjustment
			g_vMotionBlurValues[ 2 ] =
				std::clamp( ( fabs( g_vMotionBlurValues[ 2 ] ) - flMotionBlurFallingMin ) / ( flMotionBlurFallingMax - flMotionBlurFallingMin ), 0.0f,
			                1.0f ) *
				( g_vMotionBlurValues[ 2 ] >= 0.0f ? 1.0f : -1.0f );
			g_vMotionBlurValues[ 2 ] /= 30.0f; // To counter-adjust for time adjustment above

			//=================//
			// Apply intensity //
			//=================//
			g_vMotionBlurValues[ 0 ] *= flMotionBlurRotationIntensity * flMotionBlurGlobalStrength;
			g_vMotionBlurValues[ 1 ] *= flMotionBlurRotationIntensity * flMotionBlurGlobalStrength;
			g_vMotionBlurValues[ 2 ] *= flMotionBlurFallingIntensity * flMotionBlurGlobalStrength;
			g_vMotionBlurValues[ 3 ] *= flMotionBlurRollIntensity * flMotionBlurGlobalStrength;

			//===============================================================//
			// Dampen motion blur from 100%-0% as fps drops from 50fps-30fps //
			//===============================================================//
			// if ( !IsX360( ) ) // I'm not doing this on the 360 yet since I can't test it
			{
				float flSlowFps      = 30.0f;
				float flFastFps      = 50.0f;
				float flCurrentFps   = ( flTimeElapsed > 0.0f ) ? ( 1.0f / flTimeElapsed ) : 0.0f;
				float flDampenFactor = std::clamp( ( ( flCurrentFps - flSlowFps ) / ( flFastFps - flSlowFps ) ), 0.0f, 1.0f );

				// engine->Con_NPrintf( 4, "gpGlobals->realtime %.2f  gpGlobals->curtime %.2f", gpGlobals->realtime, gpGlobals->curtime );
				// engine->Con_NPrintf( 5, "flCurrentFps %.2f", flCurrentFps );
				// engine->Con_NPrintf( 7, "flTimeElapsed %.2f", flTimeElapsed );

				g_vMotionBlurValues[ 0 ] *= flDampenFactor;
				g_vMotionBlurValues[ 1 ] *= flDampenFactor;
				g_vMotionBlurValues[ 2 ] *= flDampenFactor;
				g_vMotionBlurValues[ 3 ] *= flDampenFactor;

				// engine->Con_NPrintf( 6, "Dampen: %.2f", flDampenFactor );
			}

			// engine->Con_NPrintf( 6, "Final values: { %6.2f%%, %6.2f%%, %6.2f%%, %6.2f%% }", g_vMotionBlurValues[0]*100.0f,
			// g_vMotionBlurValues[1]*100.0f, g_vMotionBlurValues[2]*100.0f, g_vMotionBlurValues[3]*100.0f );
		}

		//============================================//
		// Zero out blur if still in that time window //
		//============================================//
		if ( g_csgo.m_globals->m_realtime < s_flNoRotationalMotionBlurUntil ) {
			// engine->Con_NPrintf( 9, " No Rotation @ %f ", gpGlobals->realtime );

			// Zero out rotational blur but leave forward/falling blur alone
			g_vMotionBlurValues[ 0 ] = 0.0f; // X
			g_vMotionBlurValues[ 1 ] = 0.0f; // Y
			g_vMotionBlurValues[ 3 ] = 0.0f; // Roll
		} else {
			s_flNoRotationalMotionBlurUntil = 0.0f;
		}

		//====================================//
		// Store current frame for next frame //
		//====================================//
		s_vPreviousPositon[ 0 ]      = vCurrentPosition[ 0 ];
		s_vPreviousPositon[ 1 ]      = vCurrentPosition[ 1 ];
		s_vPreviousPositon[ 2 ]      = vCurrentPosition[ 2 ];
		s_mPreviousFrameBasisVectors = mCurrentBasisVectors;
		s_flPreviousPitch            = flCurrentPitch;
		s_flPreviousYaw              = flCurrentYaw;
		s_flLastTimeUpdate           = g_csgo.m_globals->m_realtime;
	}

	// static auto UpdateScreenEffectTexture = g_mem[ CLIENT_DLL ].get_address< uintptr_t >( HASH_CT( "UpdateScreenTexture" ) );
	// static auto DumpTGAofRenderTarget = g_mem[ CLIENT_DLL ].get_address< void( __fastcall* )( int, int ) >( HASH_CT( "DumpTGAofRenderTarget" ) );

	if ( true ) {
		auto pRenderContext = g_csgo.m_material_system->get_render_context( );
		auto pSrc           = g_csgo.m_material_system->find_texture( "_rt_FullFrameFB", "RenderTargets" );
		int nSrcWidth       = pSrc->get_actual_width( );
		int nSrcHeight      = pSrc->get_actual_height( );
		int dest_width, dest_height, nDummy;
		pRenderContext->get_viewport( nDummy, nDummy, dest_width, dest_height );

		// if ( g_pMaterialSystemHardwareConfig->GetHDRType( ) != HDR_TYPE_FLOAT ) {
		// UpdateScreenEffectTexture( 0, g_ctx.m_view.x, g_ctx.m_view.y, g_ctx.m_view.w, g_ctx.m_view.h, true,
		//                            nullptr ); // Do we need to check if we already did this?
		// }
		UpdateScreenEffectTexture( 0, g_ctx.m_view.x, g_ctx.m_view.y, g_ctx.m_view.w, g_ctx.m_view.h, true, nullptr );

		// Get material pointer
		auto pMatMotionBlur = g_csgo.m_material_system->find_material( "dev/motion_blur", "Other textures", true );

		// SetRenderTargetAndViewPort( dest_rt0 );
		// pRenderContext->PopRenderTargetAndViewport();

		// swoopae i know you're here you better fuck off

		if ( pMatMotionBlur != NULL ) {
			pRenderContext->draw_screen_space_rectangle( pMatMotionBlur, 0, 0, dest_width, dest_height, 0, 0, nSrcWidth - 1, nSrcHeight - 1,
			                                             nSrcWidth, nSrcHeight, world->get_client_renderable( ) );

			// if ( g_bDumpRenderTargets ) {
			// 	DumpTGAofRenderTarget( dest_width, dest_height, "MotionBlur" );
			// DumpTGAofRenderTarget( dest_width, dest_height );
			// }
		}
	}
}

void cavalcade::hooks::view_render::render_view( unk ecx, unk edx, sdk::view& view, sdk::view& hudview, int a, int b ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< render_view_fn >( HASH_CT( "RenderView" ) );
	// cope, seethe
	g_ctx.m_view = view;
	og( ecx, edx, view, hudview, a, b );
}

int cavalcade::hooks::view_render::gay_function( ) {
	static auto og = g_mem[ CLIENT_DLL ].get_og< gay_function_fn >( HASH_CT( "GayFunction" ) );
	auto f         = og( );
	do_motionblur( );
	return f;
}