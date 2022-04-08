/**
 * Versatile bitmask implementation
 * @file VersatileMask.hpp
 * @author Adam Koleszar
 */
#ifndef VERSATILE_MASK_HEADER
#define VERSATILE_MASK_HEADER

/**
 * @brief Versatile bitmask with named parts
 * 
 * @details This is an abstract class, implement `mask` and `shift` methods.
 * 
 * Example:
 * 
 * ```{.cpp}
 * enum MaskElement {
 *   MASK1, MASK2, MASK3
 * }
 * 
 * class Something : public VersatileMask<uint_8, MaskElement> {
 * public:
 *   Something() : VersatileMask(DEFAULT) {}
 * private:
 *   uint8_t mask(MaskElement m) const {
 *     return MASKS[m];
 *   }
 *   uint8_t shift(MaskElement m) const {
 *     return SHIFTS[m];
 *   }
 * }
 * 
 * const std::array<uint8_t, 3> Something::MASKS = {0x0F, 0x30, 0x40};
 * const std::array<uint8_t, 3> Something::SHIFTS = {0, 4, 6};
 * ```
 * 
 * @tparam CarrierType carrier (integral type) to hold the data
 * @tparam MaskElementEnum enum defining parts of the bitmask
 */
template<typename CarrierType, typename MaskElementEnum>
class VersatileMask {
public:
    VersatileMask(CarrierType d) : data(d) {}
    VersatileMask& operator=(const VersatileMask<CarrierType, MaskElementEnum>& mask) {
        data = mask.data;
        return *this;
    }
    bool operator==(const VersatileMask<CarrierType, MaskElementEnum>& mask) const {
        return (data == mask.data);
    }
    bool operator!=(const VersatileMask<CarrierType, MaskElementEnum>& mask) const {
        return (data != mask.data);
    }
    CarrierType get() const {
        return data;
    }
    virtual ~VersatileMask() {}
protected:
    /**
     * @brief Get value in mask shifted to the least significant bit
     */
    CarrierType get(MaskElementEnum mask_element) const {
        return (data & mask(mask_element)) >> shift(mask_element);
    }
    void set(CarrierType carrier) {
        data = carrier;
    }
    void set(MaskElementEnum mask_element, CarrierType carrier) {
        // shift value to the right place and cut off anyhing outside the masked part
        CarrierType value = (carrier << shift(mask_element)) & mask(mask_element);
        // delete masked part
        data &= ~mask(mask_element);
        // set new value to masked part
        data |= value;
    }
private:
    /**
     * @brief Mask the right parts of the bits according to the enum value
     * @arg mask_element enum indicating the masked part
     * @return number turning on the relevant bits (e.g. 0x3 will make the last 2 bits relevant)
     */
    virtual CarrierType mask(MaskElementEnum mask_element) const = 0;
    /**
     * @brief Tells which bit the mask is starting
     * @arg mask_element enum indicating the masked part
     * @return the bit where the mask is starting (e.g 3 will tell you that `mask_element` starts on the 3rd bit)
     */
    virtual uint8_t shift(MaskElementEnum mask_element) const = 0;

    CarrierType data;
};

/// Helper functor for hash operations (e.g. set, map)
template<typename CarrierType, typename MaskElementEnum>
struct VersatileMaskHash {
    std::size_t operator()(const VersatileMask<CarrierType, MaskElementEnum>& m) const {
        return m.get();
    }
};

#endif