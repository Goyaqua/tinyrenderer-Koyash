#include "tgaimage.h"
#include <vector>
#include <cmath>
#include "geometry.h"


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

void fill(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
    // Sort the vertices by their x-coordinates
    if (t0.x > t1.x) std::swap(t0, t1);
    if (t0.x > t2.x) std::swap(t0, t2);
    if (t1.x > t2.x) std::swap(t1, t2);

    // Fill the triangle
    for (int x = t0.x; x <= t2.x; x++) {
        // Calculate the y-coordinates at this x for both edges
        int y0, y1;

        // Check if x is in the range for t0-t1 edge
        if (x < t1.x) {
            //left side interpolation
            float t1_factor = (x - t0.x) / (float)(t1.x - t0.x);
            y0 = t0.y * (1.0f - t1_factor) + t1.y * t1_factor;  // y-coordinate on t0-t1 edge
        } else {
            //right side interpolation
            float t1_factor = (x - t1.x) / (float)(t2.x - t1.x);
            y0 = t1.y * (1.0f - t1_factor) + t2.y * t1_factor;  // y-coordinate on t1-t2 edge
        }

        // Always calculate the y-coordinate on t0-t2 edge
        float t2_factor = (x - t0.x) / (float)(t2.x - t0.x);
        y1 = t0.y * (1.0f - t2_factor) + t2.y * t2_factor;  // y-coordinate on t0-t2 edge

        // Ensure y0 is the lower and y1 is the upper
        if (y0 > y1) std::swap(y0, y1);

        // Fill vertical line between y0 and y1
        for (int y = y0; y <= y1; y++) {
            image.set(x, y, color); // Fill the pixel
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


