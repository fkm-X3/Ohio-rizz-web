
#include <cctype>
#include <cstdint>
#include <cstring>
#include <memory>
#include <stack>
#include <string>
#include <vector>

// A simple 5x7 bitmap font for ASCII 32..126
// Each line is a byte, bits 0..4 are pixels.
// We'll define a subset or a procedural generation for simplicity,
// but to be robust let's include a minimal set for the demo text.
// "Hello Browser" "This is rendered in C++ and Rust."

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
                      char c, uint32_t color) {
  if (c < 32 || c > 126)
    return;
  const uint8_t *glyph = font5x7[c - 32];

  for (int row = 0; row < 5; ++row) {
    int py = y + row;
    if (py < 0 || py >= buf_h)
      continue;

    uint8_t bits = glyph[row];
    for (int col = 0; col < 5; ++col) {
      if ((bits >> (4 - col)) & 1) {
        draw_pixel(buffer, buf_w, buf_h, x + col, py, color);
      }
    }
  }
}

// Node structure for DOM tree
struct Node {
  std::string tag;
  std::string text;
  std::vector<std::shared_ptr<Node>> children;
  bool is_text;
  bool is_block;

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
std::shared_ptr<Node> parse_html(const std::string &html) {
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
      if (!cleaned.empty() &&
          !(cleaned.size() == 1 &&
            cleaned[0] == ' ')) { // Skip empty/just space if desirable, but
                                  // actually space matters
        // For simplicity, add it. Logic in render can decide to skip
        // whitespace-only text at start of block if needed.
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
      // Handle parsing "div class=..." -> "div"
      size_t space_pos = tag_name.find(' ');
      if (space_pos != std::string::npos) {
        tag_name = tag_name.substr(0, space_pos);
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

      // Assume no void tags (like img, br) for this specific v0.2 spec or
      // handle them if needed. For v0.2 (div, span), all have closing tags.
      stack.push(node);
    }

    pos = gt + 1;
  }
  return root;
}

// Layout state
struct LayoutState {
  int x;
  int y;
  int width;
  int height;
  uint32_t *buffer;
};

void render_node(std::shared_ptr<Node> node, LayoutState &state) {
  if (!node)
    return;

  if (node->is_text) {
    // Render text
    uint32_t color = 0xFF000000; // Black text default
    // Simple word wrap or just overflow? Spec says "Simple box model (no
    // margins yet)" Let's implement character iteration.
    for (char c : node->text) {
      draw_char(state.buffer, state.width, state.height, state.x, state.y, c,
                color);
      state.x += 6; // advance curser
      // Simple wrap
      if (state.x + 6 >= state.width) {
        state.x = 0;
        state.y += 8; // Line height
      }
    }
  } else {
    // Element
    bool is_blk = node->is_block;

    // Block start logic
    if (is_blk) {
      if (state.x != 0) {
        state.x = 0;
        state.y += 8;
      }
    }

    // Children
    for (auto &child : node->children) {
      render_node(child, state);
    }

    // Block end logic
    if (is_blk) {
      if (state.x != 0) {
        state.x = 0;
        state.y += 8;
      }
    }
  }
}

// UI Constants
const int UI_HEIGHT = 50;
const uint32_t UI_BG_COLOR = 0xFFCCCCCC;     // Light Gray
const uint32_t UI_TEXT_COLOR = 0xFF000000;   // Black
const uint32_t BUTTON_BG_COLOR = 0xFFAAAAAA; // Darker Gray

void draw_rect(uint32_t *buffer, int buf_w, int buf_h, int x, int y, int w,
               int h, uint32_t color) {
  for (int j = 0; j < h; ++j) {
    for (int i = 0; i < w; ++i) {
      draw_pixel(buffer, buf_w, buf_h, x + i, y + j, color);
    }
  }
}

void draw_text(uint32_t *buffer, int buf_w, int buf_h, int x, int y,
               const std::string &text, uint32_t color) {
  int cur_x = x;
  for (char c : text) {
    draw_char(buffer, buf_w, buf_h, cur_x, y, c, color);
    cur_x += 6;
  }
}

void render_ui(uint32_t *buffer, int width, int height,
               const std::string &url_str) {
  // 1. Background
  draw_rect(buffer, width, height, 0, 0, width, UI_HEIGHT, UI_BG_COLOR);

  // 2. Buttons
  // [ < ] [ > ] [ R ]
  int btn_y = 10;
  int btn_h = 30;
  int btn_w = 30;
  int spacing = 10;
  int x = 10;

  // Back
  draw_rect(buffer, width, height, x, btn_y, btn_w, btn_h, BUTTON_BG_COLOR);
  draw_text(buffer, width, height, x + 11, btn_y + 11, "<", UI_TEXT_COLOR);
  x += btn_w + spacing;

  // Forward
  draw_rect(buffer, width, height, x, btn_y, btn_w, btn_h, BUTTON_BG_COLOR);
  draw_text(buffer, width, height, x + 11, btn_y + 11, ">", UI_TEXT_COLOR);
  x += btn_w + spacing;

  // Refresh
  draw_rect(buffer, width, height, x, btn_y, btn_w, btn_h, BUTTON_BG_COLOR);
  draw_text(buffer, width, height, x + 11, btn_y + 11, "R", UI_TEXT_COLOR);
  x += btn_w + spacing;

  // 3. Address Bar
  int url_x = x;
  int url_w = width - x - 10;
  draw_rect(buffer, width, height, url_x, btn_y, url_w, btn_h,
            0xFFFFFFFF); // White bg
  draw_text(buffer, width, height, url_x + 5, btn_y + 11, url_str,
            UI_TEXT_COLOR);

  // Bottom border
  draw_rect(buffer, width, height, 0, UI_HEIGHT - 1, width, 1, 0xFF000000);
}

extern "C" {
void render_frame(const char *html_cstr, uint32_t *buffer, int width,
                  int height) {
  // Clear background
  for (int i = 0; i < width * height; ++i) {
    buffer[i] = 0xFFFFFFFF; // White
  }

  // Render content first? Or UI first? Layers.
  // Let's parse content.
  std::string html(html_cstr);
  auto root = parse_html(html);

  // Render Content
  LayoutState state;
  state.x = 0;
  state.y = UI_HEIGHT + 10; // Start below UI
  state.width = width;
  state.height = height;
  state.buffer = buffer;

  render_node(root, state);

  // Render UI on top
  // In a real browser, UI might be a separate window or layer,
  // but here we just draw over the buffer.
  // Actually, we want to ensure content doesn't draw OVER the UI
  // if we scroll (not implemented yet), so drawing UI last is safer
  // if content is simply clipped by window bounds.
  // But we have state.y starting below UI, so it shouldn't overlap
  // unless we had negative margins.
  // Let's draw UI last to be sure it's on top.
  render_ui(buffer, width, height, "verify.html");
}
}
