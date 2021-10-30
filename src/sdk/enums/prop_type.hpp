#ifndef PROP_TYPE_HPP
#define PROP_TYPE_HPP

namespace sdk {
	enum class prop_type : i32
	{
		INT = 0,
		FLOAT,
		VECTOR,
		VECTOR2D,
		STRING,
		ARRAY,
		DATATABLE,
		INT64,
		SENDPROPTYPEMAX
	};
} // namespace sdk

#endif // PROP_TYPE_HPP