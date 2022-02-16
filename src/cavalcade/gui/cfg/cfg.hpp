#pragma once

namespace gui {
	// wonder if there's a better way to do this
	template< typename T >
	concept ConfigEntry = std::is_same_v< T, bool > || std::is_same_v< T, float > || std::is_same_v< T, int > || std::is_same_v< T, uint32_t > ||
		std::is_same_v< T, render::color >;

	struct cfg {
	private:

		using variants = std::variant< bool, float, int, uint32_t, render::color >;

		static inline std::unordered_map< uint32_t, variants > s_entries{ };

	public:

		template< ConfigEntry T >
		static T* add( uint32_t hash ) {
			s_entries.insert( { hash, T( ) } );
			return &get< T >( hash );
		}

		template< ConfigEntry T >
		static T& get( uint32_t hash ) {
			auto& entry = s_entries.at( hash );

			return std::get< T >( entry );
		}
	};
} // namespace gui