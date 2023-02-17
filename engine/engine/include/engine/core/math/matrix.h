#pragma once
#include <array>
#include "operation_defs.h"

namespace engine::math
{
	namespace detail
	{
		template<typename T, int Dim>
		class vector_data
		{
		private:
			std::array<T, Dim> m_values;

		public:
			vector_data()
			{

			}

			template<typename... Params>
			vector_data(Params... i_params)
				: m_values({ i_params... })
			{

			}

			const T& get(int i_dimension) const
			{
				return m_values[i_dimension];
			}

			T& get(int i_dimension)
			{
				return m_values[i_dimension];
			}
		};

		template<typename T>
		class vector_data<T, 2>
		{
		public:
			T x;
			T y;

		public:
			vector_data()
			{

			}

			vector_data(const T& i_x, const T& i_y)
				: x(i_x)
				, y(i_y)
			{

			}

			const T& get(int i_dimension) const
			{
				switch (i_dimension)
				{
				case 0:
					return x;
				default:
					return y;
				}
			}

			T& get(int i_dimension)
			{
				switch (i_dimension)
				{
				case 0:
					return x;
				default:
					return y;
				}
			}
		};

		template<typename T>
		class vector_data<T, 3>
		{
		public:
			T x;
			T y;
			T z;

		public:
			vector_data()
			{

			}

			vector_data(const T& i_x, const T& i_y, const T& i_z)
				: x(i_x)
				, y(i_y)
				, z(i_z)
			{

			}

			const T& get(int i_dimension) const
			{
				switch (i_dimension)
				{
				case 0:
					return x;
				case 1:
					return y;
				case 2:
				default:
					return z;
				}
			}

			T& get(int i_dimension)
			{
				switch (i_dimension)
				{
				case 0:
					return x;
				case 1:
					return y;
				case 2:
				default:
					return z;
				}
			}

			
		};
	}

	
	template<typename T, int Dim>
	class vector: public detail::vector_data<T, Dim>
	{

	public:
		vector()
		{

		}

		template<typename... Params>
		requires (sizeof...(Params) == Dim)
		vector(Params... i_params)
			: detail::vector_data<T, Dim>(i_params...)
		{
			//C++ 14 osea anticuado
			//static_assert(sizeof...(i_params) == Dim, "El numero de parametros no encaja");
		}

		const T& operator[] (int i_dimension) const
		{
			return this->get(i_dimension);
		}

		T& operator[] (int i_dimension)
		{
			return this->get(i_dimension);
		}

		template<typename T2>
		requires sum::constraint<T, T2>
		auto operator+ (const vector<T2, Dim>& i_other) const
		{
			using R = sum::type<T, T2>;
			vector<R, Dim> result;
			for (int i = 0; i < Dim; i++)
			{
				result[i] = this->get(i) + i_other[i];
			}
			return result;
		}

		template<typename T2>
		requires std::is_same<sum::type<T, T2>, T>::value
		void operator+= (const vector<T2, Dim>& i_other)
		{
			for (int i = 0; i < Dim; i++)
			{
				(*this)[i] = this->get(i) + i_other[i];
			}
		}

		template<typename T2>
		requires diff::constraint<T, T2>
		auto operator- (const vector<T2, Dim>& i_other) const
		{
			using R = diff::type<T, T2>;
			vector<R, Dim> result;
			for (int i = 0; i < Dim; i++)
			{
				result[i] = this->get(i) - i_other[i];
			}
			return result;
		}

		auto operator- () const
		{
			vector<T, Dim> result;
			for (int i = 0; i < Dim; i++)
			{
				result[i] = -this->get(i);
			}
			return result;
		}

		template<typename T2>
		requires mult::constraint<T, T2>
		auto operator* (const T2& i_other) const
		{
			using R = mult::type<T, T2>;
			vector<R, Dim> result;
			for (int i = 0; i < Dim; i++)
			{
				result[i] = this->get(i) * i_other;
			}
			return result;
		}

		template<typename T2>
		requires div::constraint<T, T2>
		auto operator/ (const T2& i_other) const
		{
			using R = div::type<T, T2>;
			vector<R, Dim> result;
			for (int i = 0; i < Dim; i++)
			{
				result[i] = this->get(i) / i_other;
			}
			return result;
		}

		auto lengthSq() const
		{
			return _lengthSq(std::make_index_sequence<Dim>{});
		}

		T length() const
		{
			return std::sqrt(_lengthSq(std::make_index_sequence<Dim>{}));
		}

		auto normalized() const
		{
			T len = length();
			using R = div::type<T, T>;
			vector<R, Dim> result;
			for (size_t i = 0; i < Dim; ++i)
			{
				result[i] = this->get(i) / len;
			}
			return result;
		}

	private:
		template<size_t... Indices>
		auto _lengthSq(std::index_sequence<Indices...>) const
		{
			return ((this->get(Indices) * this->get(Indices)) + ...);
		}

		

	};

	namespace detail
	{
		template<typename T, typename... Params>
		vector<T, sizeof...(Params) + 1> create(T i_p1, Params... i_params)
		{
			return vector<T, sizeof...(Params) + 1>(i_p1, i_params...);
		}
	}

	template<typename T, typename T2>
	auto cross(const detail::vector_data<T, 3>& i_left, const detail::vector_data<T2, 3>& i_right)
	{
		return detail::create(i_left.y * i_right.z - i_left.z * i_right.y, i_left.z * i_right.x - i_left.x * i_right.z, i_left.x * i_right.y - i_left.y * i_right.x);
	}

	
}