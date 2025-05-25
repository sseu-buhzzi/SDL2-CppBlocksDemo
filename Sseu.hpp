#ifndef SSEU_HPP
#define SSEU_HPP

#include "sseu_headers.hpp"
#include <iostream>

namespace Sseu {
    ::SDL_Window *window;
    ::SDL_Renderer *renderer;
    ::SDL_GLContext gl_context;
    ::int32_t window_x, window_y, window_width, window_height;
    bool running{true};

    struct ActivityBase{
        virtual ~ActivityBase() = default;
        virtual void init() = 0;
        virtual void update() = 0;
    };

    struct ActivityStack{
        struct Node{
            ActivityBase *activity;
            ::Sseu::ActivityStack::Node *prev;
        };
        Node *base;
        inline ActivityStack(): base{nullptr} { }
        template<typename T> inline void push() {
            (this->base = new ::Sseu::ActivityStack::Node{new T, this->base})->activity->init();
                                                                // ::std::cout<<__FILE__<<':'<<__LINE__<<'\t'<<this->base<<'\n';
                                                                // ::std::cout<<__FILE__<<':'<<__LINE__<<'\t'<<this->base->activity<<'\n';
        }
        inline void pop() {
            ::Sseu::ActivityStack::Node *deleted_node{this->base};
            this->base = this->base->prev;
                                                                // ::std::cout<<__FILE__<<':'<<__LINE__<<'\t'<<deleted_node->activity<<'\n';
            delete deleted_node->activity;
                                                                // ::std::cout<<__FILE__<<':'<<__LINE__<<'\t'<<deleted_node<<'\n';
            delete deleted_node;
                                                                // ::std::cout<<__FILE__<<':'<<__LINE__<<'\t'<<this->base<<'\n';
            if (this->base != nullptr) {
                this->base->activity->init();
            }
                                                                // ::std::cout<<__FILE__<<':'<<__LINE__<<'\t'<<this->base<<'\n';
                                                                ::std::cout<<__FILE__<<':'<<__LINE__<<'\t'<<"Main activity popped.\n";
        }
        template<typename T = ::Sseu::ActivityBase> inline T *top() const {
            return reinterpret_cast<T *>(this->base->activity);
        }
        inline bool empty() const {
            return this->base == nullptr;
        }
    };

    ActivityStack activity_stack;
    // template<typename T> void push_activity() {
    //     activity_stack.push(new T);
    //     activity_stack.top()->init();
    // }
    // void pop_activity() {
    //     delete activity_stack.top();
    //     activity_stack.pop();
    //     if (!activity_stack.empty()) {
    //                                                             // std::cout<<__LINE__<<'\n';
    //         activity_stack.top()->init();
    //                                                             // std::cout<<__LINE__<<'\n';
    //     }
    // }

    void init(char const *title, ::int32_t x, ::int32_t y, ::int32_t width, ::int32_t height) {
        ::SDL_Init(SDL_INIT_VIDEO);
        ::Sseu::window = ::SDL_CreateWindow(title, x, y, width, height, ::SDL_WindowFlags::SDL_WINDOW_OPENGL);
        ::Sseu::renderer = ::SDL_CreateRenderer(window, -1, 0U);
        ::Sseu::gl_context = ::SDL_GL_CreateContext(window);
        ::Sseu::window_x = x;
        ::Sseu::window_y = y;
        ::Sseu::window_width = width;
        ::Sseu::window_height = height;
    }

    void quit() {
        ::SDL_GL_DeleteContext(::Sseu::gl_context);
                                                                ::std::cout<<__FILE__<<':'<<__LINE__<<'\t'<<'\n';
        ::SDL_DestroyRenderer(::Sseu::renderer);
                                                                ::std::cout<<__FILE__<<':'<<__LINE__<<'\t'<<'\n';
        ::SDL_DestroyWindow(::Sseu::window);
                                                                ::std::cout<<__FILE__<<':'<<__LINE__<<'\t'<<'\n';
        ::SDL_Quit();
                                                                ::std::cout<<__FILE__<<':'<<__LINE__<<'\t'<<'\n';
    }

    void mainloop() {
        ::SDL_Event event;
                                                                // try{
        while (::Sseu::running) {
            while (::SDL_PollEvent(&event)) {
                if (event.type == ::SDL_QUIT) {
                    ::Sseu::running = false;
                }
            }

            if (::Sseu::activity_stack.empty()) {
                                                                ::std::cout<<__FILE__<<':'<<__LINE__<<'\t'<<"Activity stack is empty.\n";
                break;
            }
                                                                // ::std::cout<<__LINE__<<'\n';
            ::Sseu::activity_stack.top()->update();
                                                                // ::std::cout<<__LINE__<<'\n';
        }
                                                                // }catch(const ::std::exception&e){
                                                                //  ::std::cout<<"Error: ";
                                                                //  ::std::cout<<e.what()<<'\n';
                                                                // }
    }
}

#endif
