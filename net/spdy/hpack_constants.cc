// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/spdy/hpack_constants.h"

#include <bitset>

namespace net {

namespace {

uint32 bits32(const std::string& bitstring) {
  return std::bitset<32>(bitstring).to_ulong();
}

}  // namespace

// Produced by applying the python program [1] to tables provided by [2].
// [1]
// import re, sys
// count = 0
// for l in sys.stdin:
//   m = re.match(r"^ +('.+'|EOS)? \( *(\d+)\) \|([10\|]+) \[(\d+)\]", l)
//   if m:
//     g = m.groups()
//     print('  {bits32("%s"), %s, %s},%s' % (
//       g[2].replace('|','').ljust(32,'0'), g[3], g[1],
//       ('  // %s' % g[0]) if g[0] else ''))
//     count += 1
// print("Total: %s" % count)
//
// [2] http://tools.ietf.org/html/draft-ietf-httpbis-header-compression-06

// HpackHuffmanSymbol entries are initialized as {code, length, id}.
// Codes are specified in the |length| most-significant bits of |code|.
std::vector<HpackHuffmanSymbol> HpackHuffmanCode() {
  const HpackHuffmanSymbol kHpackHuffmanCode[] = {
    {bits32("11111111111111111111011101000000"), 27, 0},
    {bits32("11111111111111111111011101100000"), 27, 1},
    {bits32("11111111111111111111011110000000"), 27, 2},
    {bits32("11111111111111111111011110100000"), 27, 3},
    {bits32("11111111111111111111011111000000"), 27, 4},
    {bits32("11111111111111111111011111100000"), 27, 5},
    {bits32("11111111111111111111100000000000"), 27, 6},
    {bits32("11111111111111111111100000100000"), 27, 7},
    {bits32("11111111111111111111100001000000"), 27, 8},
    {bits32("11111111111111111111100001100000"), 27, 9},
    {bits32("11111111111111111111100010000000"), 27, 10},
    {bits32("11111111111111111111100010100000"), 27, 11},
    {bits32("11111111111111111111100011000000"), 27, 12},
    {bits32("11111111111111111111100011100000"), 27, 13},
    {bits32("11111111111111111111100100000000"), 27, 14},
    {bits32("11111111111111111111100100100000"), 27, 15},
    {bits32("11111111111111111111100101000000"), 27, 16},
    {bits32("11111111111111111111100101100000"), 27, 17},
    {bits32("11111111111111111111100110000000"), 27, 18},
    {bits32("11111111111111111111100110100000"), 27, 19},
    {bits32("11111111111111111111100111000000"), 27, 20},
    {bits32("11111111111111111111100111100000"), 27, 21},
    {bits32("11111111111111111111101000000000"), 27, 22},
    {bits32("11111111111111111111101000100000"), 27, 23},
    {bits32("11111111111111111111101001000000"), 27, 24},
    {bits32("11111111111111111111101001100000"), 27, 25},
    {bits32("11111111111111111111101010000000"), 27, 26},
    {bits32("11111111111111111111101010100000"), 27, 27},
    {bits32("11111111111111111111101011000000"), 27, 28},
    {bits32("11111111111111111111101011100000"), 27, 29},
    {bits32("11111111111111111111101100000000"), 27, 30},
    {bits32("11111111111111111111101100100000"), 27, 31},
    {bits32("11101000000000000000000000000000"), 8, 32},  // ' '
    {bits32("11111111110000000000000000000000"), 12, 33},  // '!'
    {bits32("11111111111010000000000000000000"), 14, 34},  // '"'
    {bits32("11111111111110000000000000000000"), 15, 35},  // '#'
    {bits32("11111111111110100000000000000000"), 15, 36},  // '$'
    {bits32("10010000000000000000000000000000"), 6, 37},  // '%'
    {bits32("11011100000000000000000000000000"), 7, 38},  // '&'
    {bits32("11111111111111000000000000000000"), 15, 39},  // '''
    {bits32("11111111010000000000000000000000"), 11, 40},  // '('
    {bits32("11111111011000000000000000000000"), 11, 41},  // ')'
    {bits32("11111110100000000000000000000000"), 10, 42},  // '*'
    {bits32("11111111100000000000000000000000"), 11, 43},  // '+'
    {bits32("11101001000000000000000000000000"), 8, 44},  // ','
    {bits32("10010100000000000000000000000000"), 6, 45},  // '-'
    {bits32("00100000000000000000000000000000"), 5, 46},  // '.'
    {bits32("00000000000000000000000000000000"), 4, 47},  // '/'
    {bits32("00101000000000000000000000000000"), 5, 48},  // '0'
    {bits32("00110000000000000000000000000000"), 5, 49},  // '1'
    {bits32("00111000000000000000000000000000"), 5, 50},  // '2'
    {bits32("10011000000000000000000000000000"), 6, 51},  // '3'
    {bits32("10011100000000000000000000000000"), 6, 52},  // '4'
    {bits32("10100000000000000000000000000000"), 6, 53},  // '5'
    {bits32("10100100000000000000000000000000"), 6, 54},  // '6'
    {bits32("10101000000000000000000000000000"), 6, 55},  // '7'
    {bits32("10101100000000000000000000000000"), 6, 56},  // '8'
    {bits32("10110000000000000000000000000000"), 6, 57},  // '9'
    {bits32("11110110000000000000000000000000"), 9, 58},  // ':'
    {bits32("11101010000000000000000000000000"), 8, 59},  // ';'
    {bits32("11111111111111111000000000000000"), 18, 60},  // '<'
    {bits32("10110100000000000000000000000000"), 6, 61},  // '='
    {bits32("11111111111111100000000000000000"), 17, 62},  // '>'
    {bits32("11110110100000000000000000000000"), 9, 63},  // '?'
    {bits32("11111111111011000000000000000000"), 14, 64},  // '@'
    {bits32("11011110000000000000000000000000"), 7, 65},  // 'A'
    {bits32("11101011000000000000000000000000"), 8, 66},  // 'B'
    {bits32("11101100000000000000000000000000"), 8, 67},  // 'C'
    {bits32("11101101000000000000000000000000"), 8, 68},  // 'D'
    {bits32("11101110000000000000000000000000"), 8, 69},  // 'E'
    {bits32("11100000000000000000000000000000"), 7, 70},  // 'F'
    {bits32("11110111000000000000000000000000"), 9, 71},  // 'G'
    {bits32("11110111100000000000000000000000"), 9, 72},  // 'H'
    {bits32("11111000000000000000000000000000"), 9, 73},  // 'I'
    {bits32("11111000100000000000000000000000"), 9, 74},  // 'J'
    {bits32("11111110110000000000000000000000"), 10, 75},  // 'K'
    {bits32("11111001000000000000000000000000"), 9, 76},  // 'L'
    {bits32("11101111000000000000000000000000"), 8, 77},  // 'M'
    {bits32("11111001100000000000000000000000"), 9, 78},  // 'N'
    {bits32("11111010000000000000000000000000"), 9, 79},  // 'O'
    {bits32("11111010100000000000000000000000"), 9, 80},  // 'P'
    {bits32("11111011000000000000000000000000"), 9, 81},  // 'Q'
    {bits32("11111011100000000000000000000000"), 9, 82},  // 'R'
    {bits32("11110000000000000000000000000000"), 8, 83},  // 'S'
    {bits32("11110001000000000000000000000000"), 8, 84},  // 'T'
    {bits32("11111100000000000000000000000000"), 9, 85},  // 'U'
    {bits32("11111100100000000000000000000000"), 9, 86},  // 'V'
    {bits32("11111101000000000000000000000000"), 9, 87},  // 'W'
    {bits32("11111101100000000000000000000000"), 9, 88},  // 'X'
    {bits32("11111110000000000000000000000000"), 9, 89},  // 'Y'
    {bits32("11111111000000000000000000000000"), 10, 90},  // 'Z'
    {bits32("11111111111100000000000000000000"), 14, 91},  // '['
    {bits32("11111111111111111111101101000000"), 27, 92},  // '\'
    {bits32("11111111111000000000000000000000"), 13, 93},  // ']'
    {bits32("11111111111101000000000000000000"), 14, 94},  // '^'
    {bits32("10111000000000000000000000000000"), 6, 95},  // '_'
    {bits32("11111111111111111100000000000000"), 19, 96},  // '`'
    {bits32("01000000000000000000000000000000"), 5, 97},  // 'a'
    {bits32("10111100000000000000000000000000"), 6, 98},  // 'b'
    {bits32("01001000000000000000000000000000"), 5, 99},  // 'c'
    {bits32("11000000000000000000000000000000"), 6, 100},  // 'd'
    {bits32("00010000000000000000000000000000"), 4, 101},  // 'e'
    {bits32("11000100000000000000000000000000"), 6, 102},  // 'f'
    {bits32("11001000000000000000000000000000"), 6, 103},  // 'g'
    {bits32("11001100000000000000000000000000"), 6, 104},  // 'h'
    {bits32("01010000000000000000000000000000"), 5, 105},  // 'i'
    {bits32("11100010000000000000000000000000"), 7, 106},  // 'j'
    {bits32("11100100000000000000000000000000"), 7, 107},  // 'k'
    {bits32("01011000000000000000000000000000"), 5, 108},  // 'l'
    {bits32("11010000000000000000000000000000"), 6, 109},  // 'm'
    {bits32("01100000000000000000000000000000"), 5, 110},  // 'n'
    {bits32("01101000000000000000000000000000"), 5, 111},  // 'o'
    {bits32("01110000000000000000000000000000"), 5, 112},  // 'p'
    {bits32("11110010000000000000000000000000"), 8, 113},  // 'q'
    {bits32("01111000000000000000000000000000"), 5, 114},  // 'r'
    {bits32("10000000000000000000000000000000"), 5, 115},  // 's'
    {bits32("10001000000000000000000000000000"), 5, 116},  // 't'
    {bits32("11010100000000000000000000000000"), 6, 117},  // 'u'
    {bits32("11100110000000000000000000000000"), 7, 118},  // 'v'
    {bits32("11011000000000000000000000000000"), 6, 119},  // 'w'
    {bits32("11110011000000000000000000000000"), 8, 120},  // 'x'
    {bits32("11110100000000000000000000000000"), 8, 121},  // 'y'
    {bits32("11110101000000000000000000000000"), 8, 122},  // 'z'
    {bits32("11111111111111101000000000000000"), 17, 123},  // '{'
    {bits32("11111111101000000000000000000000"), 11, 124},  // '|'
    {bits32("11111111111111110000000000000000"), 17, 125},  // '}'
    {bits32("11111111110100000000000000000000"), 12, 126},  // '~'
    {bits32("11111111111111111111101101100000"), 27, 127},
    {bits32("11111111111111111111101110000000"), 27, 128},
    {bits32("11111111111111111111101110100000"), 27, 129},
    {bits32("11111111111111111111101111000000"), 27, 130},
    {bits32("11111111111111111111101111100000"), 27, 131},
    {bits32("11111111111111111111110000000000"), 27, 132},
    {bits32("11111111111111111111110000100000"), 27, 133},
    {bits32("11111111111111111111110001000000"), 27, 134},
    {bits32("11111111111111111111110001100000"), 27, 135},
    {bits32("11111111111111111111110010000000"), 27, 136},
    {bits32("11111111111111111111110010100000"), 27, 137},
    {bits32("11111111111111111111110011000000"), 27, 138},
    {bits32("11111111111111111111110011100000"), 27, 139},
    {bits32("11111111111111111111110100000000"), 27, 140},
    {bits32("11111111111111111111110100100000"), 27, 141},
    {bits32("11111111111111111111110101000000"), 27, 142},
    {bits32("11111111111111111111110101100000"), 27, 143},
    {bits32("11111111111111111111110110000000"), 27, 144},
    {bits32("11111111111111111111110110100000"), 27, 145},
    {bits32("11111111111111111111110111000000"), 27, 146},
    {bits32("11111111111111111111110111100000"), 27, 147},
    {bits32("11111111111111111111111000000000"), 27, 148},
    {bits32("11111111111111111111111000100000"), 27, 149},
    {bits32("11111111111111111111111001000000"), 27, 150},
    {bits32("11111111111111111111111001100000"), 27, 151},
    {bits32("11111111111111111111111010000000"), 27, 152},
    {bits32("11111111111111111111111010100000"), 27, 153},
    {bits32("11111111111111111111111011000000"), 27, 154},
    {bits32("11111111111111111111111011100000"), 27, 155},
    {bits32("11111111111111111111111100000000"), 27, 156},
    {bits32("11111111111111111111111100100000"), 27, 157},
    {bits32("11111111111111111111111101000000"), 27, 158},
    {bits32("11111111111111111111111101100000"), 27, 159},
    {bits32("11111111111111111111111110000000"), 27, 160},
    {bits32("11111111111111111111111110100000"), 27, 161},
    {bits32("11111111111111111111111111000000"), 27, 162},
    {bits32("11111111111111111111111111100000"), 27, 163},
    {bits32("11111111111111111110000000000000"), 26, 164},
    {bits32("11111111111111111110000001000000"), 26, 165},
    {bits32("11111111111111111110000010000000"), 26, 166},
    {bits32("11111111111111111110000011000000"), 26, 167},
    {bits32("11111111111111111110000100000000"), 26, 168},
    {bits32("11111111111111111110000101000000"), 26, 169},
    {bits32("11111111111111111110000110000000"), 26, 170},
    {bits32("11111111111111111110000111000000"), 26, 171},
    {bits32("11111111111111111110001000000000"), 26, 172},
    {bits32("11111111111111111110001001000000"), 26, 173},
    {bits32("11111111111111111110001010000000"), 26, 174},
    {bits32("11111111111111111110001011000000"), 26, 175},
    {bits32("11111111111111111110001100000000"), 26, 176},
    {bits32("11111111111111111110001101000000"), 26, 177},
    {bits32("11111111111111111110001110000000"), 26, 178},
    {bits32("11111111111111111110001111000000"), 26, 179},
    {bits32("11111111111111111110010000000000"), 26, 180},
    {bits32("11111111111111111110010001000000"), 26, 181},
    {bits32("11111111111111111110010010000000"), 26, 182},
    {bits32("11111111111111111110010011000000"), 26, 183},
    {bits32("11111111111111111110010100000000"), 26, 184},
    {bits32("11111111111111111110010101000000"), 26, 185},
    {bits32("11111111111111111110010110000000"), 26, 186},
    {bits32("11111111111111111110010111000000"), 26, 187},
    {bits32("11111111111111111110011000000000"), 26, 188},
    {bits32("11111111111111111110011001000000"), 26, 189},
    {bits32("11111111111111111110011010000000"), 26, 190},
    {bits32("11111111111111111110011011000000"), 26, 191},
    {bits32("11111111111111111110011100000000"), 26, 192},
    {bits32("11111111111111111110011101000000"), 26, 193},
    {bits32("11111111111111111110011110000000"), 26, 194},
    {bits32("11111111111111111110011111000000"), 26, 195},
    {bits32("11111111111111111110100000000000"), 26, 196},
    {bits32("11111111111111111110100001000000"), 26, 197},
    {bits32("11111111111111111110100010000000"), 26, 198},
    {bits32("11111111111111111110100011000000"), 26, 199},
    {bits32("11111111111111111110100100000000"), 26, 200},
    {bits32("11111111111111111110100101000000"), 26, 201},
    {bits32("11111111111111111110100110000000"), 26, 202},
    {bits32("11111111111111111110100111000000"), 26, 203},
    {bits32("11111111111111111110101000000000"), 26, 204},
    {bits32("11111111111111111110101001000000"), 26, 205},
    {bits32("11111111111111111110101010000000"), 26, 206},
    {bits32("11111111111111111110101011000000"), 26, 207},
    {bits32("11111111111111111110101100000000"), 26, 208},
    {bits32("11111111111111111110101101000000"), 26, 209},
    {bits32("11111111111111111110101110000000"), 26, 210},
    {bits32("11111111111111111110101111000000"), 26, 211},
    {bits32("11111111111111111110110000000000"), 26, 212},
    {bits32("11111111111111111110110001000000"), 26, 213},
    {bits32("11111111111111111110110010000000"), 26, 214},
    {bits32("11111111111111111110110011000000"), 26, 215},
    {bits32("11111111111111111110110100000000"), 26, 216},
    {bits32("11111111111111111110110101000000"), 26, 217},
    {bits32("11111111111111111110110110000000"), 26, 218},
    {bits32("11111111111111111110110111000000"), 26, 219},
    {bits32("11111111111111111110111000000000"), 26, 220},
    {bits32("11111111111111111110111001000000"), 26, 221},
    {bits32("11111111111111111110111010000000"), 26, 222},
    {bits32("11111111111111111110111011000000"), 26, 223},
    {bits32("11111111111111111110111100000000"), 26, 224},
    {bits32("11111111111111111110111101000000"), 26, 225},
    {bits32("11111111111111111110111110000000"), 26, 226},
    {bits32("11111111111111111110111111000000"), 26, 227},
    {bits32("11111111111111111111000000000000"), 26, 228},
    {bits32("11111111111111111111000001000000"), 26, 229},
    {bits32("11111111111111111111000010000000"), 26, 230},
    {bits32("11111111111111111111000011000000"), 26, 231},
    {bits32("11111111111111111111000100000000"), 26, 232},
    {bits32("11111111111111111111000101000000"), 26, 233},
    {bits32("11111111111111111111000110000000"), 26, 234},
    {bits32("11111111111111111111000111000000"), 26, 235},
    {bits32("11111111111111111111001000000000"), 26, 236},
    {bits32("11111111111111111111001001000000"), 26, 237},
    {bits32("11111111111111111111001010000000"), 26, 238},
    {bits32("11111111111111111111001011000000"), 26, 239},
    {bits32("11111111111111111111001100000000"), 26, 240},
    {bits32("11111111111111111111001101000000"), 26, 241},
    {bits32("11111111111111111111001110000000"), 26, 242},
    {bits32("11111111111111111111001111000000"), 26, 243},
    {bits32("11111111111111111111010000000000"), 26, 244},
    {bits32("11111111111111111111010001000000"), 26, 245},
    {bits32("11111111111111111111010010000000"), 26, 246},
    {bits32("11111111111111111111010011000000"), 26, 247},
    {bits32("11111111111111111111010100000000"), 26, 248},
    {bits32("11111111111111111111010101000000"), 26, 249},
    {bits32("11111111111111111111010110000000"), 26, 250},
    {bits32("11111111111111111111010111000000"), 26, 251},
    {bits32("11111111111111111111011000000000"), 26, 252},
    {bits32("11111111111111111111011001000000"), 26, 253},
    {bits32("11111111111111111111011010000000"), 26, 254},
    {bits32("11111111111111111111011011000000"), 26, 255},
    {bits32("11111111111111111111011100000000"), 26, 256},  // EOS
  };
  return std::vector<HpackHuffmanSymbol>(
      kHpackHuffmanCode,
      kHpackHuffmanCode + arraysize(kHpackHuffmanCode));
}

}  // namespace net
