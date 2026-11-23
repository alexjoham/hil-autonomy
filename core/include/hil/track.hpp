#pragma once
#include "hil/geometry.hpp"
#include <span>

namespace hil
{
    /**
     * Fills a circle of out.size() points counterclockwise, calculated from a center and radius
     * @param out span of the output points, will be set by the method
     * @param center center of the created circle
     * @param radius radius of the circle
     */
    void fill_circle(std::span<Point> out, Point center, double radius);

    /**
     * Returns the nearest index inside the track to the given position
     * @param track all points in the track
     * @param position position from which the nearest point should be found from
     * @return position of nearest point inside track
     */
    std::size_t nearest_index(std::span<const Point> track, Point position);
} // namespace hil
