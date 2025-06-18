#include <deque>
#include <functional>
#include <list>
#include <type_traits>
#include <vector>

// Primary template (undefined)
template <typename T, template <typename...> class Container = std::vector,
		  template <typename...> class FunctionWrapper = std::function>
class Delegate;

// Specialization for function signature Ret(Args...)
template <typename Ret, typename... Args,
		  template <typename...> class Container,
		  template <typename...> class FunctionWrapper>
class Delegate<Ret(Args...), Container, FunctionWrapper> {
  public:
	using FunctionType = Ret(Args...);
	using FunctionWrapperType = FunctionWrapper<FunctionType>;

	// Constructor from a callable
	Delegate(FunctionWrapperType &&func)
		: functions({std::forward<FunctionWrapperType>(func)}) {}

	Delegate(const Delegate &other) = default;

	Delegate() = default;

	// Add a function via operator +=
	void operator+=(FunctionWrapperType func) {
		if (func)
			functions.push_back(std::move(func));
	}

	// Add a function via method
	void add(FunctionWrapperType func) { *this += std::move(func); }

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