// Copyright 2013-2023 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_JSONSCHEMA_SCHEMA_FACTORY_HPP
#define JSONCONS_JSONSCHEMA_SCHEMA_FACTORY_HPP

#include <jsoncons_ext/jsonschema/draft7/schema_builder_7.hpp>
#include <jsoncons_ext/jsonschema/draft201909/schema_builder_201909.hpp>
#include <jsoncons_ext/jsonschema/draft202012/schema_builder_202012.hpp>

namespace jsoncons {
namespace jsonschema {

    template <class Json>
    struct default_uri_resolver
    {
        Json operator()(const jsoncons::uri& uri)
        {
            if (uri.string() == "https://json-schema.org/draft/2020-12/schema")
            {
                return jsoncons::jsonschema::draft202012::schema_draft202012<Json>::get_schema();
            }
            else if (uri.string() == "https://json-schema.org/draft/2019-09/schema")
            {
                return jsoncons::jsonschema::draft201909::schema_draft201909<Json>::get_schema();
            }
            else if (uri.string() == "http://json-schema.org/draft-07/schema#")
            {
                return jsoncons::jsonschema::draft7::schema_draft7<Json>::get_schema();
            }

            JSONCONS_THROW(jsonschema::schema_error("Don't know how to load JSON Schema " + uri.string()));
        }
    };

    template <class Json>
    class schema_builder_factory
    {
    public:
        schema_builder_factory()
        {
        }
            
        std::unique_ptr<schema_builder<Json>> operator()(const Json& sch, const uri_resolver<Json>& resolver,
            evaluation_options options) const
        {
            std::unique_ptr<schema_builder<Json>> builder_factory;

            if (sch.is_object())
            {
                auto it = sch.find("$schema");
                if (it != sch.object_range().end())
                { 
                    if (it->value().as_string_view() == "https://json-schema.org/draft/2020-12/schema")
                    {
                        builder_factory = jsoncons::make_unique<jsoncons::jsonschema::draft202012::schema_builder_202012<Json>>(*this, 
                            resolver, options);
                    }
                    else if (it->value().as_string_view() == "https://json-schema.org/draft/2019-09/schema")
                    {
                        builder_factory = jsoncons::make_unique<jsoncons::jsonschema::draft201909::schema_builder_201909<Json>>(*this, 
                            resolver, options);
                    }
                    else if (it->value().as_string_view() == "http://json-schema.org/draft-07/schema#")
                    {
                        builder_factory = jsoncons::make_unique<jsoncons::jsonschema::draft7::schema_builder_7<Json>>(*this, 
                            resolver, options);
                    }
                    else
                    {
                        std::string message("Unsupported schema version ");
                        message.append(it->value().template as<std::string>());
                        JSONCONS_THROW(schema_error(message));
                    }
                }
                else 
                {
                    switch (options.default_version())
                    {
                        case spec_version::draft7:
                            builder_factory = jsoncons::make_unique<jsoncons::jsonschema::draft7::schema_builder_7<Json>>(*this, 
                                resolver, options);
                            break;
                        case spec_version::draft201909:
                            builder_factory = jsoncons::make_unique<jsoncons::jsonschema::draft201909::schema_builder_201909<Json>>(*this, 
                                resolver, options);
                            break;
                        case spec_version::draft202012:
                            builder_factory = jsoncons::make_unique<jsoncons::jsonschema::draft202012::schema_builder_202012<Json>>(*this, 
                                resolver, options);
                            break;
                        default:
                            JSONCONS_THROW("Unsupported schema version");
                            break;
                    }
                }
            }
            else if (sch.is_bool())
            {
                builder_factory = jsoncons::make_unique<jsoncons::jsonschema::draft7::schema_builder_7<Json>>(*this, 
                    resolver, options);
            }
            else
            {
                 JSONCONS_THROW(schema_error("Schema must be object or boolean"));
            }
            return builder_factory;
        }
    };

    template <class Json,class URIResolver>
    typename std::enable_if<extension_traits::is_unary_function_object_exact<URIResolver,Json,std::string>::value,std::shared_ptr<json_schema<Json>>>::type
    make_schema(const Json& sch, const std::string& retrieval_uri, const URIResolver& resolver, 
        evaluation_options options = evaluation_options{})
    {
        schema_builder_factory<Json> builder_factory{};
        auto schema_builder = builder_factory(sch, resolver, options);

        schema_builder->build_schema(sch, retrieval_uri);
        return std::make_shared<json_schema<Json>>(schema_builder->get_schema());
    }

    template <class Json>
    std::shared_ptr<json_schema<Json>> make_schema(const Json& sch, const std::string& retrieval_uri, 
        evaluation_options options = evaluation_options{})
    {
        schema_builder_factory<Json> builder_factory{};
        auto schema_builder = builder_factory(sch, default_uri_resolver<Json>{}, options);

        schema_builder->build_schema(sch, retrieval_uri);
        return std::make_shared<json_schema<Json>>(schema_builder->get_schema());
    }

    template <class Json,class URIResolver>
    typename std::enable_if<extension_traits::is_unary_function_object_exact<URIResolver,Json,std::string>::value,std::shared_ptr<json_schema<Json>>>::type
    make_schema(const Json& sch, const URIResolver& resolver, 
        evaluation_options options = evaluation_options{})
    {
        schema_builder_factory<Json> builder_factory{};
        auto schema_builder = builder_factory(sch, uri_resolver<Json>{ resolver }, options);

        schema_builder->build_schema(sch);
        return std::make_shared<json_schema<Json>>(schema_builder->get_schema());
    }

    template <class Json>
    std::shared_ptr<json_schema<Json>> make_schema(const Json& sch, 
        evaluation_options options = evaluation_options{})
    {
        schema_builder_factory<Json> builder_factory{};
        auto schema_builder = builder_factory(sch, default_uri_resolver<Json>{}, options);

        schema_builder->build_schema(sch);
        return std::make_shared<json_schema<Json>>(schema_builder->get_schema());
    }

} // namespace jsonschema
} // namespace jsoncons

#endif // JSONCONS_JSONSCHEMA_SCHEMA_HPP
