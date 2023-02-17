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

			template<int Dim2, typename... Params>
			vector_data(const vector_data<T, Dim2>& i_other, Params... i_params)
			{
				decltype(m_values) remain({ i_params... });
				int i = 0;
				for (i = 0; i < Dim2; ++i)
				{
					m_values[i] = i_other.get(i);
				}
				for (; i < Dim; ++i)
				{
					m_values[i] = remain[i - Dim2];
				}
			}

			const T& get(int i_dimension) const
			{
				return m_values[i_dimension];
			}

			T& get(int i_dimension)
			{
				return m_values[i_dimension];
			}

			template<typename T2>
			void set(int i_dimension, const T2& i_value)
			{
				m_values[i_dimension] = i_value;
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

			vector_data(T i_x, T i_y)
				: x(std::move(i_x))
				, y(std::move(i_y))
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

			template<typename T2>
			void set(int i_dimension, const T2& i_value)
			{
				switch (i_dimension)
				{
				case 0:
					x = i_value;
					break;
				default:
					y = i_value;
					break;
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

			vector_data(const vector_data<T, 2>& i_other, const T& i_z)
				: x(i_other.get(0))
				, y(i_other.get(1))
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

			template<typename T2>
			void set(int i_dimension, const T2& i_value)
			{
				switch (i_dimension)
				{
				case 0:
					x = i_value;
					break;
				case 1:
					y = i_value;
					break;
				case 2:
				default:
					z = i_value;
					break;
				}
			}

			
		};
	}

	
	template<typename T, int Dim>
	class vector: public detail::vector_data<T, Dim>
	{

	public:
		using value_type = T;

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

		template<int Dim2, typename... Params>
		requires (sizeof...(Params) == Dim - Dim2)
		vector(const vector<T, Dim2>& i_v, Params... i_params)
			: detail::vector_data<T, Dim>(i_v, i_params...)
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

		template<typename T2>
		void operator= (const vector<T2, Dim>& i_other)
		{
			for (int i = 0; i < Dim; i++)
			{
				this->set(i, i_other.get(i));
			}
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

		template<typename T, typename T2, int Dim, size_t... Indices>
		auto _dot(const vector<T, Dim>& i_left, const vector<T2, Dim>& i_right, std::index_sequence<Indices...>)
		{
			return ((i_left.get(Indices) * i_right.get(Indices)) + ...);
		}

		//unreal
		template<typename T, int Dim, size_t... Indices>
		inline bool _IsNearlyEqual(const engine::math::vector<T, Dim>& _ArgA, const engine::math::vector<T, Dim>& _ArgB, std::index_sequence<Indices...>)
		{
			return (IsNearlyEqual(_ArgA.get(Indices), _ArgB.get(Indices)) && ...);
		}
	}

	template<typename T, typename T2>
	inline auto cross(const detail::vector_data<T, 3>& i_left, const detail::vector_data<T2, 3>& i_right)
	{
		return detail::create(i_left.y * i_right.z - i_left.z * i_right.y, i_left.z * i_right.x - i_left.x * i_right.z, i_left.x * i_right.y - i_left.y * i_right.x);
	}

	template<typename T, typename T2, int Dim>
	inline auto dot(const vector<T, Dim>& i_left, const vector<T2, Dim>& i_right)
	{
		return detail::_dot(i_left, i_right, std::make_index_sequence<Dim>{});
	}


	template<typename T, typename T2, typename T3>
	inline auto signed_angle(const vector<T, 3>& i_left, const vector<T2, 3>& i_right, const vector<T3, 3>& i_axis)
	{
		auto d = dot(i_left, i_right);
		auto normal = cross(i_left, i_right);
		auto x1 = i_left.get(0);
		auto y1 = i_left.get(1);
		auto z1 = i_left.get(2);
		auto x2 = i_right.get(0);
		auto y2 = i_right.get(1);
		auto z2 = i_right.get(2);
		auto xn = i_axis.get(0);
		auto yn = i_axis.get(1);
		auto zn = i_axis.get(2);
		auto det = x1 * y2 * zn + x2 * yn * z1 + xn * y1 * z2 - z1 * y2 * xn - z2 * yn * x1 - zn * y1 * x2;
		return atan2(det, d);
	}

	template<typename T, typename T2, typename TAngle, int Dim>
	inline auto rotate_axis(const vector<T, Dim>& i_vector, const vector<T2, Dim>& i_axis, const TAngle& i_angle)
	{
		auto cosinus = cos(i_angle);
		using TCos = decltype(cosinus);
		return i_vector * cos(i_angle) + cross(i_axis, i_vector) * sin(i_angle) + 
			i_axis * 
				(dot(i_axis,i_vector)) 
			* (TCos(1.f) - cosinus);
	}

	
}


//unreal
template<typename T, int Dim>
inline bool IsNearlyEqual(const engine::math::vector<T, Dim>& _ArgA, const engine::math::vector<T, Dim>& _ArgB)
{
	return engine::math::detail::_IsNearlyEqual(_ArgA, _ArgB, std::make_index_sequence<Dim>{});
}

namespace std
{
	template<typename T, int Dim>
	auto distance(const engine::math::vector<T, Dim>& i_left, const engine::math::vector<T, Dim>& i_right)
	{
		return (i_right - i_left).length();
	}
}