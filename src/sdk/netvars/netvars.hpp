#ifndef NETVARS_HPP
#define NETVARS_HPP

namespace sdk {

	struct data_map;
	struct recv_table;
	struct recv_prop;

	struct netvars {
	private:

		std::ofstream m_file;
		std::unordered_map< u32, ptr > m_offsets;

	public:

		bool init( );

		ptr get( u32 netvar_hash );

		ptr find_in_datamap( const data_map* map, u32 netvar_hash );

	private:

		void store( std::string_view table_name, const recv_table* table, ptrdiff offset = 0u, i32 depth = 0 );

		std::string get_type( const recv_prop* prop );
	};
} // namespace sdk

inline sdk::netvars g_netvars;

#endif // NETVARS_HPP