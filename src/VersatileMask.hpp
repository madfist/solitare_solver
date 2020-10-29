#ifndef VERSATILE_MASK_HEADER
#define VERSATILE_MASK_HEADER

template<typename CarrierType, typename MaskElementEnum>
class VersatileMask {
public:
    VersatileMask(CarrierType d) : data(d) {}
    VersatileMask& operator=(const VersatileMask<CarrierType, MaskElementEnum>& m) {
        data = m.data;
        return *this;
    }
    bool operator==(const VersatileMask<CarrierType, MaskElementEnum>& m) const {
        return (data == m.data);
    }
    bool operator!=(const VersatileMask<CarrierType, MaskElementEnum>& m) const {
        return (data != m.data);
    }
    CarrierType get() const {
        return data;
    }
    virtual ~VersatileMask() {}
protected:
    CarrierType get(MaskElementEnum m) const {
        return (data & mask(m)) >> shift(m);
    }
    void set(CarrierType d) {
        data = d;
    }
    void set(MaskElementEnum m, CarrierType d) {
        CarrierType value = (d << shift(m)) & mask(m);
        data &= ~mask(m);
        data |= value;
    }
private:
    virtual CarrierType mask(MaskElementEnum) const = 0;
    virtual uint8_t shift(MaskElementEnum) const = 0;

    CarrierType data;
};

template<typename CarrierType, typename MaskElementEnum>
struct VersatileMaskHash {
    std::size_t operator()(const VersatileMask<CarrierType, MaskElementEnum>& m) const {
        return m.get();
    }
};

#endif