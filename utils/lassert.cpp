//
// Created by ne1mnn on 2/28/2024.
//

#include "lassert.h"
#include "logger/console_logger.h"

std::unique_ptr<scl::logger> scl::assert_logger(new console_logger("ASSERT_LOGGER"));

