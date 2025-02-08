#pragma once

class DCFilter {
public:
  DCFilter() : x1(0.0f), y1(0.0f), coeff(0.995f) {}

  void setCoeff(float coeff) { this->coeff = coeff; }

  float process(float input) {
    float y0 = input - x1 + coeff * y1;
    x1 = input;
    y1 = y0;
    return y0;
  }

private:
  float x1, y1, coeff;
};
