///
/// \file content.h
///
#pragma once

#include "../http/rest/controller.h"

namespace ops
{
    class content_controller : public http::rest::controller
    {
    public:
        content_controller();

        void get_item(http::request request) override;
        void get(http::request request) override;
        void post(http::request request) override;

        void post_rep(http::request request);
        void post_media(http::request request);
    };
}