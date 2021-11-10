#pragma once

namespace gui {
	// wonder if there's a better way to do this
	template< typename T >
	concept ConfigEntry = std::is_same_v< T, bool > || std::is_same_v< T, f32 > || std::is_same_v< T, i32 >;

	struct cfg {
	private:

		using types = std::variant< bool, f32, i32 >;

		static inline std::unordered_map< u32, types > s_entries{ };

	public:

		template< ConfigEntry T >
		static T* add_entry( u32 hash ) {
			s_entries.insert( { hash, T( ) } );
			return &get< T >( hash );
		}

		template< ConfigEntry T >
		static T& get( u32 hash ) {
			ENFORCE( s_entries.contains( hash ), "bad hash given to cfg::get, hash: {}", hash );

			auto& entry = s_entries.at( hash );

			ENFORCE( std::holds_alternative< T >( entry ), "bad type given to cfg::get, hash: {}", hash );

			return std::get< T >( entry );
		}
	};
} // namespace gui