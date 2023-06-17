#include <dnd_config.hpp>

#include "choice_data.hpp"

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>

dnd::ChoiceData::ChoiceData(const ValidationData* parent) noexcept : ValidationSubdata(parent) {}

dnd::Errors dnd::ChoiceData::validate() const { return Errors(); }

dnd::Errors dnd::ChoiceData::validate_relations(const ContentHolder& content) const { return Errors(); }
