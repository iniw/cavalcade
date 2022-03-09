#pragma once

namespace sdk {
	struct bone_accessor {
		PAD( 4 );
		math::matrix_3x4* m_bones;
	};
} // namespace sdk