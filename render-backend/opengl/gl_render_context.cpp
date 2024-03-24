//
// Created by ne1mnn on 3/4/2024.
//

#include "gl_render_context.h"

#include "logger/console_logger.h"

scl::base_gl_render_context::base_gl_render_context()
    : _logger(std::make_unique<console_logger>("OPENGL")) {}