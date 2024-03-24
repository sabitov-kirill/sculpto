//
// Created by ne1mnn on 3/4/2024.
//

#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H
#include <memory>

namespace scl {

class vertex_array {
 public: /* Helper types */
  enum class mesh_type {
    LINES,      // Line segments (by 2 points)
    POINTS,     // Array of points
    PATCHES,    // Tesselation patches
    TRIANGLES,  // Triangle mesh - array of triangles
    TRISTRIP,   // Triangle strip - array of stripped triangles
  };

 public: /* Constructors and destructors */
  vertex_array() = default;
  vertex_array(const vertex_array&) = delete;
  vertex_array(vertex_array&&) = default;

  virtual ~vertex_array() = default;

  vertex_array& operator=(const vertex_array&) = delete;
  vertex_array& operator=(vertex_array&&) = default;

 public: /* Methods */
  virtual void bind() const = 0;
  virtual void unbind() const = 0;

 public: /* Data getters and setters */
  [[nodiscard]] virtual mesh_type get_type() const { return Type; }
  // [[nodiscard]] virtual std::shared_ptr<vertex_buffer> get_vertex_buffer() const { return vertex_buffer; }
  // [[nodiscard]] virtual std::shared_ptr<index_buffer> get_index_buffer() const { return index_buffer; }

 private: /* Data */
  mesh_type Type{};
  // std::shared_ptr<vertex_buffer> vertex_buffer{};
  // std::shared_ptr<index_buffer> index_buffer{};
};

}  // namespace scl

#endif  // VERTEX_ARRAY_H
