/*
 * Copyright (C) 2016 Simon Fels <morphis@gravedo.de>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3, as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranties of
 * MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "kydroidrect.h"
//#include "../kydroidutils.h"

#include <algorithm>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/throw_exception.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <fcntl.h>
#include <mntent.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace {

std::vector<std::string> string_split(const std::string &text, char sep) {
  std::vector<std::string> tokens;
  return boost::algorithm::split(tokens, text, boost::is_from_range(sep, sep), boost::algorithm::token_compress_on);
}

}

const KydroidRect KydroidRect::Invalid{-1, -1, -1, -1};
const KydroidRect KydroidRect::Empty{0, 0, 0, 0};

void KydroidRect::merge(const KydroidRect &rhs) {
  left_ = std::min(left_, rhs.left());
  top_ = std::min(top_, rhs.top());
  right_ = std::max(right_, rhs.right());
  bottom_ = std::max(bottom_, rhs.bottom());
}

void KydroidRect::translate(const std::int32_t &x, const std::int32_t &y) {
  auto old_width = width();
  auto old_height = height();
  left_ = x;
  top_ = y;
  right_ = x + old_width;
  bottom_ = y + old_height;
}

void KydroidRect::resize(const std::int32_t &width, const std::int32_t &height) {
  right_ = left_ + width;
  bottom_ = top_ + height;
}

std::ostream &operator<<(std::ostream &out, const KydroidRect &rect) {
  return out << "{" << rect.left() << "," << rect.top() << "," << rect.right()
             << "," << rect.bottom() << "} {" << rect.width() << ","
             << rect.height() << "}";
}

std::istream& operator>>(std::istream& in, KydroidRect &rect)
try {
  std::string str;
  in >> str;
  auto tokens = string_split(str, ',');

  switch (tokens.size()) {
  case 2: {
    rect = KydroidRect(0, 0,
             boost::lexical_cast<std::int32_t>(tokens[0]),
             boost::lexical_cast<std::int32_t>(tokens[1]));
    break;
  }
  case 4:
    rect = KydroidRect(
             boost::lexical_cast<std::int32_t>(tokens[0]),
             boost::lexical_cast<std::int32_t>(tokens[1]),
             boost::lexical_cast<std::int32_t>(tokens[2]),
             boost::lexical_cast<std::int32_t>(tokens[3]));
    break;
  default:
    break;
  }

  return in;
} catch (...) {
  return in;
}
