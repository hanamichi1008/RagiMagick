#pragma once
#include <type_traits>

namespace ragii {


	template<class T>
	struct is_char :
		std::bool_constant<
		std::is_same_v<T, char>
		|| std::is_same_v<T, char16_t>
		|| std::is_same_v<T, char32_t>
		>
	{
	};

	template<class T>
	constexpr bool is_char_v = is_char<T>::value;


}  // namespace ragii
