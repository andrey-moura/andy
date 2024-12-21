#pragma once

#include <uva/color.hpp>
#include <cstdint>

#include <uva/xml.hpp>

namespace uva
{
    namespace widgets
    {
        enum widget_cursor : uint8_t {
            widget_cursor_default,
            widget_cursor_pointer,
            widget_cursor_max
        };

        struct widget_style {
            uint8_t             flex = 0;
            widget_cursor cursor = widget_cursor::widget_cursor_default;

            int        padding = 0;
            int        gap     = 0;
            uva::color color = uva::color(0, 0, 0, 255);
            uva::color background_color = uva::color(0, 0, 0, 0);
            int border_radius = 0;
        };

        class widget {
        public:
            int x;
            int y;

            int w;
            int h;

            widget_style style;

            virtual void draw();
            virtual void parse(uva::xml::schema& schema, uva::xml& xml);
        };
    };
};