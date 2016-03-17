#ifndef PIZZA_HH
#define PIZZA_HH

#include <type_traits>
#include <array>

template<typename... Kinds>
struct Pizzeria {
private:
	/*
	Small, helper class for operations on types pack.
	*/
	template<typename... Types>
	struct types_pack {
		// returns occurences of Type in list Types...
		template<typename Type>
		static constexpr size_t count() {
			return ((std::is_same<Type, Types>::value? 1 : 0) + ... );
		}

		// checks if Type is within Types...
		template<typename Type>
		static constexpr bool is_within() {
			return count<Type>() > 0;
		}

		// checks if Types... are pairwise distinct
		static constexpr bool pairwise_distinct() {
			return ((count<Types>() == 1) && ...);
		}
	};

	using list_of_kinds = types_pack<Kinds...>;
	static_assert(list_of_kinds::pairwise_distinct(), "Pizza kinds should be pairwise distinct!");

	static constexpr size_t kinds_count = sizeof...(Kinds);
	
	template<size_t... Counts>
	struct Pizza {
		using pizzeria_type = Pizzeria<Kinds...>;

		template <typename Kind>
		static constexpr size_t count() {
			return ((std::is_same<Kind, Kinds>::value? Counts : 0)  + ...);
		}

		static constexpr const std::array<size_t, kinds_count> as_array() {
			return {{Counts...}};
		}

		using sliced_type = Pizza<2*Counts...>;
	};

	template<typename Kind>
	static constexpr size_t optimal_kind_count(size_t max) {
		static_assert(Kind::yumminess(0) == 0, "Yumminess of zero pieces should be zero!");
		size_t optimal_number = 0;
		for (size_t i = 0 ; i <= max ; i++) {
			if (Kind::yumminess(optimal_number) < Kind::yumminess(i))
				optimal_number = i;
		}
		return optimal_number;
	}

	template<typename Pizza1, typename Pizza2>
	struct best_mix_impl {
		using pizzas_sum = Pizza<(Pizza1::template count<Kinds>() + Pizza2::template count<Kinds>())...>;
		using optimal_subpizza = Pizza<optimal_kind_count<Kinds>(pizzas_sum::template count<Kinds>())...>;
		using result = optimal_subpizza;
	};

public:
	template<typename Kind>
	struct make_pizza {
		static_assert(list_of_kinds::template is_within<Kind>(), "That pizza is not in menu!");
		using type = Pizza<(std::is_same<Kind, Kinds>::value? 8 : 0)...>;
	};

	template<typename Pizza1, typename Pizza2>
	friend struct best_mix;
};

template<typename Pizza1, typename Pizza2>
struct best_mix {
	static_assert(std::is_same<typename Pizza1::pizzeria_type, typename Pizza2::pizzeria_type>::value, "Mixed pizzas should be from the same pizzeria!");
	using type = typename Pizza1::pizzeria_type::template best_mix_impl<Pizza1, Pizza2>::result;
};

#endif // PIZZA_HH
