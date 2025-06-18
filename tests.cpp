#include "delegate.hpp"
#include <gtest/gtest.h>
#include <typeindex>

using DelegateTypes =
	::testing::Types<DelegateVector<int(int)>, DelegateList<int(int)>,
					 DelegateDeque<int(int)>>;

template <typename T> class DelegateTest : public ::testing::Test {};
TYPED_TEST_SUITE(DelegateTest, DelegateTypes);

TYPED_TEST(DelegateTest, SingleFunctionReturnsValue) {
	TypeParam d([](int x) { return x + 1; });
	auto results = d(5);
	ASSERT_EQ(results.size(), 1);
	EXPECT_EQ(results[0], 6);
}

TYPED_TEST(DelegateTest, MultipleFunctionsAddedViaOperatorPlusEqual) {
	TypeParam d([](int x) { return x * 2; });
	typename TypeParam::FunctionType func = [](int x) { return x * 3; };
	d += func;
	auto results = d(5);
	EXPECT_EQ(results.size(), 2);
	EXPECT_EQ(results[0], 10);
	EXPECT_EQ(results[1], 15);
}

TYPED_TEST(DelegateTest, AddFunctionUsingAddMethod) {
	TypeParam d;
	d.add([](int x) { return x; });
	auto results = d(3);
	ASSERT_EQ(results.size(), 1);
	EXPECT_EQ(results[0], 3);
}

TYPED_TEST(DelegateTest, ClearResetsDelegate) {
	TypeParam d([](int x) { return x; });
	d.clear();
	EXPECT_TRUE(d.isEmpty());
	auto results = d(5);
	EXPECT_TRUE(results.empty());
}

TYPED_TEST(DelegateTest, NullFunctionIsIgnored) {
	typename TypeParam::FunctionType nullFunc;
	TypeParam d;
	d += nullFunc;
	EXPECT_TRUE(d.isEmpty());
}

TEST(DelegateEdgeCases, FunctionMovedSuccessfully) {
	std::function<int(int)> func = [](int x) { return x * 2; };
	DelegateVector<int(int)> d(std::move(func));
	EXPECT_TRUE(
		func.target_type().name() ==
		std::type_index(typeid(void)).name()); // func is empty after move
	auto results = d(5);
	ASSERT_EQ(results.size(), 1);
	EXPECT_EQ(results[0], 10);
}