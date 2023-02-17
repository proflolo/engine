#pragma once

namespace engine
{
	namespace detail
	{
		struct universal
		{
			template<typename T>
			operator T() {};
		};
	}

	template <typename T>
	consteval auto member_count(auto... Members)
	{
		if constexpr (requires { T{ Members... }; } == false)
		{
			return sizeof...(Members) - 1;
		}
		else
		{
			return member_count<T>(Members..., detail::universal{});
		}
	}

	template<typename T, typename Func>
	void for_each_member(const T& i_t, const Func i_func)
	{
		constexpr auto count = member_count<T>();
		static_assert(count <= 3);
		if constexpr (count == 3)
		{
			const auto& [a, b, c] = i_t;
			i_func(a);
			i_func(b);
			i_func(c);
		}
		else if constexpr (count == 2)
		{
			const auto& [a, b] = i_t;
			i_func(a);
			i_func(b);
		}
		else if constexpr (count == 1)
		{
			const auto& [a] = i_t;
			i_func(a);
		}
		else if constexpr (count == 0)
		{
			
		}
	}
}