#pragma once

namespace gui {
	struct cfg {
	private:

		using types = std::variant< bool, f32, i32 >;

		static inline std::unordered_map< u32, types > s_entries{ };

	public:

		// TODO(wini): constraint T to types held by "types"
		template< typename T >
		static T* add_entry( u32 hash ) {
			s_entries.insert( { hash, T( ) } );
			return &get< T >( hash );
		}

		template< typename T >
		static T& get( u32 hash ) {
			return std::get< T >( s_entries.at( hash ) );
		}
	};
} // namespace gui