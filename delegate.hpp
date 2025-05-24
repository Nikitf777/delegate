#include <functional>
#include <type_traits>
#include <vector>

template <typename T> class Delegate;

template <typename Ret, typename... Args> class Delegate<Ret(Args...)> {
  public:
	using FunctionType = std::function<Ret(Args...)>;

	template <typename T>
	Delegate(T &&func) : functions({std::forward<T>(func)}) {}

	Delegate() = default;

	Delegate(const Delegate &) = default;
	Delegate(Delegate &&) = default;

	Delegate &operator=(const Delegate &) = default;
	Delegate &operator=(Delegate &&) = default;

	void operator+=(FunctionType func) {
		if (func)
			functions.push_back(std::move(func));
	}

	void Add(FunctionType func) { *this += std::move(func); }

	Ret operator()(Args... args) const {
		if constexpr (std::is_void_v<Ret>) {
			for (const auto &func : functions) {
				func(std::forward<Args>(args)...);
			}
		} else {
			Ret result{};
			for (const auto &func : functions) {
				result = func(std::forward<Args>(args)...);
			}
			return result;
		}
	}

	void Clear() { functions.clear(); }
	bool IsEmpty() const { return functions.empty(); }

  private:
	std::vector<FunctionType> functions;
};
