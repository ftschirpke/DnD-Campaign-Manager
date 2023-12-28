#ifndef DATA_RESULT_HPP_
#define DATA_RESULT_HPP_

#include <dnd_config.hpp>

#include <memory>

#include <tl/expected.hpp>

#include <core/errors/errors.hpp>

namespace dnd {

/**
 * @brief A class to hold a result of an attempt to construct an object from data
 * @details This is a light wrapper around tl::expected to better fit the needs of the project
 * @tparam OutputT the type of the output
 * @tparam DataT the type of the data
 */
template <typename OutputT, typename DataT>
class DataResult {
public:
    static DataResult valid(OutputT&& output) noexcept;
    static DataResult invalid(DataT&& data, Errors&& errors) noexcept;

    bool is_valid() const noexcept;
    const OutputT& value() const;
    OutputT&& value();
    const std::pair<DataT, Errors>& data_and_errors() const;
    std::pair<DataT, Errors>&& data_and_errors();
private:
    DataResult(tl::expected<OutputT, std::pair<DataT, Errors>>&& inner) noexcept;

    tl::expected<OutputT, std::pair<DataT, Errors>> inner;
};

// holds a result of an attempt to construct an object from data
// it either holds the constructed object or the invalid data and the errors that caused it to be invalid
template <typename T>
using CreateResult = DataResult<T, typename T::Data>;

template <typename T>
CreateResult<T> ValidCreate(T&& output) noexcept;

template <typename T>
CreateResult<T> InvalidCreate(typename T::Data&& data, Errors&& errors) noexcept;

// holds a result of an attempt to construct an object from data where the constructed object is polymorphic
// it either holds the constructed object or the invalid data and the errors that caused it to be invalid
template <typename T>
using FactoryResult = DataResult<std::unique_ptr<T>, typename T::Data>;

template <typename T>
FactoryResult<T> ValidFactory(std::unique_ptr<T>&& output) noexcept;

template <typename T>
FactoryResult<T> InvalidFactory(typename T::Data&& data, Errors&& errors) noexcept;

} // namespace dnd

#endif // DATA_RESULT_HPP_
