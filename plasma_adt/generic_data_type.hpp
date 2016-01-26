#pragma once

#include<boost/optional.hpp>
#include<boost/variant.hpp>

#include<type_traits>
#include<memory>
#include<array>

namespace generic_adt
{
	template<class T>struct type_tag
	{
		typedef T type;
	};
	template<class T>constexpr type_tag<T> make_type_tag(T const&)
	{
		return type_tag<T>{};
	}
	template<class T>constexpr type_tag<T> make_type_tag()
	{
		return type_tag<T>{};
	}

	namespace type_traits
	{
		namespace detail
		{
			template<bool I, bool... Is>struct conjunction_i :conjunction_i<Is...> {};
			template<bool... Is>struct conjunction_i<false, Is...> :std::false_type {};
			template<>struct conjunction_i<true> :std::true_type {};

			template<bool I, bool... Is>struct disjunction_i :disjunction_i<Is...> {};
			template<bool... Is>struct disjunction_i<true, Is...> :std::true_type {};
			template<>struct disjunction_i<false> :std::false_type {};


		}
		template<class...Ts>struct conjunction :detail::conjunction_i<(Ts::value)...> {};
		template<class...Ts>struct disjunction :detail::disjunction_i<(Ts::value)...> {};

	}

	template<class... Ts>struct tuple {};
	template<std::size_t Id, class Tuple>struct tuple_element;
	template<std::size_t Id, class... Ts>struct tuple_element<Id, tuple<Ts...>>
	{
	private:
		template<std::size_t Id, class T, class... Ts>struct impl :impl<Id - 1, Ts...>
		{

		};
		template<class T, class... Ts>struct impl<0, T, Ts...>
		{
			typedef T type;
		};
	public:
		typedef typename impl<Id, Ts...>::type type;
	};
	struct generic_tag {};

	namespace utility
	{
		namespace detail
		{

			template<class T, class Tag>struct have_tag :std::false_type {};
			template<template<class>class Temp, class Tag, class T0>struct have_tag<Temp<T0>, Tag> :
				type_traits::disjunction<have_tag<T0, Tag>> {};
			template<template<class, class>class Temp, class Tag, class T0, class T1>struct have_tag<Temp<T0, T1>, Tag> :
				type_traits::disjunction<have_tag<T0, Tag>, have_tag<T1, Tag>> {};
			template<template<class, class, class>class Temp, class Tag, class T0, class T1, class T2>struct have_tag<Temp<T0, T1, T2>, Tag> :
				type_traits::disjunction<have_tag<T0, Tag>, have_tag<T1, Tag>, have_tag<T2, Tag>> {};
			template<template<class, class, class, class>class Temp, class Tag, class T0, class T1, class T2, class T3>struct have_tag<Temp<T0, T1, T2, T3>, Tag> :
				type_traits::disjunction<have_tag<T0, Tag>, have_tag<T1, Tag>, have_tag<T2, Tag>, have_tag<T3, Tag>> {};
			template<template<class, class, class, class, class>class Temp, class Tag, class T0, class T1, class T2, class T3, class T4>struct have_tag<Temp<T0, T1, T2, T3, T4>, Tag> :
				type_traits::disjunction<have_tag<T0, Tag>, have_tag<T1, Tag>, have_tag<T2, Tag>, have_tag<T3, Tag>, have_tag<T4, Tag>> {};
			template<template<class, class, class, class, class, class>class Temp, class Tag, class T0, class T1, class T2, class T3, class T4, class T5>struct have_tag<Temp<T0, T1, T2, T3, T4, T5>, Tag> :
				type_traits::disjunction<have_tag<T0, Tag>, have_tag<T1, Tag>, have_tag<T2, Tag>, have_tag<T3, Tag>, have_tag<T4, Tag>, have_tag<T5, Tag>> {};

			template<template<class...>class Temp, class Tag, class... Ts>struct have_tag<Temp<Ts...>, Tag> :
				type_traits::disjunction<have_tag<Ts, Tag>...> {};
			template<class Tag>struct have_tag<Tag, Tag> :std::true_type {};

			template<class T, class From, class To>struct trans_type
			{
				typedef T type;
			};
			template<template<class>class Temp, class From, class To, class T0>struct trans_type<Temp<T0>, From, To>
			{
				typedef Temp<typename trans_type<T0, From, To>::type> type;
			};
			template<template<class, class>class Temp, class From, class To, class T0, class T1>struct trans_type<Temp<T0, T1>, From, To>
			{
				typedef Temp<typename trans_type<T0, From, To>::type, typename trans_type<T1, From, To>::type> type;
			};
			template<template<class, class, class>class Temp, class From, class To, class T0, class T1, class T2>struct trans_type<Temp<T0, T1, T2>, From, To>
			{
				typedef Temp<typename trans_type<T0, From, To>::type, typename trans_type<T1, From, To>::type, typename trans_type<T2, From, To>::type> type;
			};
			template<template<class, class, class, class>class Temp, class From, class To, class T0, class T1, class T2, class T3>struct trans_type<Temp<T0, T1, T2, T3>, From, To>
			{
				typedef Temp<typename trans_type<T0, From, To>::type, typename trans_type<T1, From, To>::type, typename trans_type<T2, From, To>::type, typename trans_type<T3, From, To>::type> type;
			};
			template<template<class, class, class, class, class>class Temp, class From, class To, class T0, class T1, class T2, class T3, class T4>struct trans_type<Temp<T0, T1, T2, T3, T4>, From, To>
			{
				typedef Temp<typename trans_type<T0, From, To>::type, typename trans_type<T1, From, To>::type, typename trans_type<T2, From, To>::type, typename trans_type<T3, From, To>::type, typename trans_type<T4, From, To>::type> type;
			};
			template<template<class, class, class, class, class, class>class Temp, class From, class To, class T0, class T1, class T2, class T3, class T4, class T5>struct trans_type<Temp<T0, T1, T2, T3, T4, T5>, From, To>
			{
				typedef Temp<typename trans_type<T0, From, To>::type, typename trans_type<T1, From, To>::type, typename trans_type<T2, From, To>::type, typename trans_type<T3, From, To>::type, typename trans_type<T4, From, To>::type, typename trans_type<T5, From, To>::type> type;
			};

			template<template<class...>class Temp, class From, class To, class... Ts>struct trans_type<Temp<Ts...>, From, To>
			{
				typedef Temp<typename trans_type<Ts, From, To>::type...> type;
			};
			template<class From, class To>struct trans_type<From, From, To>
			{
				typedef To type;
			};
		}

		template<class Target, class Base,class Pointer, class Generic>
		struct trans_tuple
		{
			typedef std::tuple<
				typename detail::trans_type<
				typename detail::trans_type<
				Target, generic_tag, Generic>::type,
				Base, Pointer>::type> type;
		};
		template<class Base, class Pointer, class Generic, class... Ts>
		struct trans_tuple<tuple<Ts...>, Base, Pointer, Generic>
		{
			typedef std::tuple<
				typename detail::trans_type<
				typename detail::trans_type<
				Ts, generic_tag, Generic>::type,
				Base, Pointer>::type...> type;
		};
		template<class Base, class Pointer, class Generic>
		struct trans_tuple<void, Base, Pointer, Generic>
		{
			typedef std::tuple<>;
		};
		template<class Target, class Base>
		struct have_generic:type_traits::disjunction<
			detail::have_tag<Target,Base>,
			detail::have_tag<Target,generic_tag>>{};

		template<class Return, class Base, class ValueType, class Generic>
		struct trans_return_type
		{
			typedef typename detail::trans_type<typename detail::trans_type<Return, Base, ValueType>::type, generic_tag, Generic>::type type;
		};

		template<std::size_t Id, class T>struct id_type
		{
			T value;
		};
		template<std::size_t Id, class T>constexpr id_type<Id,
			typename std::remove_const<
			typename std::remove_reference<T>::type>::type> make_id_type(T&& v)
		{
			return id_type<Id,
				typename std::remove_const<
				typename std::remove_reference<T>::type>::type>{std::forward<T>(v)};
		}
		template<std::size_t Id, class T>T& get_id_value(id_type<Id, T>& v)
		{
			return v.value;
		}
		template<std::size_t Id, class T>constexpr T const& get_id_value(id_type<Id, T> const& v)
		{
			return v.value;
		}
		template<std::size_t Id, class T>constexpr T&& get_id_value(id_type<Id, T>&& v)
		{
			return std::move(v.value);
		}

	}

	namespace place_holder
	{
		namespace detail
		{
			template<int V, int I, int... Is>struct value_t :value_t<10 * V + I, Is...> {};
			template<int V, int I>struct value_t<V, I> :std::integral_constant<int, 10 * V + I> {};
		}
		template<int I>struct place_holder_t
		{
			struct place_holder
			{
				template<class T>boost::optional<std::tuple<pattern_match::detail::id_type<Id, T>>> operator()(const T& v)const
				{
					return boost::make_optional(pattern_match::detail::make_tuple<Id>(v));
				}
			};
			place_holder operator()(...)const
			{
				return place_holder{};
			}
		};
		template<char... Cs>constexpr auto operator"" _()
		{
			return place_holder_t<detail::value_t<0, static_cast<int>(Cs - '0')...>::value>{};
		}
		struct ignore_place_t
		{
			struct place_holder
			{
				boost::optional<std::tuple<>> operator()(...)const
				{
					return boost::make_optional(std::tuple<>{});
				}
			};
			place_holder operator()(...)const
			{
				return place_holder{};
			}
		};
	}

	namespace pattern_match
	{
		struct pattern_match_tag {};
		namespace detail
		{
			template<std::size_t Id, class T>struct id_type
			{
				typedef T type;
				T value;
				static constexpr std::size_t id = Id;
			};
			template<std::size_t Id, class T>auto make_tuple(T&& v)
			{
				return std::make_tuple(id_type<Id, typename std::remove_const<typename std::remove_reference<T>::type>::type>{std::forward<T>(v)});
			}
			template<std::size_t Id, class T, std::size_t I = 0>auto id_get(T const& v, std::enable_if_t<std::tuple_element_t<I, T>::id == Id>* = nullptr)
			{
				return std::get<I>(v).value;
			}
			template<std::size_t Id, class T, std::size_t I = 0>auto id_get(T const& v, std::enable_if_t<std::tuple_element_t<I, T>::id != Id>* = nullptr)
			{
				return id_get<Id, T, I + 1>(v);
			}
			template<std::size_t Id, class T, std::size_t I = 0, class = void>struct id_element
			{
				typedef typename id_element<Id, T, I + 1>::type type;
			};
			template<std::size_t Id, class T, std::size_t I>struct id_element<Id, T, I, std::enable_if_t<std::tuple_element_t<I, T>::id == Id>>
			{
				typedef typename std::tuple_element_t<I, T>::type type;
			};
			template<class T>auto optional_cat(T const&, boost::none_t)
			{
				return boost::none;
			}
			template<class T>auto optional_cat(boost::none_t, T const&)
			{
				return boost::none;
			}
			template<class T>auto optional_cat(T const& v)
			{
				return v;
			}
			template<class T, class U>auto optional_cat(T const& x, U const& y)
				->decltype(boost::make_optional(std::tuple_cat(*x, *y)))
			{
				if (x&&y)
				{
					return boost::make_optional(std::tuple_cat(*x, *y));
				}
				return boost::none;
			}
			template<class T, class... Ts>auto optional_cat(T const& v, Ts const&... vs)
			{
				return optional_cat(v, optional_cat(vs...));
			}

			template<class IndexSequence>struct id_tuple_apply_i;
			template<std::size_t... Is>struct id_tuple_apply_i<std::index_sequence<Is...>>
			{
				template<class T, class Func, class... Ts>static auto run(T const& t, Func const& func, Ts&&... args)
				{
					return func(id_get<Is>(t)..., std::forward<Ts>(args)...);
				}
			};
			template<class Tuple, class Func, class... Ts>auto id_tuple_apply(Tuple const& t, Func const& func, Ts&&...args)
			{
				return id_tuple_apply_i<std::make_index_sequence<std::tuple_size<Tuple>::value>>::run(t, func, std::forward<Ts>(args)...);
			}

			template<class Func, class Args, class IndexSequence>struct lambda_t;
			template<class Func, class... Args, std::size_t... Is>struct lambda_t<Func, std::tuple<Args...>, std::index_sequence<Is...>>
			{
				Func func;
				std::tuple<Args...> arg;
				template<class... Ts>decltype(auto) operator()(Ts&&... args)const
				{
					return func(std::get<Is>(arg)..., std::forward<Ts>(args)...);
				}
			};
			template<class Func, class... Args>auto make_lambda(Func func, Args... args)
			{
				return lambda_t<Func, std::tuple<Args...>, std::make_index_sequence<sizeof...(Args)>>{func, std::make_tuple(args...)};
			}

			template<class T>struct forward_t
			{
				typedef T const& type;
			};
			template<class T>struct forward_t<T&>
			{
				typedef T& type;
			};
		}
	
		template<class Return, class DataType, class... Ts>struct generic_match_t
		{
			template<class Generic>using value_type =
				typename DataType::template value_type<Generic>;
			template<class Generic>using return_type =
				utility::trans_return_type<Return, DataType, value_type<Generic>, Generic>;
			
			template<class Next, class Pattern, class Func>struct match_t
			{
				Next next;
				Pattern pattern;
				Func func;

				template<class Generic>boost::optional<return_type<Generic>> operator()
					(value_type<Generic>const& value, typename detail::forward_t<Ts>::type... arg)const
				{
					if (auto ret = next(value, args...))
						return ret;
					if (auto pat = pattern(type_tag<Generic>{})(value))
						return boost::make_optional(
							detail::id_tuple_apply(*pat, detail::make_lambda(func, type_tag<Generic>{}), arg...));
					return boost::none;
				}
				
				template<class BeforePattern, class BeforeFunc>match_t<match_t<Next, Pattern, Func>, BeforePattern, BeforeFunc>
					operator|(std::pair<BeforePattern, BeforeFunc> p)const
				{
					return match_t<match_t<Next, Pattern, Func>, BeforePattern, BeforeFunc>{*this, p.first, p.second};
				}
			};

			struct first_match_t
			{
				template<class Generic>boost::optional<return_type<Generic>> operator()
					(value_type<Generic>const& value, typename detail::forward_t<Ts>::type... arg)const
				{
					return boost::none;
				}
			};
			template<class BeforePattern, class BeforeFunc>match_t<first_match_t, BeforePattern, BeforeFunc>
				operator|(std::pair<BeforePattern, BeforeFunc> p)const
			{
				return match_t<first_match_t, BeforePattern, BeforeFunc>{first_match_t{}, p.first, p.second};
			}
		};
		template<class Return, class DataType, class... Ts>struct generic_recursion_t
		{
			template<class Generic>using value_type =
				typename DataType::template value_type<Generic>;
			template<class Generic>using return_type =
				utility::trans_return_type<Return, DataType, value_type<Generic>, Generic>;

			template<class Next, class Pattern, class Func>struct match_t
			{
				Next next;
				Pattern pattern;
				Func func;

				template<class Recur,class Generic>boost::optional<return_type<Generic>> run
					(Recur recur, value_type<Generic>const& value, typename detail::forward_t<Ts>::type... arg)const
				{
					if (auto ret = next.run(value, args...))
						return ret;
					if (auto pat = pattern(type_tag<Generic>{})(value))
						return boost::make_optional(
							detail::id_tuple_apply(*pat, detail::make_lambda(func, recur, type_tag<Generic>{}), arg...));
					return boost::none;
				}

				template<class Generic>return_type operator()
					(value_type<Generic>const& value, typename detail::forward_t<Ts>::type... arg)const
				{
					if (auto ret = run(std::cref(*this), value, arg...))return *ret;
					throw std::invalid_argument(R"(pattern match error: not found this pattern)");
				}

				template<class BeforePattern, class BeforeFunc>match_t<match_t<Next, Pattern, Func>, BeforePattern, BeforeFunc>
					operator|(std::pair<BeforePattern, BeforeFunc> p)const
				{
					return match_t<match_t<Next, Pattern, Func>, BeforePattern, BeforeFunc>{*this, p.first, p.second};
				}
			};

			struct first_match_t
			{
				template<class Recur,class Generic>boost::optional<return_type<Generic>> operator()
					(Recur, value_type<Generic>const&, typename detail::forward_t<Ts>::type...)const
				{
					return boost::none;
				}
			};
			template<class BeforePattern, class BeforeFunc>match_t<first_match_t, BeforePattern, BeforeFunc>
				operator|(std::pair<BeforePattern, BeforeFunc> p)const
			{
				return match_t<first_match_t, BeforePattern, BeforeFunc>{first_match_t{}, p.first, p.second};
			}
		};

	}
}