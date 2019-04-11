#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std; 
#define PI 3.14159265
class grayBMP {
private:	
	int width;
	int height;
	unsigned char header[54];
	unsigned char *pixels;
	unsigned char *plant;
	int size;
	string filename;


public:
	grayBMP(string _filename){
		filename = _filename;
		FILE *f = fopen(_filename.c_str(), "rb");
		plant=(unsigned char*)malloc(sizeof(unsigned char)*1024);
		fread(header, sizeof(unsigned char), 54, f);
		width = *(int*)(header+18);
		height = *(int*)(header+22);
		size = width * height; 
		fread(plant, sizeof(unsigned char), 1024, f);
		pixels = (unsigned char*)malloc(sizeof(unsigned char)*size);
		int r = fread(pixels, sizeof(unsigned char), size, f);
		if(r!=size)exit(-1);
		//    printf("%d, %d\n", r ,size);
		fclose(f);
    
		//    if(r!=size)exit(1);
  	}
	
	int getWidth(){
		return width;
	}
	int getHeight(){
		return height;
	}
	int getSize(){
		return size;
	}
	unsigned char* getImg(){
		return pixels;
	}
	unsigned char* getPlant(){
	return plant;
	}




};

double** list_Trigfunc(){
	unsigned short int i;

	double **ptr = (double**)malloc(sizeof(double*)*2);
	*ptr = (double*)malloc(sizeof(double)*360); //sin
	*(ptr+1) = (double*)malloc(sizeof(double)*360); //cos
	for(i=0; i<360; i++){
		*(*ptr+i) = cos((double)i*PI/180.0);
		*(*(ptr+1)+i) = sin((double)i*PI/180.0);
	}
	return ptr;
}
double p_max(double* ary, int num){
	int i;
	double max = *ary;

	for(i=0; i<num; i++){
		if(max<*(ary+i))
			max = *(ary+i);
	}
	return max;
}
double p_min(double* ary, int num){
	int i;
	double min = *ary;

	for(i=0; i<num; i++){
		if(min>*(ary+i))
			min = *(ary+i);
	}
	return min;
}

void max(double **table_ptr, int img_w, int img_h, int degree, int* new_hw){
	int x0 = 0.5 * (img_w - 1);     // point to rotate about
	int y0 = 0.5 * (img_h - 1);     // center of image
	double x_4[4], y_4[4], x_min, x_max, y_min, y_max; 


	x_4[0] = (0-x0) * *(*table_ptr+degree) - (0-y0) * *(*(table_ptr+1)+degree);
	y_4[0] = (0-x0) * *(*(table_ptr+1)+degree) + (0-y0) * *(*table_ptr+degree);

	x_4[1] = (0-x0) * *(*table_ptr+degree) - (img_h-1-y0) * *(*(table_ptr+1)+degree);
	y_4[1] = (0-x0) * *(*(table_ptr+1)+degree) + (img_h-1-y0) * *(*table_ptr+degree);

	x_4[2] = (img_w-1-x0) * *(*table_ptr+degree) - (img_h-1-y0) * *(*(table_ptr+1)+degree);
	y_4[2] = (img_w-1-x0) * *(*(table_ptr+1)+degree) + (img_h-1-y0) * *(*table_ptr+degree);

	x_4[3] = (img_w-1-x0) * *(*table_ptr+degree) - (0-y0) * *(*(table_ptr+1)+degree);
	y_4[3] = (img_w-1-x0) * *(*(table_ptr+1)+degree) + (0-y0) * *(*table_ptr+degree);

	x_max = p_max(x_4, 4);
	x_min = p_min(x_4, 4);
	y_max = p_max(y_4, 4);
	y_min = p_min(y_4, 4);
	new_hw[0] = ceil(x_max-x_min+1);
	new_hw[1] = ceil(y_max-y_min+1);
}    

unsigned char* rotate(unsigned char *img, int width, int height, int rot, int *new_width, int *new_height){
	double **ptr = list_Trigfunc();
	int *hw = (int*)malloc(sizeof(int)*2);
	int x0 = 0.5 * (width-1); 
	int y0 = 0.5 * (height-1);
	int _x0; 
	int _y0;
	int _x, _y, x, y, _rot;
	int pad;
	max(ptr, width, height, rot, hw);
	if((*hw)%4!=0){
	*hw=4-((*hw)%4)+(*hw);
	}
	*new_width = *hw;
	*new_height = *(hw+1);
	printf("New image size: width=%d, height=%d\n", *new_width, *new_height);
	free(hw);
	_x0 = 0.5 * (*new_width-1);
	_y0 = 0.5 * (*new_height-1);

	unsigned char *new_Img = (unsigned char*)malloc(sizeof(unsigned char)* (*new_width)*(*new_height));
	_rot = 360 - rot;

	for(int j=0; j<(*new_width) * (*new_height); j++){

		x = ((j%(*new_width))-_x0) * *(*ptr+_rot) - ((j/(*new_width))-_y0) * *(*(ptr+1)+_rot)+x0;
		y = ((j%(*new_width))-_x0) * *(*(ptr+1)+_rot) + ((j/(*new_width))-_y0) * *(*ptr+_rot)+y0;

		if(*(new_Img+j)==0 && (x>=0 && x<=(width-1)) && (y>=0 && y<=(height - 1))){
			*(new_Img+j) = *(img+(y * width + x));
		}
	}

	return new_Img;
}
bool BMPwriter(unsigned char* Img, string filename, int _width, int _height, unsigned char* plant){
	unsigned char header[54] = {
	0x42,//B
	0x4d,//M
	0, 0, 0, 0,  // file size
	0, 0,
	0, 0,
	54, 0, 0, 0, // offset
	40, 0, 0, 0, //BITMAPINFOHEADER size
	0, 0, 0, 0,
	0, 0, 0, 0,
	1, 0,        // planes
	8, 0,       // bit per pixel
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0
	};

	long width = 0;
	long height = 0;
	long pad = 0;
	long file_size = (long)_width * (long)_height + 54 + 1024 ;


	long offset = 54 + 1024;
	header[2] = (unsigned char)(file_size &0x000000ff);
	header[3] = (file_size >> 8) & 0x000000ff;
	header[4] = (file_size >> 16) & 0x000000ff;
	header[5] = (file_size >> 24) & 0x000000ff;
	header[10] = (unsigned char)(offset &0x000000ff);
	header[11] = (offset >> 8) &0x000000ff;
	header[12] = (offset >> 16) &0x000000ff;
	header[13] = (offset >> 24) &0x000000ff;
	width = (long)_width;
	header[18] = width & 0x000000ff;
	header[19] = (width >> 8) &0x000000ff;
	header[20] = (width >> 16) &0x000000ff;
	header[21] = (width >> 24) &0x000000ff;

	height = (long)_height;
	header[22] = height &0x000000ff;
	header[23] = (height >> 8) &0x000000ff;
	header[24] = (height >> 16) &0x000000ff;
	header[25] = (height >> 24) &0x000000ff; 
	//printf("%ld, %ld, %ld\n", file_size, width, height);
	FILE* fp = fopen(filename.c_str(),"w+b");
	if(fp == NULL)
		exit(-1);
	fwrite(header, sizeof(unsigned char), 54, fp);
	fwrite(plant, sizeof(unsigned char), 1024, fp);
	fwrite(Img, sizeof(unsigned char), _height*_width, fp);
	fclose(fp);
	return true;
}


int main() {
	bool save;
	int new_w, new_h;
	grayBMP a("lena.bmp");
	int *new_hw_p = (int*)malloc(sizeof(int)*2);
	int *new_width=(int*)malloc(sizeof(int));
	int *new_height=(int*)malloc(sizeof(int));
	unsigned char *img = rotate(a.getImg(), a.getWidth(), a.getHeight(), 168, new_width, new_height);

	save = BMPwriter(img, "rotation_1.bmp", *new_width, *new_height, a.getPlant());
	if(save==true)
		printf("Success!!!\n");
	else
		printf("ERROR\n");
	return 0;

}

    
