#include <cctype>
#include <cstdint>
#include <cstring>
#include <map>
#include <memory>
#include <stack>

#include <string>
#include <vector>

static const uint8_t font8x16[95][16] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // space
    {0x00, 0x00, 0x18, 0x3C, 0x3C, 0x3C, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18,
     0x00, 0x00, 0x00, 0x00}, // !
    {0x00, 0x66, 0x66, 0x66, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // "
    {0x00, 0x00, 0x00, 0x6C, 0x6C, 0xFE, 0x6C, 0x6C, 0xFE, 0x6C, 0x6C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // #
    {0x18, 0x18, 0x7C, 0xC6, 0xC2, 0x78, 0x06, 0x06, 0xC6, 0x7C, 0x18, 0x18,
     0x00, 0x00, 0x00, 0x00}, // $
    {0x00, 0x00, 0x00, 0x00, 0xC2, 0xC6, 0x0C, 0x18, 0x30, 0x60, 0xC6, 0x86,
     0x00, 0x00, 0x00, 0x00}, // %
    {0x00, 0x00, 0x38, 0x6C, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0xCC, 0x76, 0x00,
     0x00, 0x00, 0x00, 0x00}, // &
    {0x00, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // '
    {0x00, 0x00, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0C,
     0x00, 0x00, 0x00, 0x00}, // (
    {0x00, 0x00, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18, 0x30,
     0x00, 0x00, 0x00, 0x00}, // )
    {0x00, 0x00, 0x00, 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // *
    {0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x18, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // +
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x08,
     0x10, 0x00, 0x00, 0x00}, // ,
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // -
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00,
     0x00, 0x00, 0x00, 0x00}, // .
    {0x00, 0x00, 0x00, 0x02, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00,
     0x00, 0x00, 0x00, 0x00}, // /
    {0x00, 0x00, 0x3C, 0x66, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // 0
    {0x00, 0x00, 0x18, 0x38, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00,
     0x00, 0x00, 0x00, 0x00}, // 1
    {0x00, 0x00, 0x3C, 0x66, 0x06, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x7E, 0x00,
     0x00, 0x00, 0x00, 0x00}, // 2
    {0x00, 0x00, 0x3C, 0x66, 0x06, 0x1C, 0x06, 0x06, 0x06, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // 3
    {0x00, 0x00, 0x06, 0x0E, 0x1E, 0x36, 0x66, 0x66, 0x7F, 0x06, 0x06, 0x00,
     0x00, 0x00, 0x00, 0x00}, // 4
    {0x00, 0x00, 0x7E, 0x60, 0x60, 0x7C, 0x06, 0x06, 0x06, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // 5
    {0x00, 0x00, 0x1C, 0x30, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // 6
    {0x00, 0x00, 0x7E, 0x06, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x00,
     0x00, 0x00, 0x00, 0x00}, // 7
    {0x00, 0x00, 0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // 8
    {0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x0C, 0x18, 0x70, 0x00,
     0x00, 0x00, 0x00, 0x00}, // 9
    {0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // :
    {0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x08, 0x10,
     0x00, 0x00, 0x00, 0x00}, // ;
    {0x00, 0x00, 0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // <
    {0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // =
    {0x00, 0x00, 0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // >
    {0x00, 0x00, 0x3C, 0x66, 0x06, 0x0C, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00,
     0x00, 0x00, 0x00, 0x00}, // ?
    {0x00, 0x00, 0x3C, 0x66, 0x66, 0x6E, 0x6E, 0x60, 0x62, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // @
    {0x00, 0x00, 0x18, 0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x66, 0x00,
     0x00, 0x00, 0x00, 0x00}, // A
    {0x00, 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // B
    {0x00, 0x00, 0x3C, 0x66, 0x60, 0x60, 0x60, 0x60, 0x60, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // C
    {0x00, 0x00, 0x78, 0x6C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6C, 0x78, 0x00,
     0x00, 0x00, 0x00, 0x00}, // D
    {0x00, 0x00, 0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00,
     0x00, 0x00, 0x00, 0x00}, // E
    {0x00, 0x00, 0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00,
     0x00, 0x00, 0x00, 0x00}, // F
    {0x00, 0x00, 0x3C, 0x66, 0x60, 0x60, 0x6E, 0x66, 0x66, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // G
    {0x00, 0x00, 0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00,
     0x00, 0x00, 0x00, 0x00}, // H
    {0x00, 0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // I
    {0x00, 0x00, 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // J
    {0x00, 0x00, 0x66, 0x6C, 0x78, 0x70, 0x78, 0x6C, 0x66, 0x66, 0x66, 0x00,
     0x00, 0x00, 0x00, 0x00}, // K
    {0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00,
     0x00, 0x00, 0x00, 0x00}, // L
    {0x00, 0x00, 0x63, 0x63, 0x77, 0x7F, 0x6B, 0x63, 0x63, 0x63, 0x63, 0x00,
     0x00, 0x00, 0x00, 0x00}, // M
    {0x00, 0x00, 0x66, 0x66, 0x76, 0x7E, 0x7E, 0x6E, 0x66, 0x66, 0x66, 0x00,
     0x00, 0x00, 0x00, 0x00}, // N
    {0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // O
    {0x00, 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00,
     0x00, 0x00, 0x00, 0x00}, // P
    {0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6E, 0x3C, 0x0E,
     0x00, 0x00, 0x00, 0x00}, // Q
    {0x00, 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x78, 0x6C, 0x66, 0x66, 0x66, 0x00,
     0x00, 0x00, 0x00, 0x00}, // R
    {0x00, 0x00, 0x3C, 0x66, 0x60, 0x3C, 0x06, 0x06, 0x06, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // S
    {0x00, 0x00, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00,
     0x00, 0x00, 0x00, 0x00}, // T
    {0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // U
    {0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x00,
     0x00, 0x00, 0x00, 0x00}, // V
    {0x00, 0x00, 0x63, 0x63, 0x63, 0x6B, 0x6B, 0x7F, 0x77, 0x63, 0x63, 0x00,
     0x00, 0x00, 0x00, 0x00}, // W
    {0x00, 0x00, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x3C, 0x66, 0x66, 0x00,
     0x00, 0x00, 0x00, 0x00}, // X
    {0x00, 0x00, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00,
     0x00, 0x00, 0x00, 0x00}, // Y
    {0x00, 0x00, 0x7E, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x40, 0x7E, 0x00,
     0x00, 0x00, 0x00, 0x00}, // Z
    {0x00, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // [
    {0x00, 0x00, 0x00, 0x80, 0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00,
     0x00, 0x00, 0x00, 0x00}, // \ backslash
    {0x00, 0x00, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // ]
    {0x00, 0x18, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // ^
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
     0x00, 0x00, 0x00, 0x00}, // _
    {0x00, 0x30, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // `
    {0x00, 0x00, 0x00, 0x00, 0x3C, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x3E, 0x00,
     0x00, 0x00, 0x00, 0x00}, // a
    {0x00, 0x00, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // b
    {0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // c
    {0x00, 0x00, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x00,
     0x00, 0x00, 0x00, 0x00}, // d
    {0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x7E, 0x60, 0x60, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // e
    {0x00, 0x00, 0x1C, 0x30, 0x30, 0x7C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00,
     0x00, 0x00, 0x00, 0x00}, // f
    {0x00, 0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // g
    {0x00, 0x00, 0x60, 0x60, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00,
     0x00, 0x00, 0x00, 0x00}, // h
    {0x00, 0x00, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // i
    {0x00, 0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // j
    {0x00, 0x00, 0x60, 0x60, 0x66, 0x6C, 0x78, 0x70, 0x78, 0x6C, 0x66, 0x00,
     0x00, 0x00, 0x00, 0x00}, // k
    {0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x1C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // l
    {0x00, 0x00, 0x00, 0x00, 0x66, 0x7F, 0x6B, 0x63, 0x63, 0x63, 0x63, 0x00,
     0x00, 0x00, 0x00, 0x00}, // m
    {0x00, 0x00, 0x00, 0x00, 0x7C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00,
     0x00, 0x00, 0x00, 0x00}, // n
    {0x00, 0x00, 0x00, 0x00, 0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // o
    {0x00, 0x00, 0x00, 0x00, 0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x00,
     0x00, 0x00, 0x00, 0x00}, // p
    {0x00, 0x00, 0x00, 0x00, 0x3E, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x06, 0x00,
     0x00, 0x00, 0x00, 0x00}, // q
    {0x00, 0x00, 0x00, 0x00, 0x7C, 0x66, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00,
     0x00, 0x00, 0x00, 0x00}, // r
    {0x00, 0x00, 0x00, 0x00, 0x3E, 0x60, 0x3C, 0x06, 0x3E, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // s
    {0x00, 0x00, 0x30, 0x30, 0x7E, 0x30, 0x30, 0x30, 0x30, 0x30, 0x1C, 0x00,
     0x00, 0x00, 0x00, 0x00}, // t
    {0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3E, 0x00,
     0x00, 0x00, 0x00, 0x00}, // u
    {0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x00,
     0x00, 0x00, 0x00, 0x00}, // v
    {0x00, 0x00, 0x00, 0x00, 0x63, 0x6B, 0x6B, 0x7F, 0x77, 0x63, 0x63, 0x00,
     0x00, 0x00, 0x00, 0x00}, // w
    {0x00, 0x00, 0x00, 0x00, 0x66, 0x3C, 0x18, 0x18, 0x3C, 0x66, 0x66, 0x00,
     0x00, 0x00, 0x00, 0x00}, // x
    {0x00, 0x00, 0x00, 0x00, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x0C, 0x38, 0x00,
     0x00, 0x00, 0x00, 0x00}, // y
    {0x00, 0x00, 0x00, 0x00, 0x7E, 0x0C, 0x18, 0x30, 0x60, 0x7E, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // z
    {0x00, 0x00, 0x0E, 0x18, 0x18, 0x70, 0x18, 0x18, 0x18, 0x18, 0x0E, 0x00,
     0x00, 0x00, 0x00, 0x00}, // {
    {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
     0x00, 0x00, 0x00, 0x00}, // |
    {0x00, 0x00, 0x70, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x18, 0x18, 0x70, 0x00,
     0x00, 0x00, 0x00, 0x00}, // }
    {0x00, 0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00}, // ~
};

static void draw_pixel(uint32_t *buffer, int buf_w, int buf_h, int x, int y,
                       uint32_t color) {
  if (x >= 0 && x < buf_w && y >= 0 && y < buf_h) {
    buffer[y * buf_w + x] = color;
  }
}

static void draw_char(uint32_t *buffer, int buf_w, int buf_h, int x, int y,
                      char c, uint32_t color, double scale_factor,
                      double font_size_scale = 1.0, bool bold = false,
                      bool italic = false) {
  if (c < 32 || c > 126)
    return;
  const uint8_t *glyph = font8x16[c - 32];

  double total_scale = scale_factor * font_size_scale;
  if (total_scale < 0.1)
    total_scale = 0.1;

  for (int row = 0; row < 16; ++row) {
    uint8_t bits = glyph[row];
    for (int col = 0; col < 8; ++col) {
      if ((bits >> (7 - col)) & 1) {
        // Skew for italics: move horizontal offset based on row
        int italic_offset = italic ? (int)((15 - row) * total_scale * 0.2) : 0;

        // Calculate destination rectangle for this glyph pixel
        int x_start = x + (int)(col * total_scale) + italic_offset;
        int y_start = y + (int)(row * total_scale);
        int x_end = x + (int)((col + 1) * total_scale) + italic_offset;
        int y_end = y + (int)((row + 1) * total_scale);

        for (int py = y_start; py < y_end; ++py) {
          for (int px = x_start; px < x_end; ++px) {
            draw_pixel(buffer, buf_w, buf_h, px, py, color);
          }
        }

        // Bold: Draw an extra column to the right
        if (bold) {
          int bold_extra = (int)std::max(1.0, total_scale / 2.0);
          for (int py = y_start; py < y_end; ++py) {
            for (int px = x_end; px < x_end + bold_extra; ++px) {
              draw_pixel(buffer, buf_w, buf_h, px, py, color);
            }
          }
        }
      }
    }
  }
}

// Rect structure for layout
struct Rect {
  int x, y, w, h;
  Rect() : x(0), y(0), w(0), h(0) {}
};

// CSS Style structure
struct Style {
  uint32_t color;
  uint32_t bg_color;
  int font_size;
  bool has_bg;

  // Box model properties
  int margin_top, margin_right, margin_bottom, margin_left;
  int padding_top, padding_right, padding_bottom, padding_left;
  int border_top, border_right, border_bottom, border_left;
  uint32_t border_color;
  int width, height; // -1 means auto

  // Text properties
  bool font_bold;
  bool font_italic;
  std::string text_align; // "left", "center", "right"
  double line_height;

  // Bitmask for set properties
  uint64_t set_fields;
  enum Fields {
    F_COLOR = 1 << 0,
    F_BG_COLOR = 1 << 1,
    F_FONT_SIZE = 1 << 2,
    F_MARGIN_TOP = 1 << 3,
    F_MARGIN_RIGHT = 1 << 4,
    F_MARGIN_BOTTOM = 1 << 5,
    F_MARGIN_LEFT = 1 << 6,
    F_PADDING_TOP = 1 << 7,
    F_PADDING_RIGHT = 1 << 8,
    F_PADDING_BOTTOM = 1 << 9,
    F_PADDING_LEFT = 1 << 10,
    F_BORDER_TOP = 1 << 11,
    F_BORDER_RIGHT = 1 << 12,
    F_BORDER_BOTTOM = 1 << 13,
    F_BORDER_LEFT = 1 << 14,
    F_BORDER_COLOR = 1 << 15,
    F_WIDTH = 1 << 16,
    F_HEIGHT = 1 << 17,
    F_FONT_BOLD = 1 << 18,
    F_FONT_ITALIC = 1 << 19,
    F_TEXT_ALIGN = 1 << 20,
    F_LINE_HEIGHT = 1 << 21
  };

  Style()
      : color(0xFF000000), bg_color(0xFFFFFFFF), font_size(16), has_bg(false),
        margin_top(0), margin_right(0), margin_bottom(0), margin_left(0),
        padding_top(0), padding_right(0), padding_bottom(0), padding_left(0),
        border_top(0), border_right(0), border_bottom(0), border_left(0),
        border_color(0xFF000000), width(-1), height(-1), font_bold(false),
        font_italic(false), text_align("left"), line_height(1.4),
        set_fields(0) {}
};

struct StyleRule {
  std::string selector;
  Style style;
};

// Simple color map
uint32_t parse_color(const std::string &val) {
  std::string lower = val;
  for (auto &c : lower)
    c = std::tolower(c);

  if (lower[0] == '#' && lower.length() == 7) {
    try {
      uint32_t r = std::stoul(lower.substr(1, 2), nullptr, 16);
      uint32_t g = std::stoul(lower.substr(3, 2), nullptr, 16);
      uint32_t b = std::stoul(lower.substr(5, 2), nullptr, 16);
      return 0xFF000000 | (r << 16) | (g << 8) | b;
    } catch (...) {
      return 0xFF000000;
    }
  }

  if (lower == "red")
    return 0xFFFF0000;
  if (lower == "green")
    return 0xFF00FF00;
  if (lower == "blue")
    return 0xFF0000FF;
  if (lower == "black")
    return 0xFF000000;
  if (lower == "white")
    return 0xFFFFFFFF;
  if (lower == "gray" || lower == "grey")
    return 0xFF808080;
  if (lower == "yellow")
    return 0xFFFFFF00;
  if (lower == "cyan")
    return 0xFF00FFFF;
  if (lower == "magenta")
    return 0xFFFF00FF;
  if (lower == "orange")
    return 0xFFFFA500;
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
        style.set_fields |= Style::F_COLOR;
      } else if (prop == "background-color") {
        style.bg_color = parse_color(val);
        style.has_bg = true;
        style.set_fields |= Style::F_BG_COLOR;
      } else if (prop == "font-size") {
        if (val.find("px") != std::string::npos) {
          style.font_size = std::stoi(val.substr(0, val.find("px")));
          style.set_fields |= Style::F_FONT_SIZE;
        }
      } else if (prop == "margin") {
        if (val.find("px") != std::string::npos) {
          int m = std::stoi(val.substr(0, val.find("px")));
          style.margin_top = style.margin_right = style.margin_bottom =
              style.margin_left = m;
          style.set_fields |= Style::F_MARGIN_TOP | Style::F_MARGIN_RIGHT |
                              Style::F_MARGIN_BOTTOM | Style::F_MARGIN_LEFT;
        }
      } else if (prop == "padding") {
        if (val.find("px") != std::string::npos) {
          int p = std::stoi(val.substr(0, val.find("px")));
          style.padding_top = style.padding_right = style.padding_bottom =
              style.padding_left = p;
          style.set_fields |= Style::F_PADDING_TOP | Style::F_PADDING_RIGHT |
                              Style::F_PADDING_BOTTOM | Style::F_PADDING_LEFT;
        }
      } else if (prop == "border-width") {
        if (val.find("px") != std::string::npos) {
          int b = std::stoi(val.substr(0, val.find("px")));
          style.border_top = style.border_right = style.border_bottom =
              style.border_left = b;
          style.set_fields |= Style::F_BORDER_TOP | Style::F_BORDER_RIGHT |
                              Style::F_BORDER_BOTTOM | Style::F_BORDER_LEFT;
        }
      } else if (prop == "border-color") {
        style.border_color = parse_color(val);
        style.set_fields |= Style::F_BORDER_COLOR;
      } else if (prop == "width") {
        if (val.find("px") != std::string::npos) {
          style.width = std::stoi(val.substr(0, val.find("px")));
          style.set_fields |= Style::F_WIDTH;
        }
      } else if (prop == "height") {
        if (val.find("px") != std::string::npos) {
          style.height = std::stoi(val.substr(0, val.find("px")));
          style.set_fields |= Style::F_HEIGHT;
        }
      } else if (prop == "font-weight") {
        style.font_bold = (val == "bold");
        style.set_fields |= Style::F_FONT_BOLD;
      } else if (prop == "font-style") {
        style.font_italic = (val == "italic");
        style.set_fields |= Style::F_FONT_ITALIC;
      } else if (prop == "text-align") {
        style.text_align = val;
        style.set_fields |= Style::F_TEXT_ALIGN;
      } else if (prop == "line-height") {
        try {
          style.line_height = std::stod(val);
          style.set_fields |= Style::F_LINE_HEIGHT;
        } catch (...) {
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
  std::weak_ptr<Node> parent; // Parent pointer for bubbling

  // attributes
  std::string id;
  std::vector<std::string> classList;
  std::string inline_style;
  std::string href; // Link URL

  // Layout results
  Rect margin_box;
  Rect border_box;
  Rect padding_box;
  Rect content_box;

  // For text nodes: list of lines (x_offset, text_start, text_length)
  struct LineInfo {
    int x;
    int y;
    int width;
    size_t start;
    size_t length;
  };
  std::vector<LineInfo> text_lines;

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
      if (!cleaned.empty()) {
        auto text_node = std::make_shared<Node>();
        text_node->is_text = true;
        text_node->text = cleaned;
        text_node->parent = stack.top();
        stack.top()->children.push_back(text_node);
      }
    }

    if (lt == std::string::npos)
      break;

    size_t gt = html.find('>', lt);
    if (gt == std::string::npos)
      break; // Error

    std::string tag_content = html.substr(lt + 1, gt - lt - 1);
    if (tag_content.empty()) {
      pos = gt + 1;
      continue;
    }

    if (tag_content[0] == '!') { // Skip Doctypes/comments
      pos = gt + 1;
      continue;
    }

    bool is_closing = (tag_content[0] == '/');

    if (is_closing) {
      if (stack.size() > 1) { // Never pop root
        stack.pop();
      }
    } else {
      // Opening tag
      auto node = std::make_shared<Node>();

      // Parse tag name and attributes
      size_t space_pos = tag_content.find(' ');
      std::string tag_name;
      if (space_pos == std::string::npos) {
        tag_name = tag_content;
      } else {
        tag_name = tag_content.substr(0, space_pos);
        std::string attrs = tag_content.substr(space_pos + 1);

        // Very basic attribute parsing
        auto extract_attr = [&](const std::string &name) {
          size_t start = attrs.find(name + "=\"");
          if (start != std::string::npos) {
            start += name.length() + 2;
            size_t end = attrs.find('"', start);
            if (end != std::string::npos) {
              return attrs.substr(start, end - start);
            }
          }
          return std::string("");
        };

        node->id = extract_attr("id");
        std::string classes = extract_attr("class");
        if (!classes.empty()) {
          size_t c_pos = 0;
          while (c_pos < classes.length()) {
            size_t c_space = classes.find(' ', c_pos);
            std::string c = (c_space == std::string::npos)
                                ? classes.substr(c_pos)
                                : classes.substr(c_pos, c_space - c_pos);
            if (!c.empty())
              node->classList.push_back(c);
            if (c_space == std::string::npos)
              break;
            c_pos = c_space + 1;
          }
        }
        node->inline_style = extract_attr("style");
        node->href = extract_attr("href");
      }

      node->tag = tag_name;
      node->parent = stack.top();

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

      // Determine display type
      if (tag_name == "div" || tag_name == "h1" || tag_name == "p" ||
          tag_name == "body" || tag_name == "html") {
        node->is_block = true;
      } else {
        node->is_block = false; // span, b, a, etc.
      }

      stack.top()->children.push_back(node);

      // Self-closing tags list
      if (tag_name != "img" && tag_name != "br" && tag_name != "hr" &&
          tag_name != "meta" && tag_name != "link") {
        stack.push(node);
      }
    }
    pos = gt + 1;
  }
  return root;
}

// Helper to parse custom body-like CSS strings into a Style object
void parse_inline_style(const std::string &body, Style &style, double scale) {
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

    auto get_scaled_px = [&](const std::string &v) {
      if (v.find("px") != std::string::npos) {
        return (int)(std::stoi(v.substr(0, v.find("px"))) * scale);
      }
      return -1;
    };

    if (prop == "color") {
      style.color = parse_color(val);
      style.set_fields |= Style::F_COLOR;
    } else if (prop == "background-color") {
      style.bg_color = parse_color(val);
      style.has_bg = true;
      style.set_fields |= Style::F_BG_COLOR;
    } else if (prop == "font-size") {
      int s = get_scaled_px(val);
      if (s != -1) {
        style.font_size = s;
        style.set_fields |= Style::F_FONT_SIZE;
      }
    } else if (prop == "margin") {
      int m = get_scaled_px(val);
      if (m != -1) {
        style.margin_top = style.margin_right = style.margin_bottom =
            style.margin_left = m;
        style.set_fields |= Style::F_MARGIN_TOP | Style::F_MARGIN_RIGHT |
                            Style::F_MARGIN_BOTTOM | Style::F_MARGIN_LEFT;
      }
    } else if (prop == "margin-top") {
      int m = get_scaled_px(val);
      if (m != -1) {
        style.margin_top = m;
        style.set_fields |= Style::F_MARGIN_TOP;
      }
    } else if (prop == "padding") {
      int p = get_scaled_px(val);
      if (p != -1) {
        style.padding_top = style.padding_right = style.padding_bottom =
            style.padding_left = p;
        style.set_fields |= Style::F_PADDING_TOP | Style::F_PADDING_RIGHT |
                            Style::F_PADDING_BOTTOM | Style::F_PADDING_LEFT;
      }
    } else if (prop == "border-width") {
      int b = get_scaled_px(val);
      if (b != -1) {
        style.border_top = style.border_right = style.border_bottom =
            style.border_left = b;
        style.set_fields |= Style::F_BORDER_TOP | Style::F_BORDER_RIGHT |
                            Style::F_BORDER_BOTTOM | Style::F_BORDER_LEFT;
      }
    } else if (prop == "border-color") {
      style.border_color = parse_color(val);
      style.set_fields |= Style::F_BORDER_COLOR;
    } else if (prop == "width") {
      int w = get_scaled_px(val);
      if (w != -1) {
        style.width = w;
        style.set_fields |= Style::F_WIDTH;
      }
    } else if (prop == "height") {
      int h = get_scaled_px(val);
      if (h != -1) {
        style.height = h;
        style.set_fields |= Style::F_HEIGHT;
      }
    } else if (prop == "font-weight") {
      style.font_bold = (val == "bold");
      style.set_fields |= Style::F_FONT_BOLD;
    } else if (prop == "font-style") {
      style.font_italic = (val == "italic");
      style.set_fields |= Style::F_FONT_ITALIC;
    } else if (prop == "text-align") {
      style.text_align = val;
      style.set_fields |= Style::F_TEXT_ALIGN;
    } else if (prop == "line-height") {
      try {
        style.line_height = std::stod(val);
        style.set_fields |= Style::F_LINE_HEIGHT;
      } catch (...) {
      }
    }
    prop_pos = semi + 1;
  }
}

void apply_styles(std::shared_ptr<Node> node,
                  const std::vector<StyleRule> &rules, double scale,
                  Style parent_style) {
  if (!node || node->is_text)
    return;

  // Inherit some properties
  node->computed_style.color = parent_style.color;
  node->computed_style.text_align = parent_style.text_align;
  node->computed_style.line_height = parent_style.line_height;
  node->computed_style.font_size = parent_style.font_size;
  node->computed_style.font_bold = parent_style.font_bold;
  node->computed_style.font_italic = parent_style.font_italic;

  // Precedence: Tag < Class < ID < Inline
  auto apply_rule = [&](const Style &rule_style) {
    auto scale_val = [&](int v) {
      return (v == -1 || v == 0) ? v : (int)(v * scale);
    };

    if (rule_style.set_fields & Style::F_COLOR)
      node->computed_style.color = rule_style.color;
    if (rule_style.set_fields & Style::F_BG_COLOR) {
      node->computed_style.bg_color = rule_style.bg_color;
      node->computed_style.has_bg = true;
    }

    if (rule_style.set_fields & Style::F_FONT_SIZE)
      node->computed_style.font_size = (int)(rule_style.font_size * scale);

    if (rule_style.set_fields & Style::F_MARGIN_TOP)
      node->computed_style.margin_top = scale_val(rule_style.margin_top);
    if (rule_style.set_fields & Style::F_MARGIN_RIGHT)
      node->computed_style.margin_right = scale_val(rule_style.margin_right);
    if (rule_style.set_fields & Style::F_MARGIN_BOTTOM)
      node->computed_style.margin_bottom = scale_val(rule_style.margin_bottom);
    if (rule_style.set_fields & Style::F_MARGIN_LEFT)
      node->computed_style.margin_left = scale_val(rule_style.margin_left);

    if (rule_style.set_fields & Style::F_PADDING_TOP)
      node->computed_style.padding_top = scale_val(rule_style.padding_top);
    if (rule_style.set_fields & Style::F_PADDING_RIGHT)
      node->computed_style.padding_right = scale_val(rule_style.padding_right);
    if (rule_style.set_fields & Style::F_PADDING_BOTTOM)
      node->computed_style.padding_bottom =
          scale_val(rule_style.padding_bottom);
    if (rule_style.set_fields & Style::F_PADDING_LEFT)
      node->computed_style.padding_left = scale_val(rule_style.padding_left);

    if (rule_style.set_fields & Style::F_BORDER_TOP)
      node->computed_style.border_top = scale_val(rule_style.border_top);
    if (rule_style.set_fields & Style::F_BORDER_RIGHT)
      node->computed_style.border_right = scale_val(rule_style.border_right);
    if (rule_style.set_fields & Style::F_BORDER_BOTTOM)
      node->computed_style.border_bottom = scale_val(rule_style.border_bottom);
    if (rule_style.set_fields & Style::F_BORDER_LEFT)
      node->computed_style.border_left = scale_val(rule_style.border_left);
    if (rule_style.set_fields & Style::F_BORDER_COLOR)
      node->computed_style.border_color = rule_style.border_color;

    if (rule_style.set_fields & Style::F_WIDTH)
      node->computed_style.width = scale_val(rule_style.width);
    if (rule_style.set_fields & Style::F_HEIGHT)
      node->computed_style.height = scale_val(rule_style.height);

    if (rule_style.set_fields & Style::F_FONT_BOLD)
      node->computed_style.font_bold = rule_style.font_bold;
    if (rule_style.set_fields & Style::F_FONT_ITALIC)
      node->computed_style.font_italic = rule_style.font_italic;
    if (rule_style.set_fields & Style::F_TEXT_ALIGN)
      node->computed_style.text_align = rule_style.text_align;
    if (rule_style.set_fields & Style::F_LINE_HEIGHT)
      node->computed_style.line_height = rule_style.line_height;
  };

  // 1. Tag matches
  for (const auto &rule : rules) {
    if (rule.selector == node->tag) {
      apply_rule(rule.style);
    }
  }

  // 2. Class matches
  for (const auto &rule : rules) {
    if (!rule.selector.empty() && rule.selector[0] == '.') {
      std::string target_class = rule.selector.substr(1);
      for (const auto &c : node->classList) {
        if (c == target_class) {
          apply_rule(rule.style);
          break;
        }
      }
    }
    // Also handle tag.class
    size_t dot_pos = rule.selector.find('.');
    if (dot_pos != std::string::npos && dot_pos > 0) {
      std::string tag = rule.selector.substr(0, dot_pos);
      std::string cls = rule.selector.substr(dot_pos + 1);
      if (tag == node->tag) {
        for (const auto &c : node->classList) {
          if (c == cls) {
            apply_rule(rule.style);
            break;
          }
        }
      }
    }
  }

  // 3. ID matches
  for (const auto &rule : rules) {
    if (!rule.selector.empty() && rule.selector[0] == '#') {
      if (node->id == rule.selector.substr(1)) {
        apply_rule(rule.style);
      }
    }
  }

  // 4. Inline styles (pass scale here)
  if (!node->inline_style.empty()) {
    parse_inline_style(node->inline_style, node->computed_style, scale);
  }

  // Default styles if not overridden
  if (node->tag == "h1") {
    // Already scaled font_size if it was set by rules, but let's ensure
    // defaults are scaled.
    if (node->computed_style.font_size == (int)(16 * scale) ||
        node->computed_style.font_size == 16)
      node->computed_style.font_size = (int)(32 * scale);
    node->computed_style.font_bold = true;
  } else if (node->tag == "h2") {
    if (node->computed_style.font_size == (int)(16 * scale) ||
        node->computed_style.font_size == 16)
      node->computed_style.font_size = (int)(24 * scale);
    node->computed_style.font_bold = true;
  } else if (node->tag == "strong" || node->tag == "b") {
    node->computed_style.font_bold = true;
  } else if (node->tag == "em" || node->tag == "i") {
    node->computed_style.font_italic = true;
  } else if (node->tag == "a") {
    if (!(node->computed_style.set_fields & Style::F_COLOR)) {
      node->computed_style.color = 0xFF0000EE; // Link blue
    }
  }

  for (auto &child : node->children) {
    apply_styles(child, rules, scale, node->computed_style);
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

static void collect_lines_recursive(std::shared_ptr<Node> node,
                                    std::vector<Node::LineInfo *> &lines) {

  if (node->is_text) {
    for (auto &line : node->text_lines) {
      lines.push_back(&line);
    }
  } else if (!node->is_block) {
    for (auto &child : node->children) {
      collect_lines_recursive(child, lines);
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

void layout_node(std::shared_ptr<Node> node, int container_w, int line_start_x,
                 int &x, int &y, double scale_factor,
                 Style parent_style = Style()) {

  if (!node)
    return;

  Style current_style = parent_style;
  if (!node->is_text) {
    // Basic inheritance
    if (node->computed_style.color != 0xFF000000)
      current_style.color = node->computed_style.color;
    if (node->computed_style.font_size != 16)
      current_style.font_size = node->computed_style.font_size;
    if (node->computed_style.font_bold)
      current_style.font_bold = true;
    if (node->computed_style.font_italic)
      current_style.font_italic = true;
    if (node->computed_style.text_align != "left")
      current_style.text_align = node->computed_style.text_align;
    if (node->computed_style.line_height != 1.4)
      current_style.line_height = node->computed_style.line_height;

    // Save computed
    Style s = node->computed_style;
    if (!(node->computed_style.set_fields & Style::F_COLOR))
      s.color = current_style.color;
    if (!(node->computed_style.set_fields & Style::F_FONT_SIZE))
      s.font_size = current_style.font_size;
    s.font_bold = current_style.font_bold;
    s.font_italic = current_style.font_italic;
    s.text_align = current_style.text_align;
    s.line_height = current_style.line_height;
    node->computed_style = s;
  } else {
    node->computed_style = parent_style;
  }

  if (node->is_text) {
    double font_scale =
        (double)node->computed_style.font_size / (16.0 * scale_factor);
    double total_scale = scale_factor * font_scale;
    int char_w = (int)(8.0 * total_scale);
    if (node->computed_style.font_bold) {
      char_w += (int)std::max(1.0, total_scale / 2.0);
    }
    int char_h = (int)(node->computed_style.line_height * 16.0 * total_scale);

    node->text_lines.clear();
    node->margin_box.x = x;
    node->margin_box.y = y;
    int start_y = y;

    std::string text = node->text;
    size_t line_start = 0;
    int current_line_x = x;
    int first_line_start_x = x;

    size_t i = 0;
    while (i < text.length()) {
      // Skip leading spaces ONLY at the actual start of a wrap line
      if (current_line_x == line_start_x && line_start == i) {
        while (i < text.length() && std::isspace(text[i])) {
          i++;
          line_start++;
        }
        if (i >= text.length())
          break;
      }

      size_t word_end = i;
      while (word_end < text.length() && !std::isspace(text[word_end]))
        word_end++;

      // Includes spaces after word for width calculation, but we won't wrap on
      // them
      size_t segment_end = word_end;
      while (segment_end < text.length() && std::isspace(text[segment_end]))
        segment_end++;

      int seg_w = (int)((segment_end - i) * char_w);

      if (current_line_x + seg_w > container_w &&
          current_line_x > line_start_x) {
        // Wrap: finish previous line
        node->text_lines.push_back(
            {line_start == 0 ? first_line_start_x : line_start_x, y,
             current_line_x -
                 (line_start == 0 ? first_line_start_x : line_start_x),
             line_start, i - line_start});

        y += char_h;
        current_line_x = line_start_x;
        line_start = i;
      }

      current_line_x += seg_w;
      i = segment_end;
    }
    // Last line
    if (line_start < text.length()) {
      node->text_lines.push_back(
          {line_start == 0 ? first_line_start_x : line_start_x, y,
           current_line_x -
               (line_start == 0 ? first_line_start_x : line_start_x),
           line_start, text.length() - line_start});
      x = current_line_x;
    }

    node->margin_box.w =
        (y == start_y) ? (x - node->margin_box.x) : container_w;
    node->margin_box.h = (y - start_y) + char_h;
    node->border_box = node->padding_box = node->content_box = node->margin_box;
  } else {
    Style &s = node->computed_style;

    if (node->is_block && x != line_start_x) {
      x = line_start_x;
      double font_scale = (double)current_style.font_size / 16.0;
      y += (int)(scale_factor * font_scale * 16.0 *
                 node->computed_style.line_height);
    }

    int box_start_y = y;
    node->margin_box.x = x;
    node->margin_box.y = y;

    node->border_box.x = x + s.margin_left;
    node->border_box.y = y + s.margin_top;
    node->padding_box.x = node->border_box.x + s.border_left;
    node->padding_box.y = node->border_box.y + s.border_top;
    node->content_box.x = node->padding_box.x + s.padding_left;
    node->content_box.y = node->padding_box.y + s.padding_top;

    int available_w = container_w - x;
    int cur_inner_w;
    if (s.width != -1) {
      node->content_box.w = s.width;
      cur_inner_w = s.width - s.padding_left - s.padding_right - s.border_left -
                    s.border_right;
    } else {
      cur_inner_w = available_w - s.margin_left - s.margin_right -
                    s.border_left - s.border_right - s.padding_left -
                    s.padding_right;
      if (node->is_block)
        node->content_box.w = cur_inner_w;
      else
        node->content_box.w = 0;
    }

    int child_x = node->content_box.x;
    int child_y = node->content_box.y;
    int content_max_x = node->content_box.x;

    int child_container_w = node->content_box.x + cur_inner_w;
    int child_line_start_x =
        node->is_block ? node->content_box.x : line_start_x;

    for (auto &child : node->children) {
      int prev_x = child_x;
      int prev_y = child_y;
      layout_node(child, child_container_w, child_line_start_x, child_x,
                  child_y, scale_factor, node->computed_style);

      if (child_x > content_max_x)
        content_max_x = child_x;
    }

    if (s.width == -1 && !node->is_block)
      node->content_box.w = content_max_x - node->content_box.x;

    if (s.height != -1) {
      node->content_box.h = s.height;
    } else {
      double font_scale = (double)node->computed_style.font_size / 16.0;
      int line_h = (int)(scale_factor * font_scale * 16.0 *
                         node->computed_style.line_height);
      node->content_box.h =
          (child_y == node->content_box.y && node->children.empty())
              ? 0
              : (child_y - node->content_box.y + line_h);
    }

    // Align lines if it's a block
    if (node->is_block && s.text_align == "center") {
      std::vector<Node::LineInfo *> flow_lines;
      for (auto &child : node->children) {
        collect_lines_recursive(child, flow_lines);
      }

      if (!flow_lines.empty()) {
        // Group lines by Y coordinate
        std::map<int, std::vector<Node::LineInfo *>> lines_by_y;
        for (auto *line : flow_lines) {
          lines_by_y[line->y].push_back(line);
        }

        for (auto &pair : lines_by_y) {
          auto &y_lines = pair.second;
          int min_lx = 1000000;
          int max_rx = -1000000;
          for (auto *line : y_lines) {
            if (line->x < min_lx)
              min_lx = line->x;
            if (line->x + line->width > max_rx)
              max_rx = line->x + line->width;
          }

          int total_w = max_rx - min_lx;
          int available_w = node->content_box.w;
          if (total_w < available_w) {
            // How much to shift this group of lines?
            // Target center of content_box is node->content_box.x +
            // available_w/2 Current center is (min_lx + max_rx)/2
            int shift =
                (node->content_box.x + available_w / 2) - (min_lx + max_rx) / 2;
            for (auto *line : y_lines) {
              line->x += shift;
            }
          }
        }
      }
    }

    node->padding_box.w =
        node->content_box.w + s.padding_left + s.padding_right;
    node->padding_box.h =
        node->content_box.h + s.padding_top + s.padding_bottom;
    node->border_box.w = node->padding_box.w + s.border_left + s.border_right;
    node->border_box.h = node->padding_box.h + s.border_top + s.border_bottom;
    node->margin_box.w = node->border_box.w + s.margin_left + s.margin_right;
    node->margin_box.h = node->border_box.h + s.margin_top + s.margin_bottom;
    if (node->is_block) {
      x = line_start_x;
      y = node->margin_box.y + node->margin_box.h;
    } else {
      x = child_x;
      y = child_y;
    }
  }
}

void paint_node(std::shared_ptr<Node> node, const LayoutState &state,
                uint32_t current_bg_color) {
  if (!node)
    return;

  if (!node->is_text) {
    Style &s = node->computed_style;

    if (s.has_bg) {
      draw_rect(state.buffer, state.width, state.height, node->padding_box.x,
                node->padding_box.y, node->padding_box.w, node->padding_box.h,
                s.bg_color);
      current_bg_color = s.bg_color;
    }

    if (s.border_top > 0)
      draw_rect(state.buffer, state.width, state.height, node->border_box.x,
                node->border_box.y, node->border_box.w, s.border_top,
                s.border_color);
    if (s.border_bottom > 0)
      draw_rect(state.buffer, state.width, state.height, node->border_box.x,
                node->border_box.y + node->border_box.h - s.border_bottom,
                node->border_box.w, s.border_bottom, s.border_color);
    if (s.border_left > 0)
      draw_rect(state.buffer, state.width, state.height, node->border_box.x,
                node->border_box.y, s.border_left, node->border_box.h,
                s.border_color);
    if (s.border_right > 0)
      draw_rect(state.buffer, state.width, state.height,
                node->border_box.x + node->border_box.w - s.border_right,
                node->border_box.y, s.border_right, node->border_box.h,
                s.border_color);
  } else {
    double font_scale =
        (double)node->computed_style.font_size / (16.0 * state.scale_factor);
    double total_scale = state.scale_factor * font_scale;
    int char_w = (int)(8.0 * total_scale);
    if (node->computed_style.font_bold) {
      char_w += (int)std::max(1.0, total_scale / 2.0);
    }

    for (const auto &line : node->text_lines) {
      int cur_x = line.x;
      std::string line_text = node->text.substr(line.start, line.length);

      uint32_t text_color = node->computed_style.color;
      // Auto-invert if same as background
      if ((text_color & 0x00FFFFFF) == (current_bg_color & 0x00FFFFFF)) {
        text_color ^= 0x00FFFFFF;
      }

      for (char c : line_text) {
        draw_char(state.buffer, state.width, state.height, cur_x, line.y, c,
                  text_color, state.scale_factor, font_scale,
                  node->computed_style.font_bold,
                  node->computed_style.font_italic);
        cur_x += char_w;
      }
    }
  }

  for (auto &child : node->children) {
    paint_node(child, state, current_bg_color);
  }
}

static std::shared_ptr<Node> global_root = nullptr;

static std::shared_ptr<Node> find_node_at(std::shared_ptr<Node> node, int x,
                                          int y) {
  if (!node)
    return nullptr;

  // Check children first (top-most elements first)
  for (auto it = node->children.rbegin(); it != node->children.rend(); ++it) {
    auto hit = find_node_at(*it, x, y);
    if (hit)
      return hit;
  }

  // Check self
  if (x >= node->border_box.x && x < node->border_box.x + node->border_box.w &&
      y >= node->border_box.y && y < node->border_box.y + node->border_box.h) {
    return node;
  }

  return nullptr;
}

extern "C" {
bool hit_test(int x, int y, char *out_href, int max_len) {
  if (!global_root)
    return false;

  auto node = find_node_at(global_root, x, y);
  // Bubble up to find <a> tag if we hit a text node or nested span
  while (node) {
    if (node->tag == "a" && !node->href.empty()) {
      strncpy(out_href, node->href.c_str(), max_len - 1);
      out_href[max_len - 1] = '\0';
      return true;
    }
    // Traverse up to parent
    auto parent = node->parent.lock();
    node = parent;
  }
  return false;
}
void render_frame(const char *html_cstr, uint32_t *buffer, int width,
                  int height, double scale_factor) {
  // Clear background
  for (int i = 0; i < width * height; ++i) {
    buffer[i] = 0xFFFFFFFF; // White
  }

  std::string html(html_cstr);
  std::vector<StyleRule> styles;
  global_root = parse_html(html, styles);
  apply_styles(global_root, styles, scale_factor, Style());

  // Layout phase
  int x = 0;
  int y = 0;
  layout_node(global_root, width, 0, x, y, scale_factor);

  // Paint phase
  LayoutState state;
  state.x = 0;
  state.y = 0;
  state.width = width;
  state.height = height;
  state.buffer = buffer;
  state.scale_factor = scale_factor;

  paint_node(global_root, state, 0xFFFFFFFF); // Start with white background
}
}
