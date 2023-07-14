#pragma once

namespace editor
{
	namespace detail
	{
		template <class T, class... Args>
		constexpr bool is_in_tuple(const std::tuple<Args...>& t);

		template <class T, std::size_t N, class... Args>
		struct get_number_of_element_from_tuple_by_type_impl
		{
			static constexpr auto value = N;
		};

		template <class T, std::size_t N, class... Args>
		struct get_number_of_element_from_tuple_by_type_impl<T, N, T, Args...>
		{
			static constexpr auto value = N;
		};

		//template <class T, std::size_t N, class U, class... Args>
		//requires (std::is_base_of_v<T, U>)
		//struct get_number_of_element_from_tuple_by_type_impl<T, N, U, Args...>
		//{
		//	static constexpr auto value = N;
		//};

		template <class T, std::size_t N, class U, class... Args>
		//requires (!std::is_base_of_v<T, U>)
		struct get_number_of_element_from_tuple_by_type_impl<T, N, U, Args...>
		{
			static constexpr auto value = get_number_of_element_from_tuple_by_type_impl<T, N + 1, Args...>::value;
		};

		template <class T, std::size_t N, class... Args>
		struct _is_in_tuple
		{
			static constexpr auto value = false;
		};

		template <class T, std::size_t N, class... Args>
		struct _is_in_tuple<T, N, T, Args...>
		{
			static constexpr auto value = true;
		};

		template <class T, std::size_t N, class U, class... Args>
		struct _is_in_tuple<T, N, U, Args...>
		{
			static constexpr auto value = /*std::is_base_of_v<T, U> ||*/ is_in_tuple<T, N + 1, Args...>::value;
		};

		template <class T, class... Args>
		const T get_element_by_type(const std::tuple<Args...>& t)
		{
			return std::get<detail::get_number_of_element_from_tuple_by_type_impl<T, 0, Args...>::value>(t);
		}

		template <class T, class... Args>
		constexpr bool is_in_tuple(const std::tuple<Args...>& t)
		{
			return _is_in_tuple<T, 0, Args...>::value;
		}
	}

	template<auto... Props>
	class property_element
	{
		constexpr static auto properties = std::make_tuple(Props...);

	public:
		template<typename T>
		static inline const T get_property()
		{
			if constexpr (detail::is_in_tuple<T>(properties))
			{
				return detail::get_element_by_type<T>(properties);
			}
			else
			{
				return T();
			}
		}
	};
}