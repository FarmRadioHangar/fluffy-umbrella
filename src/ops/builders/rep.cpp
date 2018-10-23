#include "rep.h"
#include <bsoncxx/builder/basic/kvp.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace ops
{

rep_builder::rep_builder(const nlohmann::json& j)
  : mongodb::bson::builder{j}
{
    append(kvp("format", std::string{j.at("format")}));
    append(kvp("language", std::string{j.at("language")}));
}

} // namespace ops