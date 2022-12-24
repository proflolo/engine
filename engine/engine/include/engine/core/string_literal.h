#pragma once

namespace engine
{
	struct string_literal
	{
		constexpr string_literal()
		{

		}

		template<size_t N>
		requires (N <= 16)
		constexpr string_literal(const char(&str)[N])
		{
			std::copy_n(str, N, value);
		}


		char value[16]{};
		

		inline operator std::string() const
		{
			return std::string(value);
		}
	};
}