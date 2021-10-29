#ifndef DT_RECV_HPP
#define DT_RECV_HPP

#include "dt_common.hpp"

namespace sdk {
	struct recv_table;
	struct recv_prop;

	struct recv_proxy_data {
		const recv_prop* m_recv_prop;
		d_variant m_value;
		i32 m_element;
		i32 m_object_id;
	};

	using array_length_recv_proxy_fn   = void( __cdecl* )( unk, i32, i32 );
	using data_table_recv_var_proxy_fn = void( __cdecl* )( recv_prop*, unk*, unk, i32 );
	using recv_var_proxy_fn            = void( __cdecl* )( recv_proxy_data*, unk, unk );

	struct std_recv_proxies {
		recv_var_proxy_fn int32_to_int8;
		recv_var_proxy_fn int32_to_int16;
		recv_var_proxy_fn int32_to_int32;
		recv_var_proxy_fn int64_to_int64;
		recv_var_proxy_fn float_to_float;
		recv_var_proxy_fn vector_to_vector;
	};

	struct recv_prop {
		str m_var_name;
		prop_type m_recv_type;
		i32 m_flags;
		i32 m_string_buffer_size;
		bool m_inside_array;
		unk m_extra_data;
		recv_prop* m_array_prop;
		array_length_recv_proxy_fn m_array_length_proxy;
		recv_var_proxy_fn m_proxy_fn;
		data_table_recv_var_proxy_fn m_data_table_proxy_function;
		recv_table* m_data_table;
		i32 m_offset;
		i32 m_element_stride;
		i32 m_elements;
		cstr m_parent_array_prop_name;
	};

	struct recv_table {
		recv_prop* m_props_ptr;
		i32 m_props;
		unk m_decoder;
		str m_net_table_name;
		bool m_initialized;
		bool m_in_main_list;
	};
} // namespace sdk

#endif // DT_RECV_HPP
