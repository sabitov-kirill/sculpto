#include "pch.h"

#include "logger/console_logger.h"
#include "windows_window.h"

#include <list>
#include <thread>

namespace geom_2d {

using point = POINT;
using points_store = std::list<point>;
using point_ref = points_store::iterator;
using line = std::pair<point_ref, point_ref>;
using line_store = std::list<line>;

constexpr int POINT_RADIUS{15};
constexpr int POINT_DIAM{POINT_RADIUS * 2};
constexpr int POINT_RADIUS2{POINT_RADIUS * POINT_RADIUS};

namespace draw {

void point(const geom_2d::point &point, Gdiplus::Graphics &graphics, const Gdiplus::Pen &pen) {
  graphics.DrawEllipse(&pen, point.x - POINT_RADIUS, point.y - POINT_RADIUS, POINT_DIAM, POINT_DIAM);
}

void line(const geom_2d::point &a, const geom_2d::point &b, Gdiplus::Graphics &graphics, const Gdiplus::Pen &pen) {
  graphics.DrawLine(&pen,                                          //
                    static_cast<int>(a.x), static_cast<int>(a.y),  //
                    static_cast<int>(b.x), static_cast<int>(b.y));
}

}  // namespace draw

namespace compute {

int dist2(const point &a, const point &b) { return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y); }

std::optional<point_ref> find_point(const point &p, points_store &points) {
  for (auto ps = points.begin(); ps != points.end(); ++ps) {
    if (dist2(p, *ps) <= POINT_RADIUS2) {
      return {ps};
    }
  }
  return {};
}

}  // namespace compute

}  // namespace geom_2d

class window : public scl::windows_window<window> {
 public: /* Helper types */
 public: /* Constructors and destructors */
  window() : windows_window(L"Test") {}

 public: /* Methods */
  void on_create() {
    Gdiplus::GdiplusStartup(&_gdi_plus_token, &_gdi_plus_startup_input, nullptr);

    _logger.info("Window created");
  }

  void on_size(const int width, const int height) { _logger.info("Window resized to ({}, {})", width, height); }

  void on_close() {
    Gdiplus::GdiplusShutdown(_gdi_plus_token);

    _run = false;
    _logger.info("Window closed");
  }

  void on_draw(HDC draw_context) {
    Gdiplus::Graphics graphics(draw_context);

    Gdiplus::Pen editing_line_pen{Gdiplus::Color(150, 200, 0, 0), 1};

    draw_points(graphics);
    draw_lines(graphics);
    if (_editing_state == state::CREATING_LINE) {
      geom_2d::draw::line(**_selected_point, _mouse_pos, graphics, editing_line_pen);
    }
  }

  void on_mouse_move(int x, int y, UINT key_flags) {
    _mouse_pos = {x, y};

    if (_editing_state == state::MOVING_POINT) {
      (*_selected_point)->x = x;
      (*_selected_point)->y = y;
    }
  }

  void on_mouse_wheel(int x, int y, int z, UINT keys) {}

  void on_mouse_down(bool is_double_click, int x, int y, UINT keys) {
    _editing_state = keys & MK_RBUTTON ? state::MOVING_POINT : state::CREATING_LINE;

    _selected_point = geom_2d::compute::find_point({x, y}, _points);
    if (!_selected_point) {
      _selected_point = _points.emplace(_points.end(), x, y);
    }
  }

  void on_mouse_up(int x, int y, UINT keys) {
    if (_editing_state == state::CREATING_LINE) {
      if (auto p = geom_2d::compute::find_point({x, y}, _points)) {
        _lines.emplace_back(*_selected_point, *p);
      }
    }

    _selected_point.reset();
    _editing_state = state::IDLE;
  }

  void on_key(UINT code, bool is_down, int repeat_count, UINT flags) {}

  void run() {
    while (_run) {
      update();
      InvalidateRect(get_handle(), nullptr, true);
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  }

  void draw_points(Gdiplus::Graphics &graphics) {
    Gdiplus::Pen points_pen{Gdiplus::Color(255, 0, 0, 0), 1};
    for (auto &&point : _points) {
      geom_2d::draw::point(point, graphics, points_pen);
    }
  }

  void draw_lines(Gdiplus::Graphics &graphics) {
    Gdiplus::Pen line_pen{Gdiplus::Color(255, 0, 0, 0), 1};
    for (auto &&line : _lines) {
      geom_2d::draw::line(*line.first, *line.second, graphics, line_pen);
    }
  }

 private: /* Data */
  enum class state { IDLE, MOVING_POINT, CREATING_LINE };

  Gdiplus::GdiplusStartupInput _gdi_plus_startup_input{};
  ULONG_PTR _gdi_plus_token{};

  geom_2d::points_store _points{};
  geom_2d::line_store _lines{};

  state _editing_state{};
  std::optional<geom_2d::point_ref> _selected_point{};
  geom_2d::point _mouse_pos{};

  scl::console_logger _logger{"TEST WINDOW"};
  bool _run{true};
};

int main() {
  window w{};
  w.run();
  return 0;
}