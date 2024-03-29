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
    static DataResult valid(OutputT&& output);
    static DataResult invalid(DataT&& data, Errors&& errors);

    bool is_valid() const;
    const OutputT& value_ref() const;
    OutputT&& value();
    const std::pair<DataT, Errors>& data_and_errors_ref() const;
    std::pair<DataT, Errors>&& data_and_errors();
private:
    DataResult(tl::expected<OutputT, std::pair<DataT, Errors>>&& inner);

    tl::expected<OutputT, std::pair<DataT, Errors>> inner;
};

// holds a result of an attempt to construct an object from data
// it either holds the constructed object or the invalid data and the errors that caused it to be invalid
template <typename T>
using CreateResult = DataResult<T, typename T::Data>;

template <typename T>
CreateResult<T> ValidCreate(T&& output);

template <typename T>
CreateResult<T> InvalidCreate(typename T::Data&& data, Errors&& errors);

// holds a result of an attempt to construct an object from data where the constructed object is polymorphic
// it either holds the constructed object or the invalid data and the errors that caused it to be invalid
template <typename T>
using FactoryResult = DataResult<std::unique_ptr<T>, typename T::Data>;

template <typename T>
FactoryResult<T> ValidFactory(std::unique_ptr<T>&& output);

template <typename T>
FactoryResult<T> InvalidFactory(typename T::Data&& data, Errors&& errors);


// === IMPLEMENTATION ===

template <typename OutputT, typename DataT>
DataResult<OutputT, DataT> DataResult<OutputT, DataT>::valid(OutputT&& output) {
    return DataResult(tl::expected<OutputT, std::pair<DataT, Errors>>(std::move(output)));
}

template <typename OutputT, typename DataT>
DataResult<OutputT, DataT> DataResult<OutputT, DataT>::invalid(DataT&& data, Errors&& errors) {
    return DataResult(tl::unexpected(std::make_pair(std::move(data), std::move(errors))));
}

template <typename OutputT, typename DataT>
bool DataResult<OutputT, DataT>::is_valid() const {
    return inner.has_value();
}

template <typename OutputT, typename DataT>
const OutputT& DataResult<OutputT, DataT>::value_ref() const {
    return inner.value();
}

template <typename OutputT, typename DataT>
OutputT&& DataResult<OutputT, DataT>::value() {
    return std::move(inner.value());
}

template <typename OutputT, typename DataT>
const std::pair<DataT, Errors>& DataResult<OutputT, DataT>::data_and_errors_ref() const {
    return inner.error();
}

template <typename OutputT, typename DataT>
std::pair<DataT, Errors>&& DataResult<OutputT, DataT>::data_and_errors() {
    return std::move(inner.error());
}

template <typename OutputT, typename DataT>
DataResult<OutputT, DataT>::DataResult(tl::expected<OutputT, std::pair<DataT, Errors>>&& inner)
    : inner(std::move(inner)) {}


template <typename T>
CreateResult<T> ValidCreate(T&& output) {
    return CreateResult<T>::valid(std::move(output));
}

template <typename T>
CreateResult<T> InvalidCreate(typename T::Data&& data, Errors&& errors) {
    return CreateResult<T>::invalid(std::move(data), std::move(errors));
}

template <typename T>
FactoryResult<T> ValidFactory(std::unique_ptr<T>&& output) {
    return FactoryResult<T>::valid(std::move(output));
}

template <typename T>
FactoryResult<T> InvalidFactory(typename T::Data&& data, Errors&& errors) {
    return FactoryResult<T>::invalid(std::move(data), std::move(errors));
}

} // namespace dnd

#endif // DATA_RESULT_HPP_
