
/*
    SPDX-License-Identifier: CC0-1.0

    You should have received a copy of the CC0 legalcode along with this work.
    If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/
#include <QtGlobal>

#include <algorithm>
#include <array>

#include <math.h>

namespace hcy {

#if QT_VERSION < 0x060000
    typedef qreal color_comp_t; // double
    static constexpr qreal color_comp_min { 0.0 };
    static constexpr qreal color_comp_max { 1.0 };
#else
    typedef float color_comp_t;
    static constexpr float color_comp_min { 0.0f };
    static constexpr float color_comp_max { 1.0f };
#endif

#if 0
// https://www.itu.int/dms_pubrec/itu-r/rec/bt/R-REC-BT.601-7-201103-I!!PDF-E.pdf
// https://www.w3.org/TR/AERT/#color-contrast
static constexpr std::array<color_comp_t, 3> ry = { 0.299, 0.587, 0.114 };
#else
// https://www.itu.int/dms_pubrec/itu-r/rec/bt/R-REC-BT.709-6-201506-I!!PDF-E.pdf
// https://www.w3.org/WAI/GL/wiki/Relative_luminance#Definition_as_Stated_in_WCAG_2.x
// https://en.wikipedia.org/wiki/Luma_(video)
// https://en.wikipedia.org/wiki/Relative_luminance
static constexpr std::array<color_comp_t, 3> ry = { 0.2126, 0.7152, 0.0722 };
#endif

// https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color
static constexpr color_comp_t relative_luminance(color_comp_t r, color_comp_t g, color_comp_t b)
{
    return ry[0] * r + ry[1] * g + ry[2] * b;
}

// https://www.w3.org/TR/WCAG/#dfn-contrast-ratio
static constexpr color_comp_t contrast_ratio(color_comp_t a, color_comp_t b)
{
    return (a > b) ? (a + 0.05) / (b + 0.05) : (b + 0.05) / (a + 0.05);
}

static constexpr color_comp_t gamma_correction(color_comp_t color_component)
{
    color_comp_t color_comp = std::clamp(color_component, color_comp_min, color_comp_max);
    return (color_comp <= 0.04045) ? (color_comp / 12.92) : std::pow((color_comp + 0.055) / 1.055, 2.4);
}

static const color_comp_t luminance(color_comp_t r, color_comp_t g, color_comp_t b)
{
    return relative_luminance(gamma_correction(r), gamma_correction(g), gamma_correction(b));
}
} // namespace hcy
