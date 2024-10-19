#ifndef _SKARJALA_LOOKUP_TABLE_HPP_
#define _SKARJALA_LOOKUP_TABLE_HPP_

#include <map>
#include <stdint.h>
#include <vector>

namespace skarjala {
    class LookupTable
    {
      public:
        LookupTable(const std::vector<std::pair<uint32_t, double>>& lutValues, const uint32_t lutSize);
        ~LookupTable() = default;

        const double& operator[](const uint32_t index) const;

      private:
        std::vector<double> data;
    };
}

#endif // _SKARJALA_LOOKUP_TABLE_HPP_
