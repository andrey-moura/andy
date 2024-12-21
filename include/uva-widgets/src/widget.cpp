#include <uva/widgets/widget.hpp>

#include <uva/xml.hpp>

void uva::widgets::widget::parse(uva::xml::schema& schema, uva::xml& xml) {
    style.background_color = schema.color_attribute(xml, "background-color");

    style.flex = atoi(xml.attribute("flex", "0").data());
    style.padding = atoi(xml.attribute("padding", "0").data());
    style.gap = atoi(xml.attribute("gap", "0").data());
    style.border_radius = atoi(xml.attribute("border-radius", "0").data());
}