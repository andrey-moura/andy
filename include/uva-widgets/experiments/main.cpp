#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

#include <uva/file.hpp>
#include <uva/xml.hpp>
#include <uva/binary.hpp>
#include <uva/widgets.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

SDL_Renderer* renderer;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("uva", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_ShowWindow(window);

    renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;
    bool running = true;

    size_t total_frames = 0;
    size_t frames_last_second = 0;
    size_t total_draws = 0;
    auto very_start = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds total_duration = std::chrono::milliseconds(0);
    size_t next_duration = 1000;
    const float target_fps = 60.0;
    const float target_frame_time_ms = 1000.0 / target_fps;
    const std::chrono::nanoseconds target_frame_nanoseconds = std::chrono::nanoseconds((int)(1000000000 / target_fps));
    float total_free_time_ms = 0.0;
    std::chrono::milliseconds total_draw_duration = std::chrono::milliseconds(0);

    uva::widgets::layout l;

    while(running) {
        auto frame_start = std::chrono::high_resolution_clock::now();

        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    running = false;
                break;
                //window
                case SDL_WINDOWEVENT: {
                    auto start = std::chrono::high_resolution_clock::now();
                    //std::cout << "drawing" << std::endl;

                    auto draw_start = std::chrono::high_resolution_clock::now();

                    std::string content = uva::file::read_all_text<char>(std::filesystem::absolute("test.xml"));
                    std::string schema_content = uva::file::read_all_text<char>(std::filesystem::absolute("file.xsd"));

                    // std::cout << content << std::endl;

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                    SDL_RenderClear(renderer);

                    if(content.size()) {
                        int width, height;
                        SDL_GetWindowSize(window, &width, &height);

                        //std::cout << "file read" << std::endl;

                        uva::xml xml = uva::xml::decode(std::move(content));
                        uva::xml::schema schema(std::move(uva::xml::decode(std::move(schema_content))));

                        //std::cout << "xml decoded" << std::endl;

                        //std::cout << "found " << xml.childrens.size() << " childreans" << std::endl;

                        if(xml.tag == "layout") {
                            l.childreans.clear();

                            l.parse(schema, xml);

                            l.calculate_layout(0, 0, width, height);

                            l.draw();
                        }
                    }

                    SDL_RenderPresent(renderer);

                    auto draw_end = std::chrono::high_resolution_clock::now();
                    ++total_draws;

                    total_draw_duration += std::chrono::duration_cast<std::chrono::milliseconds>(draw_end - draw_start);
                }
                break;
            }
        }

        // Check if the window is in focus
        if(SDL_GetWindowFlags(window) & SDL_WINDOW_MOUSE_FOCUS) {
            //std::cout << "window in focus" << std::endl;

            int x, y;
            SDL_GetMouseState(&x, &y);

            // Try to find the element under the mouse

            uva::widgets::widget* element_at_mouse = nullptr;

            SDL_SystemCursor cursor = SDL_SYSTEM_CURSOR_ARROW;

            for(auto& child : l.childreans) {
                if(x >= child->x && x <= child->x + child->w && y >= child->y && y <= child->y + child->h) {
                    element_at_mouse = child.get();
                    
                    auto le = dynamic_cast<uva::widgets::layout*>(child.get());

                    if(le) {
                        for(auto& child2 : le->childreans) {
                            if(x >= child2->x && x <= child2->x + child2->w && y >= child2->y && y <= child2->y + child2->h) {
                                element_at_mouse = child2.get();
                                break;
                            }
                        }
                    }
                }
            }

            if(element_at_mouse) {
                switch(element_at_mouse->style.cursor) {
                    case uva::widgets::widget_cursor::widget_cursor_pointer:
                        cursor = SDL_SYSTEM_CURSOR_HAND;
                        break;
                    default:
                        cursor = SDL_SYSTEM_CURSOR_ARROW;
                        break;
                }
            }

            SDL_SetCursor(SDL_CreateSystemCursor(cursor));
        }

        auto now = std::chrono::high_resolution_clock::now();

        total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - very_start);

        ++total_frames;
        ++frames_last_second;

        if(total_duration.count() >= next_duration) {
            next_duration += 1000;

            float fps = total_frames / (total_duration.count() / 1000.0);

            std::cout << "total duration (ms): " << total_duration.count() << std::endl;
            std::cout << "total frames: " << total_frames << std::endl;
            std::cout << "target frame time (ms): " << target_frame_time_ms << std::endl;
            float avg_frame_time = total_duration.count() / (float)total_frames;
            float percent_frame_time = (avg_frame_time  / target_frame_time_ms) * 100.0;
            std::cout << "AVG frame time (ms): " << total_duration.count() / (float)total_frames << " (" << percent_frame_time << "%)" << std::endl;
            std::cout << "fps: " << fps << std::endl;
            std::cout << "fps last second: " << frames_last_second << std::endl;
            std::cout << "AVG free time (ms): " << total_free_time_ms / total_frames << std::endl;
            std::cout << "total draws: " << total_draws << std::endl;
            std::cout << "AVG draw time (ms): " << total_draw_duration.count() / (float)total_draws << std::endl;
            std::cout << std::endl;

            frames_last_second = 0;
        }

        auto frame_end = std::chrono::high_resolution_clock::now();

        auto frame_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(frame_end - frame_start);
        
        if(frame_duration < target_frame_nanoseconds) {
            std::chrono::nanoseconds free_time = target_frame_nanoseconds - frame_duration;
            int free_time_nano = free_time.count();
            total_free_time_ms += free_time_nano / 1000000.0;

            std::this_thread::sleep_for(free_time);
        }
    }

    return 0;
}

void uva::widgets::widget::draw()
{
    if(style.background_color.a == 0) {
        return;
    }

    SDL_SetRenderDrawColor(renderer, style.background_color.r, style.background_color.g, style.background_color.b, style.background_color.a);

    SDL_Rect rect = { x, y, w, h };

    if(style.border_radius) {
        rect.x += style.border_radius;
        rect.w -= 2 * style.border_radius;
    }

    SDL_RenderFillRect(renderer, &rect);

    if(style.border_radius) {
        rect.x = x;
        rect.y = style.border_radius + y;
        rect.h = h - 2 * style.border_radius;
        rect.w = style.border_radius;

        SDL_RenderFillRect(renderer, &rect);

        rect.x = x + w - style.border_radius;
        rect.y = style.border_radius + y;
        rect.h = h - 2 * style.border_radius;
        rect.w = style.border_radius;

        SDL_RenderFillRect(renderer, &rect);

        filledCircleRGBA(renderer, x + style.border_radius, y + style.border_radius, style.border_radius, style.background_color.r, style.background_color.g, style.background_color.b, style.background_color.a);
        filledCircleRGBA(renderer, (x - 1) + w - style.border_radius, y + style.border_radius, style.border_radius, style.background_color.r, style.background_color.g, style.background_color.b, style.background_color.a);
        // Bottom left
        filledCircleRGBA(renderer, x + style.border_radius, y + h - style.border_radius, style.border_radius, style.background_color.r, style.background_color.g, style.background_color.b, style.background_color.a);
        filledCircleRGBA(renderer, (x - 1) + w - style.border_radius, y + h - style.border_radius, style.border_radius, style.background_color.r, style.background_color.g, style.background_color.b, style.background_color.a);
    }

}