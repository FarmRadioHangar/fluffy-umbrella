/*!
 * \file document.h
 */
#pragma once

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types/value.hpp>
#include <cpprest/json.h>
#include <map>
#include <set>
#include "exception.h"

namespace polls
{
    namespace utils
    {
        namespace builder
        {
            /*!
             * \class document
             *
             * \brief Utility class for creating polls::model documents from
             * JSON data.
             */
            template <typename T> class document
            {
            public:
                explicit document(const web::json::value& json);
                explicit document(const std::string& data);

                ~document() = default;
                document(const document&) = delete;
                document& operator=(const document&) = delete;

                void add_property(const std::string& name, const web::json::value::value_type type, const bool required = false);
                void add_unique_constraint(const std::string& key);

                T build() const;

            protected:
                bsoncxx::types::value to_bson_value(const web::json::value& json) const;

            private:
                using prop_map = std::map<std::string, std::pair<web::json::value::value_type, bool>>;

                web::json::value      _json;
                prop_map              _properties;
                std::set<std::string> _unique_keys;
            };

            /*!
             * \brief todo
             */
            template <typename T>
            document<T>::document(const web::json::value& json)
              : _json{json},
                _properties{},
                _unique_keys{}
            {
            }

            /*!
             * \brief todo
             */
            template <typename T>
            document<T>::document(const std::string& data)
              : _json{web::json::value::parse(data)},
                _properties{},
                _unique_keys{}
            {
            }

            /*!
             * \brief todo
             */
            template <typename T> void document<T>::add_property(
                const std::string&                 name,
                const web::json::value::value_type type,
                const bool                         required)
            {
                _properties.insert({name, {type, required}});
            }

            /*!
             * \brief todo
             */
            template <typename T>
            void document<T>::add_unique_constraint(const std::string& key)
            {
                _unique_keys.insert(key);
            }

            /*!
             * \brief todo
             */
            template <typename T> T document<T>::build() const
            {
                bsoncxx::builder::basic::document builder{};

                if (!_json.is_object()) {
                    throw builder::error{
                        builder::error_type::json_not_an_object,
                        "JSON data must be an object"};
                }

                for (const auto& prop : _properties)
                {
                    const auto& key = prop.first;
                    const auto type = prop.second.first;
                    const bool required = prop.second.second;

                    if (required && !_json.has_field(key)) {
                        throw builder::key_validation_error{
                            builder::error_type::missing_property,
                            "Missing property: " + key,
                            key};
                    }
                    if (_json.has_field(key) && _json.at(key).type() != type) {
                        throw builder::key_validation_error{
                            builder::error_type::type_mismatch,
                            "Type mismatch for key '" + key + "'",
                            key};
                    }
                    builder.append(kvp(key, to_bson_value(_json.at(key))));
                }

                for (const auto& key : _unique_keys)
                {
                    if (_json.has_field(key))
                    {
                        auto filter = bsoncxx::builder::basic::make_document(
                            kvp(key, to_bson_value(_json.at(key)))
                        );
                        if (T::count(filter.view()) > 0) {
                            throw builder::key_validation_error{
                                builder::error_type::unique_constraint_violation,
                                "Unique constraint violation for key: " + key,
                                key};
                        }
                    }
                }

                T document{};
                document.set_data(bsoncxx::to_json(builder.extract()));
                return document;
            }

            template <typename T> bsoncxx::types::value
            document<T>::to_bson_value(const web::json::value& json) const
            {
                using namespace bsoncxx::types;

                switch (json.type())
                {
                    case web::json::value::value_type::Object:
                    {
                        bsoncxx::builder::basic::document builder{};
                        for (const auto& keyval : json.as_object()) {
                            builder.append(kvp(keyval.first, to_bson_value(keyval.second)));
                        }
                        return value{b_document{builder.extract()}};
                    }
                    case web::json::value::value_type::Array:
                    {
                        bsoncxx::builder::basic::array builder{};
                        for (const auto& doc : json.as_array()) {
                            builder.append(to_bson_value(doc));
                        }
                        return value{b_array{builder.extract()}};
                    }
                    case web::json::value::value_type::Number:
                    {
                        if (json.is_integer()) {
                            return value{b_int64{json.as_integer()}};
                        } else {
                            return value{b_double{json.as_double()}};
                        }
                    }
                    case web::json::value::value_type::Boolean:
                        return value{b_bool{json.as_bool()}};
                    case web::json::value::value_type::String:
                        return value{b_utf8{json.as_string()}};
                    case web::json::value::value_type::Null:
                    default:
                        return value{b_null{}};
                }
            }
        }
    }
}
