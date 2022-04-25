#include <array>
#include <unordered_set>

#include "gtest/gtest.h"
#include "VersatileMask.hpp"

enum TestEnum {
    TEST_1,
    TEST_2,
    TEST_3
};

class TestMask : public VersatileMask<uint8_t, TestEnum> {
public:
    TestMask() : VersatileMask(0) {}
    using Hash = VersatileMaskHash<uint8_t, TestEnum>;
    void set(TestEnum key, uint8_t value) {
        VersatileMask::set(key, value);
    }
private:
    uint8_t mask(TestEnum t) const override {
        return MASKS[t];
    }
    uint8_t shift(TestEnum t) const override {
        return SHIFTS[t];
    }
    static const std::array<uint8_t, 3> MASKS;
    static const std::array<uint8_t, 3> SHIFTS;
};

const std::array<uint8_t, 3> TestMask::MASKS = {0x0F, 0x30, 0xC0};
const std::array<uint8_t, 3> TestMask::SHIFTS = {0, 4, 6};

TEST(versatile_mask_test, constructor) {
    TestMask mask;
    EXPECT_EQ(mask.get(), 0);
    EXPECT_EQ(mask, TestMask());
}

TEST(versatile_mask_test, set) {
    TestMask mask;
    mask.set(TestEnum::TEST_1, 7);
    EXPECT_EQ(mask.get(), 0x07);
    mask.set(TestEnum::TEST_2, 1);
    EXPECT_EQ(mask.get(), 0x17);
    mask.set(TestEnum::TEST_3, 2);
    EXPECT_EQ(mask.get(), 0x97);
}

TEST(versatile_mask_test, hash) {
    std::unordered_set<TestMask, typename TestMask::Hash> test_set;
    TestMask mask1, mask2;
    mask1.set(TestEnum::TEST_1, 7);
    mask2.set(TestEnum::TEST_3, 2);
    test_set.insert(mask1);
    test_set.insert(mask2);
    EXPECT_EQ(test_set.size(), 2);
}
