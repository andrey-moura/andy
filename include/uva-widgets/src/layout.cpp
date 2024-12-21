#include <uva/widgets.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

void uva::widgets::layout::calculate_layout(int __x, int __y, int __w, int __h)
{
    x = __x;
    y = __y;
    w = __w;
    h = __h;

    int current_x = x + style.padding;
    int current_y = y + style.padding;

    int number_of_spacer_elements = 0;
    int available_space = 0;

    if(style_layout.direction == layout_style::layout_flex_direction::vertical) {
        available_space = __h - (style.padding * 2) - style.gap * childreans.size();
    } else {
        available_space = __w - (style.padding * 2) - style.gap * childreans.size();
    }

    for(auto& child : childreans) {
        if(child->style.flex > 0) {
            number_of_spacer_elements += child->style.flex;
        } else {
            if(style_layout.direction == layout_style::layout_flex_direction::vertical) {
                available_space -= child->h;
            } else {
                available_space -= child->w;
            }
        }
    }

    if(number_of_spacer_elements > 0) {
        int available_space_per_spacer = available_space / number_of_spacer_elements;

        for(auto& child : childreans) {
            if(child->style.flex > 0) {
                int child_space = available_space_per_spacer * child->style.flex;

                if(style_layout.direction == layout_style::layout_flex_direction::vertical) {
                    child->h = child_space;
                } else {
                    child->w = child_space;
                }
            }
        }
    }

    for(auto& child : childreans) {
        child->x = current_x;
        child->y = current_y;

        if(style_layout.direction == layout_style::layout_flex_direction::vertical) {
            child->w = __w - (style.padding * 2);
            current_y += child->h;
            current_y += style.gap;
        } else {
            child->h = __h - (style.padding * 2);
            current_x += child->w;
            current_x += style.gap;
        }

        layout* le = dynamic_cast<layout*>(child.get());

        if(le) {
            le->calculate_layout(child->x, child->y, child->w, child->h);
        }
    }
}

void uva::widgets::layout::draw()
{
    widget::draw();

    for(auto& child : childreans) {
        child->draw();
    }
}

void uva::widgets::layout::parse(uva::xml::schema& schema, uva::xml& xml)
{
    widget::parse(schema, xml);
    
    style_layout.type = (uva::widgets::layout_style::layout_type)schema.integer_attribute(xml, "type");
    style_layout.direction = (uva::widgets::layout_style::layout_flex_direction)schema.integer_attribute(xml, "direction");

    for(auto& child : xml.childrens) {
        if(child.tag == "text") {
            std::shared_ptr<uva::widgets::text> te = std::make_shared<uva::widgets::text>();
            te->content = child.content;
            
            te->parse(schema, child);

            // Get text size
            TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/abyssinica/AbyssinicaSIL-Regular.ttf", 24);

            if(!font) {
                throw std::runtime_error(SDL_GetError());
            }

            TTF_SizeText(font, te->content.c_str(), &te->w, &te->h);
            TTF_CloseFont(font);

            childreans.push_back(te);
        } else if(child.tag == "layout") {
            std::shared_ptr<uva::widgets::layout> le2 = std::make_shared<uva::widgets::layout>();
            le2->parse(schema, child);

            childreans.push_back(le2);
        }
    }
}