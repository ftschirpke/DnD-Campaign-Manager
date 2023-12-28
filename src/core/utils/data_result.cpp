#include <dnd_config.hpp>

#include "data_result.hpp"

#include <utility>

#include <tl/expected.hpp>

#include <core/errors/errors.hpp>

namespace dnd {

template <typename OutputT, typename DataT>
DataResult<OutputT, DataT> DataResult<OutputT, DataT>::valid(OutputT&& output) noexcept {
    return DataResult(tl::expected(std::move(output)));
}

template <typename OutputT, typename DataT>
DataResult<OutputT, DataT> DataResult<OutputT, DataT>::invalid(DataT&& data, Errors&& errors) noexcept {
    return DataResult(tl::unexpected(std::make_pair(std::move(data), std::move(errors))));
}

template <typename OutputT, typename DataT>
bool DataResult<OutputT, DataT>::is_valid() const noexcept {
    return inner.has_value();
}

template <typename OutputT, typename DataT>
const OutputT& DataResult<OutputT, DataT>::value() const {
    return inner.value();
}

template <typename OutputT, typename DataT>
OutputT&& DataResult<OutputT, DataT>::value() {
    return inner.value();
}

template <typename OutputT, typename DataT>
const std::pair<DataT, Errors>& DataResult<OutputT, DataT>::data_and_errors() const {
    return inner.error();
}

template <typename OutputT, typename DataT>
std::pair<DataT, Errors>&& DataResult<OutputT, DataT>::data_and_errors() {
    return inner.error();
}

template <typename OutputT, typename DataT>
DataResult<OutputT, DataT>::DataResult(tl::expected<OutputT, std::pair<DataT, Errors>>&& inner) noexcept
    : inner(std::move(inner)) {}


template <typename T>
CreateResult<T> ValidCreate(T&& output) noexcept {
    return CreateResult<T>::valid(std::move(output));
}

template <typename T>
CreateResult<T> InvalidCreate(typename T::Data&& data, Errors&& errors) noexcept {
    return CreateResult<T>::invalid(std::move(data), std::move(errors));
}

template <typename T>
FactoryResult<T> ValidFactory(std::unique_ptr<T>&& output) noexcept {
    return FactoryResult<T>::valid(std::move(output));
}

template <typename T>
FactoryResult<T> InvalidFactory(typename T::Data&& data, Errors&& errors) noexcept {
    return FactoryResult<T>::invalid(std::move(data), std::move(errors));
}

} // namespace dnd
