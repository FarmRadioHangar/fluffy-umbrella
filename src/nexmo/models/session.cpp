#include "session.h"
#include <bsoncxx/json.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace nexmo
{

session::session(const nlohmann::json& j)
  : ops::mongodb::model<session>{},
    _id{std::nullopt},
    _campaign{{}},
    _feature{{}}
{
    if (j.end() != j.find("id")) {
        _id = j.at("id");
    }

    _nexmo = j.at("nexmo");

    _campaign = core::campaign{j.at("campaign")};
    _feature = core::feature{j.at("feature")};
}

bsoncxx::builder::basic::document session::get_builder() const
{
    bsoncxx::builder::basic::document builder{};

    if (_id.has_value()) {
        builder.append(kvp("id", _id.value()));
    }

    builder.append(kvp("nexmo", bsoncxx::from_json(_nexmo.dump())));
    builder.append(kvp("campaign", _campaign.builder().extract()));
    builder.append(kvp("feature", _feature.builder().extract()));

    return builder;
}

} // namespace nexmo
