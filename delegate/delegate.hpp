#include <deque>
#include <functional>
#include <list>
#include <type_traits>
#include <vector>

#if __cplusplus >= 202002L
#include <concepts>
#endif

#if __cplusplus >= 202002L
// Concept: FunctionWrapper must be callable with Args... and return Ret
template <template <typename...> class FunctionWrapper, typename Ret,
		  typename... Args>
concept ValidFunctionWrapper =
	requires(const FunctionWrapper<Ret(Args...)> f, Args... args) {
		{ f(std::forward<Args>(args)...) } -> std::same_as<Ret>;
	};

// Concept: Container must support necessary operations
template <template <typename...> class Container, typename T>
concept ValidContainer = requires(Container<T> c, T t, size_t s) {
	{ c.push_back(t) } -> std::same_as<void>;
	{ c.clear() } -> std::same_as<void>;
	{ c.empty() } -> std::convertible_to<bool>;
	{ c.size() } -> std::same_as<size_t>;
};
#endif

// Primary template (undefined)
template <typename T, template <typename...> class Container = std::vector,
		  template <typename...> class FunctionWrapper = std::function>
class Delegate;

// Specialization for function signature Ret(Args...)
template <typename Ret, typename... Args,
		  template <typename...> class Container,
		  template <typename...> class FunctionWrapper>
#if __cplusplus >= 202002L
	requires ValidFunctionWrapper<FunctionWrapper, Ret, Args...> &&
			 ValidContainer<Container, FunctionWrapper<Ret(Args...)>>
#endif
class Delegate<Ret(Args...), Container, FunctionWrapper> {
  public:
	using FunctionType = Ret(Args...);
	using FunctionWrapperType = FunctionWrapper<FunctionType>;

	Delegate() = default;

	Delegate(const Delegate &other) = default;

	// Constructor from a callable
	Delegate(FunctionWrapperType func) : functions({std::move(func)}) {}

	// Add a function via operator +=
	void operator+=(FunctionWrapperType func) {
		if (func)
			functions.push_back(std::move(func));
	}

	// Add a function via method
	void add(FunctionWrapperType func) { *this += std::move(func); }

	// Call operator: invoke all bound functions and return results
	[[nodiscard]]
	auto operator()(Args... args) const {
		if constexpr (!std::is_void_v<Ret>) {
			std::vector<Ret> results;
			results.reserve(functions.size());
			for (const auto &func : functions) {
				results.push_back(func(std::forward<Args>(args)...));
			}
			return results;
		} else {
			for (const auto &func : functions) {
				func(std::forward<Args>(args)...);
			}
		}
	}

	void clear() { functions.clear(); }
	bool isEmpty() const { return functions.empty(); }

  private:
	Container<FunctionWrapperType> functions;
};

// Alias for std::vector as the default container
template <typename T,
		  template <typename...> class FunctionWrapper = std::function>
using DelegateVector = Delegate<T, std::vector, FunctionWrapper>;

// Alias for std::list
template <typename T,
		  template <typename...> class FunctionWrapper = std::function>
using DelegateList = Delegate<T, std::list, FunctionWrapper>;

// Alias for std::deque
template <typename T,
		  template <typename...> class FunctionWrapper = std::function>
using DelegateDeque = Delegate<T, std::deque, FunctionWrapper>;