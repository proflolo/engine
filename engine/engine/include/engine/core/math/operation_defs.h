#pragma once
#include <utility>

namespace engine::math
{
	template<typename T>
	concept arithmetic = std::integral<T> or std::floating_point<T>;

	namespace mult
	{
		template<typename T1, typename T2>
		using type = decltype(std::declval<T1>() * std::declval<T2>());

		template<typename T1, typename T2>
		concept constraint = requires(T1 i_left, T2 i_right)
		{
			i_left * i_right;
		};
	}

	namespace div
	{
		template<typename T1, typename T2>
		using type = decltype(std::declval<T1>()/ std::declval<T2>());

		template<typename T1, typename T2>
		concept constraint = requires(T1 i_left, T2 i_right)
		{
			i_left / i_right;
		};
	}

	namespace sum
	{
		template<typename T1, typename T2>
		using type = decltype(std::declval<T1>() + std::declval<T2>());

		template<typename T1, typename T2>
		concept constraint = requires(T1 i_left, T2 i_right)
		{
			i_left + i_right;
		};
	}

	namespace diff
	{
		template<typename T1, typename T2>
		using type = decltype(std::declval<T1>() - std::declval<T2>());

		template<typename T1, typename T2>
		concept constraint = requires(T1 i_left, T2 i_right)
		{
			i_left - i_right;
		};
	}
}