#pragma once

namespace engine::math
{
	class random
	{
	public:
		using result_type = std::default_random_engine::result_type;

	private:
		class base
		{
		public:
			virtual ~base() = default;
			virtual result_type get() = 0;
		};

		template<typename T>
		class std_impl : public base
		{
		public:
			std_impl(T&& i_r)
				: m_stdRandom(i_r)
			{

			}

			inline result_type get()
			{
				return m_stdRandom();
			}

		private:
			T m_stdRandom;
		};

		random(std::unique_ptr<base> i_impl)
			: m_impl(std::move(i_impl))
		{

		}

	public:

		template<typename T>
		static random from_std(T&& i_random)
		{
			return random(std::move(std::unique_ptr<std_impl<T>>(new std_impl<T>(std::move(i_random)))));
		}

		static constexpr result_type min()
		{
			return std::default_random_engine::min();
		}

		static constexpr result_type max()
		{
			return std::default_random_engine::max();
		}

		result_type operator() ()
		{
			return m_impl->get();
		}

		template<typename T>
		T Get(const T& i_min, const T& i_max)
		{
			T delta = i_max - i_min;
			return i_min + static_cast<T>(m_impl->get() - min()) * delta / static_cast<T>(max() - min());
		}

		template<typename T, typename M>
		measure<M, T> Get(const measure<M, T>& i_min, const measure<M, T>& i_max)
		{
			T v = Get<T>(i_min.internalValueInSI(), i_max.internalValueInSI());
			return measure<M, T>(v);
		}

	private:
		
		

		std::unique_ptr<base> m_impl;

	};
}