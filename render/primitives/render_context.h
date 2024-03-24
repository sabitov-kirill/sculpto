//
// Created by ne1mnn on 3/4/2024.
//

#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "exception.h"

namespace scl {

class render_exception : public exception {
  using exception::exception;

 public:
  render_exception() noexcept = default;

  explicit render_exception(const std::string& message) : exception("Render: " + message) {}
};

class render_context {
 public: /* Constructors and destructors */
 public: /* Methods */
};

}  // namespace scl

#endif  // RENDER_CONTEXT_H
