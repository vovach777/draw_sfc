#pragma once
/*
A 2D algorithm that combines Peano (3x3) and Hilbert (2x2) blocks was published
by Lutz Tautenhahn in 2003:

[1] Lutz Tautenhahn: Draw a Space-Filling Curve of Arbitrary Size,
http://lutanho.net/pic2html/draw_sfc.html, 2003.
*/

#include <cmath>
#include <utility>

namespace hilbert_piano {
template <typename RenderCallback, typename Message>
inline void go(int x0, int y0, int dxl, int dyl, int dxr, int dyr, char dir,
               RenderCallback &&render,
               Message &&msg) { // x0, y0: start corner looking to the center of
                                // the rectangle
  // dxl, dyl: vector from the start corner to the left corner of the rectangle
  // dxr, dyr: vector from the start corner to the right corner of the rectangle
  // dir: direction to go - "l"=left, "m"=middle, "r"=right
  // msg("go: "+x0+", "+y0+", "+dxl+", "+dyl+", "+dxr+", "+dyr+", "+dir);
  // render if 2x3 or smaller

  if (abs((dxl + dyl) * (dxr + dyr)) <= 6) {
    int ddx, ddy, ii;
    if (abs(dxl + dyl) == 1) {
      ddx = dxr / abs(dxr + dyr);
      ddy = dyr / abs(dxr + dyr);
      for (ii = 0; ii < abs(dxr + dyr); ii++)
        render(x0 + ii * ddx + (dxl + ddx - 1) / 2,
               y0 + ii * ddy + (dyl + ddy - 1) / 2, dir);
      return;
    }
    if (abs(dxr + dyr) == 1) {
      ddx = dxl / abs(dxl + dyl);
      ddy = dyl / abs(dxl + dyl);
      for (ii = 0; ii < abs(dxl + dyl); ii++)
        render(x0 + ii * ddx + (dxr + ddx - 1) / 2,
               y0 + ii * ddy + (dyr + ddy - 1) / 2, dir);
      return;
    }
    if (dir == 'l') {
      ddx = dxr / abs(dxr + dyr);
      ddy = dyr / abs(dxr + dyr);
      for (ii = 0; ii < abs(dxr + dyr); ii++)
        render(x0 + ii * ddx + (dxl / 2 + ddx - 1) / 2,
               y0 + ii * ddy + (dyl / 2 + ddy - 1) / 2, dir);
      for (ii = abs(dxr + dyr) - 1; ii >= 0; ii--)
        render(x0 + ii * ddx + (dxl + dxl / 2 + ddx - 1) / 2,
               y0 + ii * ddy + (dyl + dyl / 2 + ddy - 1) / 2, dir);
      return;
    }
    if (dir == 'r') {
      ddx = dxl / abs(dxl + dyl);
      ddy = dyl / abs(dxl + dyl);
      for (ii = 0; ii < abs(dxl + dyl); ii++)
        render(x0 + ii * ddx + (dxr / 2 + ddx - 1) / 2,
               y0 + ii * ddy + (dyr / 2 + ddy - 1) / 2, dir);
      for (ii = abs(dxl + dyl) - 1; ii >= 0; ii--)
        render(x0 + ii * ddx + (dxr + dxr / 2 + ddx - 1) / 2,
               y0 + ii * ddy + (dyr + dyr / 2 + ddy - 1) / 2, dir);
      return;
    }
    if (dir == 'm') {
      if (abs(dxr + dyr) == 3) {
        ddx = dxr / abs(dxr + dyr);
        ddy = dyr / abs(dxr + dyr);
        render(x0 + (dxl / 2 + ddx - 1) / 2, y0 + (dyl / 2 + ddy - 1) / 2, dir);
        render(x0 + (dxl + dxl / 2 + ddx - 1) / 2,
               y0 + (dyl + dyl / 2 + ddy - 1) / 2, dir);
        render(x0 + ddx + (dxl + dxl / 2 + ddx - 1) / 2,
               y0 + ddy + (dyl + dyl / 2 + ddy - 1) / 2, dir);
        render(x0 + ddx + (dxl / 2 + ddx - 1) / 2,
               y0 + ddy + (dyl / 2 + ddy - 1) / 2, dir);
        render(x0 + 2 * ddx + (dxl / 2 + ddx - 1) / 2,
               y0 + 2 * ddy + (dyl / 2 + ddy - 1) / 2, dir);
        render(x0 + 2 * ddx + (dxl + dxl / 2 + ddx - 1) / 2,
               y0 + 2 * ddy + (dyl + dyl / 2 + ddy - 1) / 2, dir);
        return;
      }
      if (abs(dxl + dyl) == 3) {
        ddx = dxl / abs(dxl + dyl);
        ddy = dyl / abs(dxl + dyl);
        render(x0 + (dxr / 2 + ddx - 1) / 2, y0 + (dyr / 2 + ddy - 1) / 2, dir);
        render(x0 + (dxr + dxr / 2 + ddx - 1) / 2,
               y0 + (dyr + dyr / 2 + ddy - 1) / 2, dir);
        render(x0 + ddx + (dxr + dxr / 2 + ddx - 1) / 2,
               y0 + ddy + (dyr + dyr / 2 + ddy - 1) / 2, dir);
        render(x0 + ddx + (dxr / 2 + ddx - 1) / 2,
               y0 + ddy + (dyr / 2 + ddy - 1) / 2, dir);
        render(x0 + 2 * ddx + (dxr / 2 + ddx - 1) / 2,
               y0 + 2 * ddy + (dyr / 2 + ddy - 1) / 2, dir);
        render(x0 + 2 * ddx + (dxr + dxr / 2 + ddx - 1) / 2,
               y0 + 2 * ddy + (dyr + dyr / 2 + ddy - 1) / 2, dir);
        return;
      }
    }
    msg("renderError");
    return;
  }
  // divide into 2 parts if necessary
  if (2 * (abs(dxl) + abs(dyl)) >
      3 * (abs(dxr) + abs(dyr))) // left side much longer than right side
  {
    int dxl2 = dxl / 2;
    int dyl2 = dyl / 2;
    if ((abs(dxr) + abs(dyr)) % 2 == 0) // right side is even
    {
      if ((abs(dxl) + abs(dyl)) % 2 == 0) // make 2 parts from even side
      {
        if (dir == 'l') {
          if ((abs(dxl) + abs(dyl)) % 4 ==
              0) // make 2 parts even-even from even side
          {
            go(x0, y0, dxl2, dyl2, dxr, dyr, 'l',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxl2, y0 + dyl2, dxl - dxl2, dyl - dyl2, dxr, dyr, 'l',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
          } else // make 2 parts odd-odd from even side
          {
            go(x0, y0, dxl2, dyl2, dxr, dyr, 'm',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxl2 + dxr, y0 + dyl2 + dyr, -dxr, -dyr, dxl - dxl2,
               dyl - dyl2, 'm', std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
          }
          return;
        }
      } else // make 2 parts from odd side
      {
        if (dir == 'm') {
          if ((abs(dxl2) + abs(dyl2)) % 2 == 0) {
            go(x0, y0, dxl2, dyl2, dxr, dyr, 'l',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxl2, y0 + dyl2, dxl - dxl2, dyl - dyl2, dxr, dyr, 'm',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
          } else {
            go(x0, y0, dxl2, dyl2, dxr, dyr, 'm',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxl2 + dxr, y0 + dyl2 + dyr, -dxr, -dyr, dxl - dxl2,
               dyl - dyl2, 'r', std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
          }
          return;
        }
      }
    } else // right side is odd
    {
      if (dir == 'l') {
        go(x0, y0, dxl2, dyl2, dxr, dyr, 'l',
           std::forward<RenderCallback>(render),
           std::forward<Message>(msg));
        go(x0 + dxl2, y0 + dyl2, dxl - dxl2, dyl - dyl2, dxr, dyr, 'l',
           std::forward<RenderCallback>(render),
           std::forward<Message>(msg));
        return;
      }
      if (dir == 'm') {
        go(x0, y0, dxl2, dyl2, dxr, dyr, 'l',
           std::forward<RenderCallback>(render),
           std::forward<Message>(msg));
        go(x0 + dxl2, y0 + dyl2, dxl - dxl2, dyl - dyl2, dxr, dyr, 'm',
           std::forward<RenderCallback>(render),
           std::forward<Message>(msg));
        return;
      }
    }
  }
  if (2 * (abs(dxr) + abs(dyr)) >
      3 * (abs(dxl) + abs(dyl))) // right side much longer than left side
  {
    int dxr2 = dxr / 2;
    int dyr2 = dyr / 2;
    if ((abs(dxl) + abs(dyl)) % 2 == 0) // left side is even
    {
      if ((abs(dxr) + abs(dyr)) % 2 == 0) // make 2 parts from even side
      {
        if (dir == 'r') {
          if ((abs(dxr) + abs(dyr)) % 4 ==
              0) // make 2 parts even-even from even side
          {
            go(x0, y0, dxl, dyl, dxr2, dyr2, 'r',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxr2, y0 + dyr2, dxl, dyl, dxr - dxr2, dyr - dyr2, 'r',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
          } else // make 2 parts odd-odd from even side
          {
            go(x0, y0, dxl, dyl, dxr2, dyr2, 'm',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxr2 + dxl, y0 + dyr2 + dyl, dxr - dxr2, dyr - dyr2, -dxl,
               -dyl, 'm', std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
          }
          return;
        }
      } else // make 2 parts from odd side
      {
        if (dir == 'm') {
          if ((abs(dxr2) + abs(dyr2)) % 2 == 0) {
            go(x0, y0, dxl, dyl, dxr2, dyr2, 'r',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxr2, y0 + dyr2, dxl, dyl, dxr - dxr2, dyr - dyr2, 'm',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
          } else {
            go(x0, y0, dxl, dyl, dxr2, dyr2, 'm',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxr2 + dxl, y0 + dyr2 + dyl, dxr - dxr2, dyr - dyr2, -dxl,
               -dyl, 'l', std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
          }
          return;
        }
      }
    } else // left side is odd
    {
      if (dir == 'r') {
        go(x0, y0, dxl, dyl, dxr2, dyr2, 'r',
           std::forward<RenderCallback>(render),
           std::forward<Message>(msg));
        go(x0 + dxr2, y0 + dyr2, dxl, dyl, dxr - dxr2, dyr - dyr2, 'r',
           std::forward<RenderCallback>(render),
           std::forward<Message>(msg));
        return;
      }
      if (dir == 'm') {
        go(x0, y0, dxl, dyl, dxr2, dyr2, 'r',
           std::forward<RenderCallback>(render),
           std::forward<Message>(msg));
        go(x0 + dxr2, y0 + dyr2, dxl, dyl, dxr - dxr2, dyr - dyr2, 'm',
           std::forward<RenderCallback>(render),
           std::forward<Message>(msg));
        return;
      }
    }
  }
  // divide into 2x2 parts
  if ((dir == 'l') || (dir == 'r')) {
    int dxl2 = dxl / 2;
    int dyl2 = dyl / 2;
    int dxr2 = dxr / 2;
    int dyr2 = dyr / 2;
    if ((abs(dxl + dyl) % 2 == 0) && (abs(dxr + dyr) % 2 == 0)) // even-even
    {
      if (abs(dxl2 + dyl2 + dxr2 + dyr2) % 2 == 0) // ee-ee or oo-oo
      {
        if (dir == 'l') {
          go(x0, y0, dxl2, dyl2, dxr2, dyr2, 'r',
             std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
          go(x0 + dxr2, y0 + dyr2, dxl2, dyl2, dxr - dxr2, dyr - dyr2, 'l',
             std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
          go(x0 + dxr2 + dxl2, y0 + dyr2 + dyl2, dxl - dxl2, dyl - dyl2,
             dxr - dxr2, dyr - dyr2, 'l', std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
          go(x0 + dxr2 + dxl, y0 + dyr2 + dyl, dxl2 - dxl, dyl2 - dyl, -dxr2,
             -dyr2, 'r', std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
        } else {
          go(x0, y0, dxl2, dyl2, dxr2, dyr2, 'l',
             std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
          go(x0 + dxl2, y0 + dyl2, dxl - dxl2, dyl - dyl2, dxr2, dyr2, 'r',
             std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
          go(x0 + dxr2 + dxl2, y0 + dyr2 + dyl2, dxl - dxl2, dyl - dyl2,
             dxr - dxr2, dyr - dyr2, 'r', std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
          go(x0 + dxr + dxl2, y0 + dyr + dyl2, -dxl2, -dyl2, dxr2 - dxr,
             dyr2 - dyr, 'l', std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
        }
      } else // ee-oo or oo-ee
      {
        if ((dxr2 + dyr2) % 2 == 0) // ee-oo
        {
          if (dir == 'l') {
            go(x0, y0, dxl2, dyl2, dxr2, dyr2, 'r',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxr2, y0 + dyr2, dxl2, dyl2, dxr - dxr2, dyr - dyr2, 'm',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxr + dxl2, y0 + dyr + dyl2, dxr2 - dxr, dyr2 - dyr,
               dxl - dxl2, dyl - dyl2, 'm',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxr2 + dxl, y0 + dyr2 + dyl, dxl2 - dxl, dyl2 - dyl, -dxr2,
               -dyr2, 'r', std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
          } else // ee-oo for dir="r" not possible, so transforming into
                 // e-1,e+1-oo = oo-oo
          {
            if (dxr2 != 0)
              dxr2++;
            else
              dyr2++;
            go(x0, y0, dxl2, dyl2, dxr2, dyr2, 'l',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxl2, y0 + dyl2, dxl - dxl2, dyl - dyl2, dxr2, dyr2, 'm',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxl + dxr2, y0 + dyl + dyr2, dxr - dxr2, dyr - dyr2,
               dxl2 - dxl, dyl2 - dyl, 'm',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxl2 + dxr, y0 + dyl2 + dyr, -dxl2, -dyl2, dxr2 - dxr,
               dyr2 - dyr, 'l', std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
          }
        } else // oo-ee
        {
          if (dir == 'r') {
            go(x0, y0, dxl2, dyl2, dxr2, dyr2, 'l',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxl2, y0 + dyl2, dxl - dxl2, dyl - dyl2, dxr2, dyr2, 'm',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxl + dxr2, y0 + dyl + dyr2, dxr - dxr2, dyr - dyr2,
               dxl2 - dxl, dyl2 - dyl, 'm',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxl2 + dxr, y0 + dyl2 + dyr, -dxl2, -dyl2, dxr2 - dxr,
               dyr2 - dyr, 'l', std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
          } else // oo-ee for dir="l" not possible, so transforming into
                 // oo-e-1,e+1 = oo-oo
          {
            if (dxl2 != 0)
              dxl2++;
            else
              dyl2++;
            go(x0, y0, dxl2, dyl2, dxr2, dyr2, 'r',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxr2, y0 + dyr2, dxl2, dyl2, dxr - dxr2, dyr - dyr2, 'm',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxr + dxl2, y0 + dyr + dyl2, dxr2 - dxr, dyr2 - dyr,
               dxl - dxl2, dyl - dyl2, 'm',
               std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
            go(x0 + dxr2 + dxl, y0 + dyr2 + dyl, dxl2 - dxl, dyl2 - dyl, -dxr2,
               -dyr2, 'r', std::forward<RenderCallback>(render),
               std::forward<Message>(msg));
          }
        }
      }
    } else // not even-even
    {
      if ((abs(dxl + dyl) % 2 != 0) && (abs(dxr + dyr) % 2 != 0)) // odd-odd
      {
        if (dxl2 % 2 != 0)
          dxl2 = dxl - dxl2; // get it in a shape eo-eo
        if (dyl2 % 2 != 0)
          dyl2 = dyl - dyl2;
        if (dxr2 % 2 != 0)
          dxr2 = dxr - dxr2;
        if (dyr2 % 2 != 0)
          dyr2 = dyr - dyr2;
        if (dir == 'l') {
          go(x0, y0, dxl2, dyl2, dxr2, dyr2, 'r',
             std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
          go(x0 + dxr2, y0 + dyr2, dxl2, dyl2, dxr - dxr2, dyr - dyr2, 'm',
             std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
          go(x0 + dxr + dxl2, y0 + dyr + dyl2, dxr2 - dxr, dyr2 - dyr,
             dxl - dxl2, dyl - dyl2, 'm', std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
          go(x0 + dxr2 + dxl, y0 + dyr2 + dyl, dxl2 - dxl, dyl2 - dyl, -dxr2,
             -dyr2, 'r', std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
        } else {
          go(x0, y0, dxl2, dyl2, dxr2, dyr2, 'l',
             std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
          go(x0 + dxl2, y0 + dyl2, dxl - dxl2, dyl - dyl2, dxr2, dyr2, 'm',
             std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
          go(x0 + dxl + dxr2, y0 + dyl + dyr2, dxr - dxr2, dyr - dyr2,
             dxl2 - dxl, dyl2 - dyl, 'm', std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
          go(x0 + dxl2 + dxr, y0 + dyl2 + dyr, -dxl2, -dyl2, dxr2 - dxr,
             dyr2 - dyr, 'l', std::forward<RenderCallback>(render),
             std::forward<Message>(msg));
        }
      } else // even-odd or odd-even
      {
        if (abs(dxl + dyl) % 2 == 0) // odd-even
        {
          if (dir == 'l') {
            if (dxr2 % 2 != 0)
              dxr2 = dxr - dxr2; // get it in a shape eo-xx
            if (dyr2 % 2 != 0)
              dyr2 = dyr - dyr2;
            if (abs(dxl + dyl) > 2) {
              go(x0, y0, dxl2, dyl2, dxr2, dyr2, 'r',
                 std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
              go(x0 + dxr2, y0 + dyr2, dxl2, dyl2, dxr - dxr2, dyr - dyr2, 'l',
                 std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
              go(x0 + dxr2 + dxl2, y0 + dyr2 + dyl2, dxl - dxl2, dyl - dyl2,
                 dxr - dxr2, dyr - dyr2, 'l',
                 std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
              go(x0 + dxr2 + dxl, y0 + dyr2 + dyl, dxl2 - dxl, dyl2 - dyl,
                 -dxr2, -dyr2, 'r', std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
            } else {
              go(x0, y0, dxl2, dyl2, dxr2, dyr2, 'r',
                 std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
              go(x0 + dxr2, y0 + dyr2, dxl2, dyl2, dxr - dxr2, dyr - dyr2, 'm',
                 std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
              go(x0 + dxr + dxl2, y0 + dyr + dyl2, dxr2 - dxr, dyr2 - dyr,
                 dxl - dxl2, dyl - dyl2, 'm',
                 std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
              go(x0 + dxr2 + dxl, y0 + dyr2 + dyl, dxl2 - dxl, dyl2 - dyl,
                 -dxr2, -dyr2, 'r', std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
            }
          } else
            msg("4-part-error1: %d, %d, %d, %d, %d, %d, %c", x0, y0, dxl, dyl,
                dxr, dyr, dir);
        } else // even-odd
        {
          if (dir == 'r') {
            if (dxl2 % 2 != 0)
              dxl2 = dxl - dxl2; // get it in a shape xx-eo
            if (dyl2 % 2 != 0)
              dyl2 = dyl - dyl2;
            if (abs(dxr + dyr) > 2) {
              go(x0, y0, dxl2, dyl2, dxr2, dyr2, 'l',
                 std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
              go(x0 + dxl2, y0 + dyl2, dxl - dxl2, dyl - dyl2, dxr2, dyr2, 'r',
                 std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
              go(x0 + dxr2 + dxl2, y0 + dyr2 + dyl2, dxl - dxl2, dyl - dyl2,
                 dxr - dxr2, dyr - dyr2, 'r',
                 std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
              go(x0 + dxr + dxl2, y0 + dyr + dyl2, -dxl2, -dyl2, dxr2 - dxr,
                 dyr2 - dyr, 'l', std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
            } else {
              go(x0, y0, dxl2, dyl2, dxr2, dyr2, 'l',
                 std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
              go(x0 + dxl2, y0 + dyl2, dxl - dxl2, dyl - dyl2, dxr2, dyr2, 'm',
                 std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
              go(x0 + dxl + dxr2, y0 + dyl + dyr2, dxr - dxr2, dyr - dyr2,
                 dxl2 - dxl, dyl2 - dyl, 'm',
                 std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
              go(x0 + dxl2 + dxr, y0 + dyl2 + dyr, -dxl2, -dyl2, dxr2 - dxr,
                 dyr2 - dyr, 'l', std::forward<RenderCallback>(render),
                 std::forward<Message>(msg));
            }
          } else
            msg("4-part-error2: %d, %d, %d, %d, %d, %d, %c", x0, y0, dxl, dyl,
                dxr, dyr, dir);
        }
      }
    }
  } else // dir=="m" -> divide into 3x3 parts
  {
    if ((abs(dxl + dyl) % 2 == 0) && (abs(dxr + dyr) % 2 == 0))
      msg("9-part-error1: %d, %d, %d, %d, %d, %d, %c", x0, y0, dxl, dyl, dxr,
          dyr, dir, std::forward<RenderCallback>(render),
          std::forward<Message>(msg));
    int dxl2, dyl2, dxr2, dyr2;
    if (abs(dxr + dyr) % 2 == 0) // even-odd: oeo-ooo
    {
      dxl2 = dxl / 3;
      dyl2 = dyl / 3;
      dxr2 = dxr / 3;
      dyr2 = dyr / 3;
      if ((dxl2 + dyl2) % 2 == 0) // make it odd
      {
        dxl2 = dxl - 2 * dxl2;
        dyl2 = dyl - 2 * dyl2;
      }
      if ((dxr2 + dyr2) % 2 == 0) // make it odd (not necessary, however results
                                  // are better for 12x30, 18x30 etc.)
      {
        if (abs(dxr2 + dyr2) != 2) {
          if (dxr < 0)
            dxr2++;
          if (dxr > 0)
            dxr2--; // dont use else here !
          if (dyr < 0)
            dyr2++;
          if (dyr > 0)
            dyr2--; // dont use else here !
        }
      }
    } else // odd-even: ooo-oeo
    {
      dxl2 = dxl / 3;
      dyl2 = dyl / 3;
      dxr2 = dxr / 3;
      dyr2 = dyr / 3;
      if ((dxr2 + dyr2) % 2 == 0) // make it odd
      {
        dxr2 = dxr - 2 * dxr2;
        dyr2 = dyr - 2 * dyr2;
      }
      if ((dxl2 + dyl2) % 2 == 0) // make it odd (not necessary, however results
                                  // are better for 12x30, 18x30 etc.)
      {
        if (abs(dxl2 + dyl2) != 2) {
          if (dxl < 0)
            dxl2++;
          if (dxl > 0)
            dxl2--; // dont use else here !
          if (dyl < 0)
            dyl2++;
          if (dyl > 0)
            dyl2--; // dont use else here !
        }
      }
    }
    if (abs(dxl + dyl) < abs(dxr + dyr)) {
      go(x0, y0, dxl2, dyl2, dxr2, dyr2, 'm',
         std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxl2 + dxr2, y0 + dyl2 + dyr2, -dxr2, -dyr2, dxl - 2 * dxl2,
         dyl - 2 * dyl2, 'm', std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxl - dxl2, y0 + dyl - dyl2, dxl2, dyl2, dxr2, dyr2, 'm',
         std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxl + dxr2, y0 + dyl + dyr2, dxr - 2 * dxr2, dyr - 2 * dyr2,
         -dxl2, -dyl2, 'm', std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxr - dxr2 + dxl - dxl2, y0 + dyr - dyr2 + dyl - dyl2,
         2 * dxl2 - dxl, 2 * dyl2 - dyl, 2 * dxr2 - dxr, 2 * dyr2 - dyr, 'm',
         std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxl2 + dxr2, y0 + dyl2 + dyr2, dxr - 2 * dxr2, dyr - 2 * dyr2,
         -dxl2, -dyl2, 'm', std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxr - dxr2, y0 + dyr - dyr2, dxl2, dyl2, dxr2, dyr2, 'm',
         std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxr + dxl2, y0 + dyr + dyl2, -dxr2, -dyr2, dxl - 2 * dxl2,
         dyl - 2 * dyl2, 'm', std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxr - dxr2 + dxl - dxl2, y0 + dyr - dyr2 + dyl - dyl2, dxl2, dyl2,
         dxr2, dyr2, 'm', std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
    } else {
      go(x0, y0, dxl2, dyl2, dxr2, dyr2, 'm',
         std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxl2 + dxr2, y0 + dyl2 + dyr2, dxr - 2 * dxr2, dyr - 2 * dyr2,
         -dxl2, -dyl2, 'm', std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxr - dxr2, y0 + dyr - dyr2, dxl2, dyl2, dxr2, dyr2, 'm',
         std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxr + dxl2, y0 + dyr + dyl2, -dxr2, -dyr2, dxl - 2 * dxl2,
         dyl - 2 * dyl2, 'm', std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxr - dxr2 + dxl - dxl2, y0 + dyr - dyr2 + dyl - dyl2,
         2 * dxl2 - dxl, 2 * dyl2 - dyl, 2 * dxr2 - dxr, 2 * dyr2 - dyr, 'm',
         std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxl2 + dxr2, y0 + dyl2 + dyr2, -dxr2, -dyr2, dxl - 2 * dxl2,
         dyl - 2 * dyl2, 'm', std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxl - dxl2, y0 + dyl - dyl2, dxl2, dyl2, dxr2, dyr2, 'm',
         std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxl + dxr2, y0 + dyl + dyr2, dxr - 2 * dxr2, dyr - 2 * dyr2,
         -dxl2, -dyl2, 'm', std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
      go(x0 + dxr - dxr2 + dxl - dxl2, y0 + dyr - dyr2 + dyl - dyl2, dxl2, dyl2,
         dxr2, dyr2, 'm', std::forward<RenderCallback>(render),
         std::forward<Message>(msg));
    }
  }
}

template <typename RenderCallback>
void spacefill(
    int ww, int hh,
    RenderCallback &&render) // width, height, render callback, render context
{
  auto msg = [](auto a...) {};
  if (hh > ww) // go top->down
  {
    if ((hh % 2 == 1) && (ww % 2 == 0))
      go(0, 0, ww, 0, 0, hh, 'm', std::forward<RenderCallback>(render), msg); // go diagonal
    else
      go(0, 0, ww, 0, 0, hh, 'r', std::forward<RenderCallback>(render), msg); // go top->down
  } else                           // go left->right
  {
    if ((ww % 2 == 1) && (hh % 2 == 0))
      go(0, 0, ww, 0, 0, hh, 'm', std::forward<RenderCallback>(render), msg); // go diagonal
    else
      go(0, 0, ww, 0, 0, hh, 'l', std::forward<RenderCallback>(render), msg); // go left->right
  }
}
}
using hilbert_piano::spacefill;
