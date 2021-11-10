#pragma once

namespace gui {
	struct cfg {
	private:

		static inline std::unordered_map< u32, void* > s_entries{ };

	public:

		static void add_entry( u32 hash, void* ptr ) {
			s_entries[ hash ] = ptr;
		}

		template< typename T >
		static T& get( u32 hash ) {
			return *static_cast< T* >( s_entries.at( hash ) );
		}
	};
} // namespace gui