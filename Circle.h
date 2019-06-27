#ifndef CIRCLE_H
#define CIRCLE_H

#include <math.h>
#include <algorithm>

// parameters
static double s_thr = 10e-30; // the threshold of the step to release pain or surrender resource
static double p_thr = 10e-6;  // the threshold of the system potential to terminate the program
static double shrink = 10;    // the factor to initialize the step which is supposed to be proportional to the size
static double u_ratio = 0.8;  // the ratio for decreasing the step once the new system potential increases
static double reg = 0.8;      // the regularization coefficient to simulate circles with elastic connections from the origin

class Circle {
public:
    double radius;
    double center_x = 0.0;
    double center_y = 0.0;
    std::vector<bool> boundary = {false, false, false, false}; // record whether the circle touches the boundary

    Circle(double r) : radius(r) {};
    ~Circle() {};
    double Distance_of_Center(const Circle& obj);
    double objPotential(Circle obj);
    double marginPotential(double L, double W);
    double gradient_x(std::vector<Circle>& circles, double L);
    double gradient_y(std::vector<Circle>& circles, double W);
    bool operator ==(const Circle& obj);
};

bool Circle :: operator==(const Circle& obj) {
    return radius == obj.radius && center_x == obj.center_x && center_y == obj.center_y;
}

double Circle :: Distance_of_Center(const Circle& obj) {
    return sqrt(pow(center_x - obj.center_x, 2) + pow(center_y - obj.center_y, 2));
}

//Hook's law without the constant coefficient
double Circle :: objPotential(Circle obj) {
    if(Distance_of_Center(obj) < radius + obj.radius) {
        return radius + obj.radius - Distance_of_Center(obj);
    }
    else { return 0; }
}

double Circle :: marginPotential(double L, double W) {
    double _potential = 0;
    boundary[0] = false, boundary[1] = false, boundary[2] = false, boundary[3] = false;
    if(center_x + L/2 < radius) { _potential  += radius - center_x - L/2; boundary[0] = true; }
    if(L/2 - center_x < radius) { _potential  += radius - L/2 + center_x; boundary[1] = true; }
    if(center_y + W/2 < radius) { _potential  += radius - center_y - W/2; boundary[2] = true; }
    if(W/2 - center_y < radius) { _potential  += radius - W/2 + center_y; boundary[3] = true; }
    return _potential;
}

double Circle :: gradient_x(std::vector<Circle> &circles, double L) {
    double gradient = 0;
    // first calculate the gradient of potentials towards other circles
    for(const auto& obj : circles) {
        if(!(*this == obj)) {
            double p = objPotential(obj);
            if(p != 0) { gradient += 4*p*(obj.center_x - center_x) / Distance_of_Center(obj); }
        }
    }
    // then calculate the gradient of potentials against the boundary
    if(boundary[0]) { gradient += (2*center_x + L - 2*radius); }
    if(boundary[1]) { gradient += (2*center_x - L + 2*radius); }

    return gradient + 2*reg*center_x;
}

double Circle :: gradient_y(std::vector<Circle> &circles, double W) {
    double gradient = 0;
    // first calculate the gradient of potentials towards other circles
    for(const auto& obj : circles) {
        if(!(*this == obj)) {
            double p = objPotential(obj);
            if(p != 0) { gradient += 4*p*(obj.center_y - center_y) / Distance_of_Center(obj); }
        }
    }
    // then calculate the gradient of potentials against the boundary
    if(boundary[2]) { gradient += (2*center_y + W - 2*radius); }
    if(boundary[3]) { gradient += (2*center_y - W + 2*radius); }

    return gradient + 2*reg*center_y;
}

#endif //CIRCLE_H
