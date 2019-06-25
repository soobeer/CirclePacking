#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include "Circle.h"

// overload max function
int max(std::vector<double>& _array) {
    double cur = 0;
    int index = 0;
    for(int i = 0; i < _array.size(); ++i) {
        if(_array[i] > cur) { cur = _array[i]; index = i; }
    }
    return index;
}

// overload min function
int min(std::vector<double>& _array) {
    double cur = _array[0];
    int index = 0;
    for(int i = 1; i < _array.size(); ++i) {
        if(_array[i] < cur) { cur = _array[i]; index = i; }
    }
    return index;
}

// calculate the system potential
double system_potential(std::vector<Circle>& circles, std::vector<double>& uis, double L, double W) {
    double u = 0;
    int index = 0;
    for(auto& ci : circles) {
        double ui = 0;
        ui += ci.marginPotential(L, W);
        for(const auto& obj : circles) {
            if(!(ci == obj)) {
                ui += ci.objPotential(obj);
            }
        }
        uis[index] = ui + reg*(pow(ci.center_x, 2) + pow(ci.center_y, 2)); // add the regularization term
        index ++;
        u += ui;
    }

    return u;
}

// update all circles' positions by the gradient of system potential
void update_coordinates(std::vector<Circle>& circles, double L, double W, double step) {
    for(auto& ci : circles) {
        ci.center_x = ci.center_x - ci.gradient_x(circles, L)*step;
        ci.center_y = ci.center_y - ci.gradient_y(circles, W)*step;
    }
}

// randomly pick a new position from five options
void quasi_human(std::vector<Circle>& circles, double L, double W, int index) {
    srand(time(NULL));
    switch(rand()%5) {
        case 0: {
            circles[index].center_x = circles[index].radius - L/2;
            circles[index].center_y = circles[index].radius - W/2;
            break;
        }  // left-bottom corner
        case 1: {
            circles[index].center_x = circles[index].radius - L/2;
            circles[index].center_y = W/2 - circles[index].radius;
            break;
        }  // left-top corner
        case 2: {
            circles[index].center_x = L/2 - circles[index].radius;
            circles[index].center_y = W/2 - circles[index].radius;
            break;
        } // right-top corner
        case 3: {
            circles[index].center_x = L/2 - circles[index].radius;
            circles[index].center_y = circles[index].radius - W/2;
            break;
        } // right-bottom corner
        case 4: {
            circles[index].center_x = 0;
            circles[index].center_y = 0;
            break;
        }  // center
    }
}

void quasi_physic(std::vector<Circle>& circles, double L, double W) {
    int packing_num = circles.size();
    double s_ini = std::min(L, W) / shrink; // the step should be proportional to the size of the rectangle
    std::vector<double> uis(packing_num, 0); // store each circle's potential
    double u_old = 0, step = s_ini;
    int candi = 0; // track the last candidate whose pain was released or resource was surrendered
    //bool jd = false; // whether to release the pain or surrender the resource
    while(true) {
        if(step < s_thr) {
            std::vector<double> rdp(packing_num, 0);
            for(int i = 0; i < packing_num; ++i) { rdp[i] = uis[i] / pow(circles[i].radius, 2); }
            int new_candi = max(rdp);
            if(candi == -1 || new_candi != candi) {
                // change the index coordinates
                quasi_human(circles, L, W, new_candi);
                candi = new_candi;
            }
            else {
                // change the min(uis) coordinates
                new_candi = min(uis);
                quasi_human(circles, L, W, new_candi);
                candi = -1;
            }
            step = s_ini;
        }
        else {
            double u_cur = system_potential(circles, uis, L, W);
            if(u_cur < p_thr) { break; }
            if(u_cur < u_old) { update_coordinates(circles, L, W, step); }
            else { step = u_ratio*step; update_coordinates(circles, L, W, step); }
            u_old = u_cur;
        }
    }
}

int main() {
    // input streaming module
    double L, W;
    std::cout << "Please input the rectangle's dimensions: " << std::endl;
    std::cout << "Length: ";
    std::cin >> L;
    std::cout << "Width: ";
    std::cin >> W;
    std::cout << "Please input the radii of circles you want to pack: " << std::endl;
    double _radius;
    std::vector<Circle> circles;
    srand (time(NULL));
    while(std::cin >> _radius) {
        Circle new_circle(_radius);
        new_circle.center_x = rand()*L/RAND_MAX - L/2;
        new_circle.center_y = rand()*W/RAND_MAX - W/2;
        circles.push_back(new_circle);
        std::cout << "Please input again if more circles need to be packed, or type 'n' to terminate: " << std::endl;
    }
//    for(const auto& ci : circles) { std::cout << ci.center_x << " " << ci.center_y << std::endl; }


    quasi_physic(circles, L, W);
    std::ofstream coords;
    coords.open("circles.txt");
    coords << L << " " << W << std::endl;
    for(const auto& ci : circles) {
        coords << ci.radius << " " << ci.center_x << " " << ci.center_y << std::endl;
    }
    coords.close();

    return 0;
}