#pragma once
#include <type_traits>
#include <string>
#include "../type/type_traits.h"


namespace ragii { namespace text {

	// 数字か？
	template<
		class C,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr bool is_digit(C c)
	{
		return c >= C('0') && c <= C('9');
	}

	// 英小文字か？
	template<
		class C,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr bool is_lower_letter(C c)
	{
		return c >= C('a') && c <= C('z');
	}

	// 英大文字か？
	template<
		class C,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr bool is_upper_letter(C c)
	{
		return c >= C('A') && c <= C('Z');
	}

	// 英字か？
	template<
		class C,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr bool is_letter(C c)
	{
		return is_lower_letter(c) || is_upper_letter(c);
	}

	// 英小文字化
	template<
		class C,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr C to_lower(C c)
	{
		return is_upper_letter(c) ? C(c + ('a' - 'A')) : c;
	}

	// 英大文字化
	template<
		class C,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr C to_upper(C c)
	{
		return is_lower_letter(c) ? C(c - ('a' - 'A')) : c;
	}

	// 文字型から算術型へ変換
	template<
		class T,
		T Fallback = T(),
		class C,
		typename std::enable_if<std::is_arithmetic_v<T>, std::nullptr_t>::type = nullptr,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr T char_to_digit(C c)
	{
		return T(is_digit<C>(c) ? c - C('0') : Fallback);
	}

	// 算術型から文字型へ変換
	template<
		class C,
		C Fallback = C(),
		class T,
		typename std::enable_if<std::is_arithmetic_v<T>, std::nullptr_t>::type = nullptr,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr C digit_to_char(T t)
	{
		return t >= 0 && t <= 9 ? C(t + C('0')) : Fallback;
	}

	// 文字列型から算術型へ変換
	template<
		class T,
		class C,
		T Fallback = T(),
		typename std::enable_if<std::is_arithmetic_v<T>, std::nullptr_t>::type = nullptr,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr T str_to_arithmetic(const C* s)
	{
		T result = T();
		while (true) {
			C c = *s;
			if (c == C('\0')) {
				break;
			}
			result = result * T(10) + char_to_digit<T>(c);
			++s;
		}
		return result;
	}

	// 算術型から文字列型へ変換
	template<
		class C,
		class T,
		size_t C_SZ = sizeof(C),
		size_t T_SZ = sizeof(T),
		typename std::enable_if<std::is_arithmetic_v<T>, std::nullptr_t>::type = nullptr,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr void arithmetic_to_str(T t, C (&result)[T_SZ + 1])
	{
		C* p = result;
		T tmp = t;
		for (size_t i = 0; i < T_SZ; i++) {
			p[i] = tmp & 0xff;
			tmp >>= (C_SZ * 8);
		}
	}

	template<
		class C,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr const C* begin(const C* s)
	{
		return s;
	}

	template<
		class C,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr const C* end(const C* s)
	{
		size_t i = 0;
		auto p = begin<C>(s);
		while (*p != C('\0')) {
			if (*p != *begin<C>(s)) {
				++i;
			}
			++p;
		}
		++i;
		return s + i;
	}

	template<
		class C,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr bool is_empty(const C* s)
	{
		return *s == C('\0');
	}

	// 文字列長を取得
	template<
		class C,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr size_t length(const C* s)
	{
		return end<C>(s) - begin<C>(s);
	}

	// 文字列の先頭を比較
	template<
		class C,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr bool starts_with(const C* base, const C* search)
	{
		const C null = C('\0');
		while (*base != null && *search != null) {
			if (*base != *search) {
				return false;
			}
			base++;
			search++;
		}
		return true;
	}

	// 文字列の末尾を比較
	template<
		class C,
		typename std::enable_if<ragii::is_char_v<C>, std::nullptr_t>::type = nullptr
	>
	constexpr bool ends_with(const C* base, const C* search)
	{
		const size_t base_len = length<C>(base);
		const size_t search_len = length<C>(search);

		if (base_len < search_len) {
			return false;
		}

		const C* base_begin = end<C>(base) - search_len;
		const bool result = starts_with<C>(base_begin, search);

		return result;
	}


}  // namespace text
 }  // namespace ragii
