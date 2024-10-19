#include "LookupTable.hpp"

#include <cassert>
#include <format>
#include <stdexcept>

namespace skarjala {
    LookupTable::LookupTable(const std::vector<std::pair<uint32_t, double>>& lutPoints, const uint32_t lutSize)
    {
        if (lutPoints.size() < 2) {
            throw std::invalid_argument("Input vector for LookupTable must have at least two values to interpolate");
        }
        if (lutPoints.front().first != 0) {
            throw std::invalid_argument("First value of input vector for LookupTable must have an index of 0");
        }
        if (lutPoints.back().first != lutSize - 1) {
            throw std::invalid_argument("Last value of input vector for LookupTable must have an index of lutSize - 1");
        }
        if (lutSize < 2) {
            throw std::invalid_argument("LookupTable has to have at least 2 indices");
        }

        data = std::vector<double>(lutSize, 0.0);

        auto [lowerPointIndex, lowerPointValue] = lutPoints[0];
        data[0] = lowerPointValue;

        for (int index = 1; index < lutPoints.size(); ++index) {
            const auto [upperPointIndex, upperPointValue] = lutPoints[index];
            assert(upperPointIndex > lowerPointIndex);

            const auto totalIndicesInRange = upperPointIndex - lowerPointIndex;
            const double valueDeltaBetweenIndices = (upperPointValue - lowerPointValue) / static_cast<double>(totalIndicesInRange);

            for (int dataIndex = 1; dataIndex <= totalIndicesInRange; ++dataIndex) {
                data[lowerPointIndex + dataIndex] = lowerPointValue + valueDeltaBetweenIndices * static_cast<double>(dataIndex);
            }

            lowerPointIndex = upperPointIndex;
            lowerPointValue = upperPointValue;
        }
    }

    const double& LookupTable::operator[](const uint32_t index) const
    {
        if (index > data.size()) {
            const auto errmsg = std::format("LookupTable received out-of-range index {} on max size of {}", index, data.size());
            throw std::out_of_range(errmsg);
        }
        return data[index];
    }
}
