#include <typeinfo>
#include <limits>
#include <cstdint>

using namespace std;

template<typename CODE_VALUE, int CODE_VALUE_BITS_, int FREQUENCY_BITS_>
struct model_metrics {

    static const int CODE_VALUE_BITS      = CODE_VALUE_BITS_;
    static const int FREQUENCY_BITS       = FREQUENCY_BITS_;
    static const CODE_VALUE MAX_CODE      = (CODE_VALUE(1) << CODE_VALUE_BITS) - 1;
    static const CODE_VALUE MAX_FREQ      = (CODE_VALUE(1) << FREQUENCY_BITS) - 1;
    static const CODE_VALUE ONE_FOURTH    = CODE_VALUE(1) << (CODE_VALUE_BITS - 2);;
    static const CODE_VALUE ONE_HALF      = 2 * ONE_FOURTH;
    static const CODE_VALUE THREE_FOURTHS = 3 * ONE_FOURTH;

    template<typename STRING, typename STREAM>

    static void dump(const STRING &name, STREAM &s) {}

    struct prob {
        CODE_VALUE low;
        CODE_VALUE high;
        CODE_VALUE count;
    };
};
