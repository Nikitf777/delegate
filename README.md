# delegate
This header-only library provides a simple template class for creating C#-like multicast delegates.
It's written using modern C++11 and C++20 features. However, it can be built with C++11. In this case, it doesn't use `concepts`.

### Usage
```C++
#include "delegate/delegate.hpp"
#include <iostream>

struct MyClass {
	void Print(int x) { std::cout << "MyClass::Print(" << x << ")\n"; }
};

void FreeFunction(int x) { std::cout << "FreeFunction(" << x << ")\n"; }

int main() {
	// Create a delegate that matches the signature void(int)
	Delegate<void(int)> myDelegate;

	MyClass obj;

	// Add a member function using std::bind
	myDelegate += std::bind(&MyClass::Print, &obj, std::placeholders::_1);

	// Add a lambda
	myDelegate += [](int x) { std::cout << "Lambda(" << x << ")\n"; };

	// Add a free function
	myDelegate += FreeFunction;

	// Invoke the delegate
	myDelegate(42);

	return 0;
}
```
### Returning values
```C++
#include "delegate/delegate.hpp"
#include <iostream>

struct MyClass {
	int a;
	int product(int x) {
		std::cout << "MyClass::product(" << x << ")\n";
		return x * a;
	}
};

int FreeFunction(int x) {
	std::cout << "FreeFunction(" << x << ")\n";
	return x * 2;
}

int main() {
	// Create a delegate that matches the signature void(int)
	Delegate<int(int)> myDelegate;

	MyClass obj{2};

	// Add a member function using std::bind
	myDelegate += std::bind(&MyClass::product, &obj, std::placeholders::_1);

	// Add a lambda
	myDelegate += [](int x) {
		std::cout << "Lambda(" << x << ")\n";
		return x * 2;
	};

	// Add a free function
	myDelegate += FreeFunction;

	// Invoke the delegate
	std::vector results = myDelegate(42);

	// Get the results
	std::cout << "Results:\n";
	for (auto result : results)
		std::cout << result << std::endl;

	return 0;
}
```
### Predefined Aliases
```C++
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
```

