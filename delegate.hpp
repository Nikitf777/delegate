#include <deque>
#include <functional>
#include <list>
#include <type_traits>
#include <vector>

// Primary template (undefined)
template <typename T, template <typename...> class Container = std::vector>
class Delegate;

// Specialization for function signature Ret(Args...)
template <typename Ret, typename... Args,
		  template <typename...> class Container>
class Delegate<Ret(Args...), Container> {
  public:
	using FunctionType = std::function<Ret(Args...)>;

	// Constructor from a callable
	template <typename TFunc>
	Delegate(TFunc &&func) : functions({std::forward<TFunc>(func)}) {}

	Delegate() = default;

	// Add a function via operator +=
	void operator+=(FunctionType func) {
		if (func)
			functions.push_back(std::move(func));
	}

	// Add a function via method
	void add(FunctionType func) { *this += std::move(func); }

	// Call operator: invoke all bound functions and return results
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

	// Utility methods
	void clear() { functions.clear(); }
	bool isEmpty() const { return functions.empty(); }

  private:
	Container<FunctionType> functions;
};

// Alias for std::vector as the default container
template <typename T> using DelegateVector = Delegate<T, std::vector>;

// Alias for std::list
template <typename T> using DelegateList = Delegate<T, std::list>;

// Alias for std::deque
template <typename T> using DelegateDeque = Delegate<T, std::deque>;