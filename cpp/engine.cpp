#include "engine.hpp"
#include <cstring>


// Simple font data (5x7 bitmap) for 'H', 'e', 'l', 'o', etc. could be here,
// but for now we'll just fill the screen with a color based on the HTML content
// length to prove the link works. We will make it smarter later.

void fill_rect(uint32_t *buffer, int buf_w, int buf_h, int x, int y, int w,
               int h, uint32_t color) {
  for (int j = y; j < y + h; ++j) {
    if (j < 0 || j >= buf_h)
      continue;
    for (int i = x; i < x + w; ++i) {
      if (i < 0 || i >= buf_w)
        continue;
      buffer[j * buf_w + i] = color;
    }
  }
}

extern "C" {
void render_page(const char *html, uint32_t *buffer, int width, int height) {
  // Clear background to white
  for (int i = 0; i < width * height; ++i) {
    buffer[i] = 0xFFFFFFFF; // ARGB white
  }

  // Very basic parsing: look for "<h1>"
  const char *h1 = strstr(html, "<h1>");
  if (h1) {
    // Draw a red rectangle to represent H1
    fill_rect(buffer, width, height, 50, 50, 200, 50, 0xFFFF0000);
  }

  // Look for "<p>"
  const char *p = strstr(html, "<p>");
  if (p) {
    // Draw a blue rectangle to represent Paragraph
    fill_rect(buffer, width, height, 50, 120, 300, 100, 0xFF0000FF);
  }
}
}
