#include "tgaimage.h"
#include <vector>
#include <cmath>
#include "geometry.h"
#include <algorithm>


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);

const int width  = 200;
const int height = 200;

//Bresenham's Line Drawing Algorithm
void line( Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color){

    bool steep = false;
    if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y)){
        std::swap(p0.x,p0.y);
        std::swap(p1.x,p1.y);
        steep = true;
    }

    if (p0.x>p1.x){
        std::swap(p0.x,p1.x);
        std::swap(p0.y,p1.y);
    }

    for(int x = p0.x; x <= p1.x; x++){
        float t = (x - p0.x)/(float)(p1.x - p0.x);
        int y = p0.y * (1.0 - t) + p1.y * t;
        if (steep){
            image.set(y,x,color);
        } else {
            image.set(x,y,color);
        }
    }
}

void triangle( Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color){

    line(t0,t1,image,color);
    line(t1,t2,image,color);
    line(t2,t0,image,color);
}

// Fill the triangle using scanline algorithm
void fill(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
    // Sort the points based on the x-coordinate
    if (t0.x > t1.x) std::swap(t0, t1);
    if (t0.x > t2.x) std::swap(t0, t2);
    if (t1.x > t2.x) std::swap(t1, t2);

    // Now t0 is the leftmost, t1 is the middle, and t2 is the rightmost point.

    // Fill the left part (from t0 to t1)
    for (int x = t0.x; x <= t1.x; x++) {
        int y0 = t0.y + (t1.y - t0.y) * (x - t0.x) / (t1.x - t0.x); // Interpolated y at x
        int y1 = t0.y + (t2.y - t0.y) * (x - t0.x) / (t2.x - t0.x); // Interpolated y at x
        if (y0 > y1) std::swap(y0, y1); // Ensure y0 is the smaller y
        for (int y = y0; y <= y1; y++) {
            image.set(x, y, color); // Set pixel color
        }
    }

    // Fill the right part (from t1 to t2)
    for (int x = t1.x; x <= t2.x; x++) {
        int y0 = t1.y + (t2.y - t1.y) * (x - t1.x) / (t2.x - t1.x); // Interpolated y at x
        int y1 = t1.y + (t0.y - t1.y) * (x - t1.x) / (t0.x - t1.x); // Interpolated y at x
        if (y0 > y1) std::swap(y0, y1); // Ensure y0 is the smaller y
        for (int y = y0; y <= y1; y++) {
            image.set(x, y, color); // Set pixel color
        }
    }
}




int main(int argc, char** argv) {

    TGAImage image(width, height, TGAImage::RGB);

    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};


    triangle(t0[0], t0[1], t0[2], image, red);
    fill(t0[0], t0[1], t0[2], image, red);  // Fill the red triangle

    triangle(t1[0], t1[1], t1[2], image, white);
    fill(t1[0], t1[1], t1[2], image, white);  // Fill the white triangle

    triangle(t2[0], t2[1], t2[2], image, green);
    fill(t2[0], t2[1], t2[2], image, green);

    image.flip_vertically(); //to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}


