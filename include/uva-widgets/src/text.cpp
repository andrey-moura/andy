#include <uva/widgets/text.hpp>

#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

extern SDL_Renderer* renderer;

void uva::widgets::text::draw()
{
    widget::draw();

    if(content.size()) {
        TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/abyssinica/AbyssinicaSIL-Regular.ttf", 24);

        if(!font) {
            throw std::runtime_error(SDL_GetError());
        }

        SDL_Color color = { style.color.r, style.color.g, style.color.b, style.color.a };
        SDL_Surface* surface = TTF_RenderText_Solid(font, content.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        // Get size of the texture
        int texture_w, texture_h;
        SDL_QueryTexture(texture, NULL, NULL, &texture_w, &texture_h);
        SDL_FreeSurface(surface);
        SDL_Rect rect = { x, y, texture_w, texture_h };
        switch(text_style.vertical_alignment) {
            case text_vertical_alignment::text_vertical_alignment_top:
                break;
            case text_vertical_alignment::text_vertical_alignment_center:
                rect.y += (h - texture_h) / 2;
                break;
            case text_vertical_alignment::text_vertical_alignment_bottom:
                rect.y += h - texture_h;
                break;
        }
        switch(text_style.horizontal_alignment) {
            case text_horizontal_alignment::text_horizontal_alignment_left:
                break;
            case text_horizontal_alignment::text_horizontal_alignment_center:
                rect.x += (w - texture_w) / 2;
                break;
            case text_horizontal_alignment::text_horizontal_alignment_right:
                rect.x += w - texture_w;
                break;
        }
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        TTF_CloseFont(font);
    }
}

void uva::widgets::text::parse(uva::xml::schema& schema, uva::xml& xml)
{
    widget::parse(schema, xml);
    
    style.color = schema.color_attribute(xml, "color");
    style.cursor = (uva::widgets::widget_cursor)schema.integer_attribute(xml, "cursor");

    text_style.vertical_alignment   = (uva::widgets::text_vertical_alignment)schema.integer_attribute(xml, "vertical-align");
    text_style.horizontal_alignment = (uva::widgets::text_horizontal_alignment)schema.integer_attribute(xml, "horizontal-align");
}