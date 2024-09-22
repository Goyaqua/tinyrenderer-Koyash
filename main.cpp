#include "tgaimage.h"
#include <vector>
#include <cmath>
#include "model.h"


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
Model *model = NULL;
const int width  = 2500;
const int height = 2500;

//Bresenham's Line Drawing Algorithm
void line( int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color){

    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)){
        std::swap(x0,y0);
        std::swap(x1,y1);
        steep = true;
    }

    if (x0>x1){
        std::swap(x0,x1);
        std::swap(y0,y1);
    }

    for(int x = x0; x <= x1; x++){
        float t = (x-x0)/(float)(x1-x0);
        int y = y0 * (1.0 - t) + y1 * t;
        if (steep){
            image.set(y,x,color);
        } else {
            image.set(x,y,color);
        }
    }
}

int main(int argc, char** argv) {

    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/SephirothR.obj");
    }
    float translateY = -1300;
    TGAImage image(width, height, TGAImage::RGB);
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);

        for (int j=0; j<3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j+1)%3]);

            float tempX0 = v0.x;   // Store original x
            v0.x = v0.z;           // New x is original z
            v0.z = -tempX0;        // New z is -original x

            float tempX1 = v1.x;   // Store original x
            v1.x = v1.z;           // New x is original z
            v1.z = -tempX1;

            float scaleFactor = 1.;  // Try values <1 to zoom out, >1 to zoom in
            int x0 = (v0.x * scaleFactor + 1.) * width / 2.;
            int y0 = (v0.y * scaleFactor + 1.) * height / 2. + translateY;
            int x1 = (v1.x * scaleFactor + 1.) * width / 2.;
            int y1 = (v1.y * scaleFactor + 1.) * height / 2. + translateY;

            line(x0, y0, x1, y1, image, white);
        }
    }

	image.flip_vertically(); //to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}


