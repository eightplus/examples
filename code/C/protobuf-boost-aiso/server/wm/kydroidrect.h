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

#ifndef KYDROID_KYDROID_RECT_H_
#define KYDROID_KYDROID_RECT_H_

#include <cstdint>
#include <ostream>

class KydroidRect {
public:
    static const KydroidRect Invalid;
    static const KydroidRect Empty;

    inline KydroidRect() : KydroidRect(Empty) {}

    inline KydroidRect(const std::int32_t &left, const std::int32_t &top,
              const std::int32_t &right, const std::int32_t &bottom)
      : left_(left), top_(top), right_(right), bottom_(bottom) {}

    inline KydroidRect(const std::int32_t &width, const std::int32_t &height)
      : left_(0), top_(0), right_(width), bottom_(height) {}

    inline void clear() { left_ = top_ = right_ = bottom_ = 0; }

    inline bool valid() const { return width() >= 0 && height() >= 0; }

    inline std::int32_t width() const { return right_ - left_; }

    inline std::int32_t height() const { return bottom_ - top_; }

    inline std::int32_t left() const { return left_; }

    inline std::int32_t top() const { return top_; }

    inline std::int32_t right() const { return right_; }

    inline std::int32_t bottom() const { return bottom_; }

    inline bool operator==(const KydroidRect &rhs) const {
    return (left_ == rhs.left() && top_ == rhs.top() && right_ == rhs.right() &&
            bottom_ == rhs.bottom());
    }

    inline bool operator!=(const KydroidRect &rhs) const { return !operator==(rhs); }

    void merge(const KydroidRect &rhs);

    void translate(const std::int32_t &x, const std::int32_t &y);

    void resize(const std::int32_t &width, const std::int32_t &height);

private:
    std::int32_t left_;
    std::int32_t top_;
    std::int32_t right_;
    std::int32_t bottom_;
};

std::ostream &operator<<(std::ostream &out, const KydroidRect &rect);
std::istream& operator>>(std::istream& in, KydroidRect &rect);

//Q_DECLARE_METATYPE(/*kydroid::graphics::*/KydroidRect)

#endif // KYDROID_KYDROID_RECT_H_
