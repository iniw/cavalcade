#ifndef DATA_MAP_HPP
#define DATA_MAP_HPP

namespace sdk {
	struct data_map;

	enum
	{
		TD_OFFSET_NORMAL = 0,
		TD_OFFSET_PACKED = 1,
		TD_OFFSET_COUNT
	};

	struct type_description {
		field_type m_field_type;
		cstr m_field_name;
		i32 m_field_offset[ TD_OFFSET_COUNT ];
		u8 m_field_size;
		i8 m_flags;

		PAD( 0xC );

		data_map* m_type_description;

		PAD( 0x18 );
	};

	struct data_map {
		type_description* m_data_desc;
		i32 m_data_fields;
		cstr m_data_class_name;
		data_map* m_base_map;
		bool m_chains_validated;
		bool m_packed_offsets_computed;
		i32 m_packed_size;
	};
} // namespace sdk

#endif // DATA_MAP_HPP