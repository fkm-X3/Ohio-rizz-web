#include <cctype>
#include <cstdint>
#include <cstring>
#include <memory>
#include <stack>
#include <string>
#include <vector>

static const uint8_t font5x7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // space
    {0x04, 0x04, 0x04, 0x00, 0x04}, // !
    {0x0A, 0x0A, 0x00, 0x00, 0x00}, // "
    {0x0A, 0x1F, 0x0A, 0x1F, 0x0A}, // #
    {0x0E, 0x15, 0x0E, 0x05, 0x0E}, // $
    {0x18, 0x19, 0x02, 0x13, 0x03}, // %
    {0x0C, 0x12, 0x0D, 0x12, 0x0D}, // &
    {0x04, 0x04, 0x00, 0x00, 0x00}, // '
    {0x02, 0x04, 0x08, 0x04, 0x02}, // (
    {0x08, 0x04, 0x02, 0x04, 0x08}, // )
    {0x00, 0x04, 0x15, 0x0E, 0x15}, // *
    {0x04, 0x04, 0x1F, 0x04, 0x04}, // +
    {0x00, 0x00, 0x00, 0x04, 0x08}, // ,
    {0x00, 0x00, 0x1F, 0x00, 0x00}, // -
    {0x00, 0x00, 0x00, 0x00, 0x04}, // .
    {0x01, 0x02, 0x04, 0x08, 0x10}, // /
    {0x0E, 0x11, 0x11, 0x11, 0x0E}, // 0
    {0x04, 0x0C, 0x04, 0x04, 0x0E}, // 1
    {0x0E, 0x11, 0x02, 0x04, 0x1F}, // 2
    {0x1F, 0x02, 0x0E, 0x01, 0x1F}, // 3
    {0x02, 0x06, 0x0A, 0x1F, 0x02}, // 4
    {0x1F, 0x10, 0x1E, 0x01, 0x1E}, // 5
    {0x06, 0x08, 0x1E, 0x11, 0x0E}, // 6
    {0x1F, 0x01, 0x02, 0x04, 0x04}, // 7
    {0x0E, 0x11, 0x0E, 0x11, 0x0E}, // 8
    {0x0E, 0x11, 0x0F, 0x01, 0x0E}, // 9
    {0x00, 0x04, 0x00, 0x04, 0x00}, // :
    {0x00, 0x04, 0x00, 0x04, 0x08}, // ;
    {0x02, 0x04, 0x08, 0x04, 0x02}, // <
    {0x00, 0x1F, 0x00, 0x1F, 0x00}, // =
    {0x08, 0x04, 0x02, 0x04, 0x08}, // >
    {0x0E, 0x11, 0x02, 0x00, 0x04}, // ?
    {0x0E, 0x11, 0x17, 0x15, 0x0E}, // @
    {0x04, 0x0A, 0x11, 0x1F, 0x11}, // A
    {0x1E, 0x11, 0x1E, 0x11, 0x1E}, // B
    {0x0E, 0x11, 0x10, 0x11, 0x0E}, // C
    {0x1C, 0x12, 0x12, 0x12, 0x1C}, // D
    {0x1F, 0x10, 0x1E, 0x10, 0x1F}, // E
    {0x1F, 0x10, 0x1E, 0x10, 0x10}, // F
    {0x0E, 0x11, 0x10, 0x13, 0x0E}, // G
    {0x11, 0x11, 0x1F, 0x11, 0x11}, // H
    {0x0E, 0x04, 0x04, 0x04, 0x0E}, // I
    {0x01, 0x01, 0x01, 0x11, 0x0E}, // J
    {0x11, 0x12, 0x1C, 0x12, 0x11}, // K
    {0x10, 0x10, 0x10, 0x10, 0x1F}, // L
    {0x11, 0x1B, 0x15, 0x11, 0x11}, // M
    {0x11, 0x19, 0x15, 0x13, 0x11}, // N
    {0x0E, 0x11, 0x11, 0x11, 0x0E}, // O
    {0x1E, 0x11, 0x1E, 0x10, 0x10}, // P
    {0x0E, 0x11, 0x11, 0x12, 0x0D}, // Q
    {0x1E, 0x11, 0x1E, 0x12, 0x11}, // R
    {0x0F, 0x10, 0x0E, 0x01, 0x1E}, // S
    {0x1F, 0x04, 0x04, 0x04, 0x04}, // T
    {0x11, 0x11, 0x11, 0x11, 0x0E}, // U
    {0x11, 0x11, 0x11, 0x0A, 0x04}, // V
    {0x11, 0x11, 0x15, 0x15, 0x0A}, // W
    {0x11, 0x0A, 0x04, 0x0A, 0x11}, // X
    {0x11, 0x0A, 0x04, 0x04, 0x04}, // Y
    {0x1F, 0x02, 0x04, 0x08, 0x1F}, // Z
    {0x0E, 0x08, 0x08, 0x08, 0x0E}, // [
    {0x10, 0x08, 0x04, 0x02, 0x01}, // \ backslash
    {0x0E, 0x02, 0x02, 0x02, 0x0E}, // ]
    {0x04, 0x0A, 0x11, 0x00, 0x00}, // ^
    {0x00, 0x00, 0x00, 0x00, 0x1F}, // _
    {0x04, 0x02, 0x00, 0x00, 0x00}, // `
    {0x00, 0x0E, 0x11, 0x11, 0x0F}, // a
    {0x10, 0x1C, 0x12, 0x12, 0x1C}, // b
    {0x00, 0x0E, 0x10, 0x10, 0x0E}, // c
    {0x01, 0x0E, 0x12, 0x12, 0x0F}, // d
    {0x00, 0x0E, 0x1F, 0x10, 0x0E}, // e
    {0x06, 0x09, 0x1C, 0x08, 0x08}, // f
    {0x00, 0x0D, 0x13, 0x0D, 0x01}, // g
    {0x10, 0x16, 0x19, 0x11, 0x11}, // h
    {0x04, 0x00, 0x0C, 0x04, 0x0E}, // i
    {0x02, 0x00, 0x06, 0x02, 0x0C}, // j
    {0x10, 0x12, 0x14, 0x12, 0x11}, // k
    {0x0C, 0x04, 0x04, 0x04, 0x0E}, // l
    {0x00, 0x1A, 0x15, 0x15, 0x15}, // m
    {0x00, 0x16, 0x19, 0x11, 0x11}, // n
    {0x00, 0x0E, 0x11, 0x11, 0x0E}, // o
    {0x00, 0x1C, 0x12, 0x1C, 0x10}, // p
    {0x00, 0x0D, 0x12, 0x0F, 0x01}, // q
    {0x00, 0x16, 0x19, 0x10, 0x10}, // r
    {0x00, 0x0E, 0x10, 0x08, 0x1E}, // s
    {0x08, 0x1C, 0x08, 0x08, 0x06}, // t
    {0x00, 0x11, 0x11, 0x11, 0x0D}, // u
    {0x00, 0x11, 0x11, 0x0A, 0x04}, // v
    {0x00, 0x11, 0x15, 0x15, 0x0A}, // w
    {0x00, 0x11, 0x0A, 0x0A, 0x11}, // x
    {0x00, 0x11, 0x11, 0x0F, 0x01}, // y
    {0x00, 0x1F, 0x04, 0x08, 0x1F}, // z
    {0x02, 0x04, 0x04, 0x04, 0x02}, // {
    {0x04, 0x04, 0x04, 0x04, 0x04}, // |
    {0x08, 0x04, 0x04, 0x04, 0x08}, // }
    {0x00, 0x00, 0x00, 0x00, 0x00}, // ~ (not impl)
};

static void draw_pixel(uint32_t *buffer, int buf_w, int buf_h, int x, int y,
                       uint32_t color) {
  if (x >= 0 && x < buf_w && y >= 0 && y < buf_h) {
    buffer[y * buf_w + x] = color;
  }
}

static void draw_char(uint32_t *buffer, int buf_w, int buf_h, int x, int y,
                      char c, uint32_t color, double scale_factor,
                      double font_size_scale = 1.0) {
  if (c < 32 || c > 126)
    return;
  const uint8_t *glyph = font5x7[c - 32];

  double total_scale = scale_factor * font_size_scale;
  if (total_scale < 1.0)
    total_scale = 1.0;

  for (int row = 0; row < 5; ++row) {
    uint8_t bits = glyph[row];
    for (int col = 0; col < 5; ++col) {
      if ((bits >> (4 - col)) & 1) {
        for (int sy = 0; sy < (int)total_scale; ++sy) {
          for (int sx = 0; sx < (int)total_scale; ++sx) {
            draw_pixel(buffer, buf_w, buf_h, x + (int)(col * total_scale) + sx,
                       y + (int)(row * total_scale) + sy, color);
          }
        }
      }
    }
  }
}

// CSS Style structure
struct Style {
  uint32_t color;
  uint32_t bg_color;
  int font_size;
  bool has_bg;

  Style()
      : color(0xFF000000), bg_color(0xFFFFFFFF), font_size(16), has_bg(false) {}
};

struct StyleRule {
  std::string selector;
  Style style;
};

// Simple color map
uint32_t parse_color(const std::string &val) {
  if (val == "red")
    return 0xFFFF0000;
  if (val == "green")
    return 0xFF00FF00;
  if (val == "blue")
    return 0xFF0000FF;
  if (val == "black")
    return 0xFF000000;
  if (val == "white")
    return 0xFFFFFFFF;
  if (val == "gray")
    return 0xFF808080;
  // Fallback to black
  return 0xFF000000;
}

std::vector<StyleRule> parse_css(const std::string &css) {
  std::vector<StyleRule> rules;
  size_t pos = 0;
  while (pos < css.length()) {
    size_t brace_open = css.find('{', pos);
    if (brace_open == std::string::npos)
      break;

    std::string selector = css.substr(pos, brace_open - pos);
    // Trim selector
    size_t first = selector.find_first_not_of(" \n\r\t");
    size_t last = selector.find_last_not_of(" \n\r\t");
    if (first != std::string::npos)
      selector = selector.substr(first, (last - first + 1));

    size_t brace_close = css.find('}', brace_open);
    if (brace_close == std::string::npos)
      break;

    std::string body = css.substr(brace_open + 1, brace_close - brace_open - 1);
    Style style;

    size_t prop_pos = 0;
    while (prop_pos < body.length()) {
      size_t colon = body.find(':', prop_pos);
      if (colon == std::string::npos)
        break;
      size_t semi = body.find(';', colon);
      if (semi == std::string::npos)
        semi = body.length();

      std::string prop = body.substr(prop_pos, colon - prop_pos);
      std::string val = body.substr(colon + 1, semi - colon - 1);

      // Trim prop/val
      auto trim = [](std::string &s) {
        size_t f = s.find_first_not_of(" \n\r\t");
        size_t l = s.find_last_not_of(" \n\r\t");
        if (f != std::string::npos)
          s = s.substr(f, (l - f + 1));
        else
          s = "";
      };
      trim(prop);
      trim(val);

      if (prop == "color") {
        style.color = parse_color(val);
      } else if (prop == "background-color") {
        style.bg_color = parse_color(val);
        style.has_bg = true;
      } else if (prop == "font-size") {
        // Simple px parser
        if (val.find("px") != std::string::npos) {
          style.font_size = std::stoi(val.substr(0, val.find("px")));
        }
      }
      prop_pos = semi + 1;
    }

    rules.push_back({selector, style});
    pos = brace_close + 1;
  }
  return rules;
}

// Node structure for DOM tree
struct Node {
  std::string tag;
  std::string text;
  std::vector<std::shared_ptr<Node>> children;
  bool is_text;
  bool is_block;
  Style computed_style;

  Node() : is_text(false), is_block(false) {}
};

// Helper to clean whitespace - collapse sequence of whitespace to single space
std::string clean_whitespace(const std::string &input) {
  std::string result;
  bool space_seen = false;
  for (char c : input) {
    if (std::isspace(static_cast<unsigned char>(c))) {
      if (!space_seen) {
        result += ' ';
        space_seen = true;
      }
    } else {
      result += c;
      space_seen = false;
    }
  }
  return result;
}

// Simple parser that builds a DOM tree
std::shared_ptr<Node> parse_html(const std::string &html,
                                 std::vector<StyleRule> &out_styles) {
  auto root = std::make_shared<Node>();
  root->tag = "root";
  root->is_block = true; // Root is a block

  std::stack<std::shared_ptr<Node>> stack;
  stack.push(root);

  size_t pos = 0;
  while (pos < html.length()) {
    size_t lt = html.find('<', pos);

    // Add text before tag
    if (lt > pos || lt == std::string::npos) {
      std::string text_part = html.substr(
          pos, (lt == std::string::npos) ? std::string::npos : lt - pos);
      std::string cleaned = clean_whitespace(text_part);
      if (!cleaned.empty() && !(cleaned.size() == 1 && cleaned[0] == ' ')) {
        auto text_node = std::make_shared<Node>();
        text_node->is_text = true;
        text_node->text = cleaned;
        stack.top()->children.push_back(text_node);
      }
    }

    if (lt == std::string::npos)
      break;

    size_t gt = html.find('>', lt);
    if (gt == std::string::npos)
      break; // Error

    std::string tag_content = html.substr(lt + 1, gt - lt - 1);
    bool is_closing = (!tag_content.empty() && tag_content[0] == '/');

    if (is_closing) {
      if (stack.size() > 1) { // Never pop root
        stack.pop();
      }
    } else {
      // Opening tag
      std::string tag_name = tag_content;
      size_t space_pos = tag_name.find(' ');
      if (space_pos != std::string::npos) {
        tag_name = tag_name.substr(0, space_pos);
      }

      if (tag_name == "style") {
        size_t style_end = html.find("</style>", gt);
        if (style_end != std::string::npos) {
          std::string css = html.substr(gt + 1, style_end - gt - 1);
          auto rules = parse_css(css);
          out_styles.insert(out_styles.end(), rules.begin(), rules.end());
          pos = style_end + 8;
          continue;
        }
      }

      auto node = std::make_shared<Node>();
      node->tag = tag_name;

      // Determine display type
      if (tag_name == "div" || tag_name == "h1" || tag_name == "p" ||
          tag_name == "body" || tag_name == "html") {
        node->is_block = true;
      } else {
        node->is_block = false; // span, b, etc.
      }

      stack.top()->children.push_back(node);
      stack.push(node);
    }
    pos = gt + 1;
  }
  return root;
}

void apply_styles(std::shared_ptr<Node> node,
                  const std::vector<StyleRule> &rules) {
  if (!node || node->is_text)
    return;

  // Simple selector match
  for (const auto &rule : rules) {
    if (rule.selector == node->tag) {
      // Apply properties (simplified, no inheritance yet)
      node->computed_style.color = rule.style.color;
      if (rule.style.has_bg) {
        node->computed_style.bg_color = rule.style.bg_color;
        node->computed_style.has_bg = true;
      }
      if (rule.style.font_size != 16) {
        node->computed_style.font_size = rule.style.font_size;
      }
    }
  }

  // Default styles if not overridden
  if (node->tag == "h1" && node->computed_style.font_size == 16) {
    node->computed_style.font_size = 32;
  } else if (node->tag == "h2" && node->computed_style.font_size == 16) {
    node->computed_style.font_size = 24;
  }

  for (auto &child : node->children) {
    apply_styles(child, rules);
  }
}

void draw_rect(uint32_t *buffer, int buf_w, int buf_h, int x, int y, int w,
               int h, uint32_t color) {
  for (int j = 0; j < h; ++j) {
    for (int i = 0; i < w; ++i) {
      draw_pixel(buffer, buf_w, buf_h, x + i, y + j, color);
    }
  }
}

// Layout state
struct LayoutState {
  int x;
  int y;
  int width;
  int height;
  uint32_t *buffer;
  double scale_factor;
};

void render_node(std::shared_ptr<Node> node, LayoutState &state,
                 Style parent_style = Style()) {
  if (!node)
    return;

  Style current_style = parent_style;
  if (!node->is_text) {
    // Merge node style into current (simplified)
    if (node->computed_style.color != 0xFF000000)
      current_style.color = node->computed_style.color;
    if (node->computed_style.has_bg) {
      current_style.bg_color = node->computed_style.bg_color;
      current_style.has_bg = true;
    }
    if (node->computed_style.font_size != 12)
      current_style.font_size = node->computed_style.font_size;
  }

  if (node->is_text) {
    // Render text
    uint32_t color = current_style.color;

    // font_size is in CSS pixels.
    // Our base glyph is 5x7. Let's say a 16px font-size means the glyph height
    // should be ~16px. So scale for the glyph is font_size / 7.0.
    double font_scale = (double)current_style.font_size / 7.0;
    double total_scale = state.scale_factor * font_scale;

    int char_w = (int)(6.0 * total_scale);
    int char_h = (int)(9.0 * total_scale); // 7 + 2 for spacing

    for (char c : node->text) {
      draw_char(state.buffer, state.width, state.height, state.x, state.y, c,
                color, state.scale_factor, font_scale);
      state.x += char_w;
      if (state.x + char_w >= state.width) {
        state.x = 0;
        state.y += char_h;
      }
    }
  } else {
    // Element
    bool is_blk = node->is_block;

    double font_scale = (double)current_style.font_size / 7.0;
    int line_height =
        (int)(state.scale_factor * font_scale * 10.0); // 1.4ish line height

    // Block start logic
    if (is_blk) {
      if (state.x != 0) {
        state.x = 0;
        state.y += line_height;
      }
      // Simple background rect for block elements
      if (node->computed_style.has_bg) {
        draw_rect(state.buffer, state.width, state.height, state.x, state.y,
                  state.width, line_height, node->computed_style.bg_color);
      }
    }

    // Children
    for (auto &child : node->children) {
      render_node(child, state, current_style);
    }

    // Block end logic
    if (is_blk) {
      if (state.x != 0 || !node->children.empty()) {
        state.x = 0;
        state.y += line_height;
      }
    }
  }
}

extern "C" {
void render_frame(const char *html_cstr, uint32_t *buffer, int width,
                  int height, double scale_factor) {
  // Clear background
  for (int i = 0; i < width * height; ++i) {
    buffer[i] = 0xFFFFFFFF; // White
  }

  std::string html(html_cstr);
  std::vector<StyleRule> styles;
  auto root = parse_html(html, styles);
  apply_styles(root, styles);

  // Render Content
  LayoutState state;
  state.x = 0;
  state.y = 0; // Start at top
  state.width = width;
  state.height = height;
  state.buffer = buffer;
  state.scale_factor = scale_factor;

  render_node(root, state);
}
}
