#ifndef DT_COMMON_HPP
#define DT_COMMON_HPP

namespace sdk {
	struct d_variant {
		union {
			f32 m_f32;
			i32 m_i32;
			str m_str;
			unk m_data;
			math::v3f m_v3f;
			i64 m_i64;
		};

		prop_type m_type;
	};
} // namespace sdk

#endif // DT_COMMON_HPP
