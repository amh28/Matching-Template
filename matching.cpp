#include <iostream>
#include <math.h>
#include "CImg.h"
#include <omp.h>
#include <chrono>


using namespace std;
using namespace cimg_library;

typedef CImg<float> IF;
typedef size_t ST;


IF find_match(IF &img, IF &temp)
{
	ST W = img.width(),
	
	   H = img.height(),
	   w = temp.width(),
	   h = temp.height();

	   
	img = img.get_channel(0);
	temp = temp.get_channel(0);

	
	ST max_sim = 99999999;	//THIS VALUE MUST CHANGE
	ST max_i, max_j;
	
	#pragma omp parallel for schedule(dynamic,1) collapse(2)
	for(ST i=0; i<W-w; i++)
	{
		for(ST j=0; j<H-h; j++)
		{
			ST tmp_sim = 0;

			for(ST k=0; k<w; k++)
				for(ST l=0; l<h; l++)
					tmp_sim += abs(img(i+k,j+l)-temp(k,l));

			if(tmp_sim<=max_sim) {	max_sim=tmp_sim;	max_i=i; 	max_j=j;	}
			
		}
	}
	
	IF match_image(w,h,1,1,0);
	for(ST k=0; k<w; k++)
		for(ST l=0; l<h; l++)
			match_image(k,l) = img(max_i + k, max_j + l);
	
	return match_image;
			
}
int main()
{
	IF img("doggy.jpeg");
	IF temp("face.jpeg");
	auto start_time = std::chrono::high_resolution_clock::now();
	IF match_image = find_match(img,temp);
	auto end_time = std::chrono::high_resolution_clock::now();
	auto time = end_time - start_time;
	std::cout << "Paralell took " << std::chrono::duration_cast<std::chrono::milliseconds>(time).count() << " to run.\n";	
	match_image.display();
	return 0;
}
	
