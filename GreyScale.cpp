#include <algorithm>
#include <fstream>
#include "GreyScale.h"
#include <vector>
#include <string>

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>


using std::endl;

GreyScale::GreyScale(int m, int n)
{
    	pic.resize(m*n);
    	Height = m;
    	Width = n;
}


GreyScale::GreyScale(const GreyScale& x):pic(x.pic), Height(x.Height),Width(x.Width) { }


float& GreyScale::operator () (int i, int j)
{
	if(i<0) {
		return (*this)(0,j);
	}
	if(j<0) {
		return (*this)(i,0);
	}
	if(i>=Height) {
		return (*this)(Height-1,j);
	}
	if(j>=Width) {
		return (*this)(i,Width-1);
	}
	return pic[i*Width+j];
}

float GreyScale::operator () (int i, int j) const
{
	if(i<0)
    {
		return (*this)(0,j);
	}
	if(j<0)
	{
		return (*this)(i,0);
	}
	if(i>=Height)
	{
		return (*this)(Height-1,j);
	}
	if(j>=Width)
	{
		return (*this)(i,Width-1);
	}
	return pic[i*Width+j];
}

GreyScale& GreyScale::Resize (int m, int n)
{
	pic.clear();
	pic.resize(m*n);
	Height=m;
	Width=n;
	return (*this);
}

int GreyScale::GetWidth()
{
	return Width;
}

int GreyScale::GetWidth() const
{
	return Width;
}

int GreyScale::GetHeight()
{
	return Height;
}

int GreyScale::GetHeight() const
{
	return Height;
}

std::vector<float> GreyScale::GetPicture()
{
	return pic;
}

GreyScale& GreyScale::operator = (const GreyScale& x)
{
	this -> Resize(x.Height,x.Width);
	pic = x.pic;
	return *this;
}

GreyScale& GreyScale::operator += (const GreyScale& x)
{
	if (Height != x.Height || Width != x.Width)
        	PicError("Inkompatible Dimensionen fuer 'Bild += Bild'!");
	for(int i=0; i<Height*Width; i++)
    {
		pic[i]+=(x.pic)[i];
	}
	return *this;
}

GreyScale& GreyScale::operator -= (const GreyScale& x)
{
	if (Height != x.Height || Width != x.Width)
        	PicError("Inkompatible Dimensionen fuer 'Bild -= Bild'!");
	for(int i=0; i<Height*Width; i++)
    {
		pic[i]-=(x.pic)[i];
	}
	return (*this);
}

GreyScale GreyScale::Binarize(float c)
{
    GreyScale gayscale = (*this);
	//gayscale.Resize(Height,Width);
	for (int i=0; i<Height*Width; i++)
    {
        if (gayscale.pic[i]<c)
		{
		    gayscale.pic[i]=0;
		}
		if (gayscale.pic[i]>=c)
		{
		    gayscale.pic[i]=1;
		}
	}
	return gayscale;
}

GreyScale GreyScale::Clamp() const
{
	GreyScale gayscale = (*this);
	for (int i=0; i<Height*Width; i++)
	{
		if(gayscale.pic[i]<0)
        {
			gayscale.pic[i]=0;
		}
		if(gayscale.pic[i]>1)
		{
			gayscale.pic[i]=1;
		}
	}
	return gayscale;
}

GreyScale GreyScale::LinTrans(float a, float b)
{
    GreyScale gayscale = (*this);
	gayscale.Resize(Height,Width);
	for (int i=0; i<Height*Width; i++)
    {
		gayscale.pic[i]=a*pic[i] + b;
	}
	return gayscale;
}

GreyScale GreyScale::Contrast()
{
    GreyScale gayscale = (*this);
	//gayscale.Resize(Height,Width);
	float max = gayscale(0,0);
	float min = gayscale(0,0);
	for (int i = 0; i<Height; i++)
    {
		for (int j = 0; j<Width; j++)
		{
			if (gayscale(i,j)>max)
			{
				max=gayscale(i,j);
			}
			else if (gayscale(i,j)<min)
			{
				min=gayscale(i,j);
			}
		}
	}
	if (max == min)
    {
		gayscale.pic.resize(Height*Width,0.5);
	}
	else
	{
		float a=((float) 1)/(max-min);
		float b=-a*min;
		gayscale = this -> LinTrans(a, b);
	}
	return gayscale;
}

GreyScale GreyScale::Invert()
{
    GreyScale gayscale = (*this);
	gayscale=this -> LinTrans(-1, 1);
	return gayscale;
}

GreyScale GreyScale::Convolve(const float mask[], int size)
{
	if((size/2)*2==size)
		PicError("size muss ungerade sein (Convolve)");
	/*if(size*size!=mask.size())
		PicError("dimension des Filterkerns muss size x size sein");*/
	GreyScale gayscale=GreyScale(Height,Width);
	for (int i=0; i<Height; i++)
    {
		for (int j=0; j<Width; j++)
		{
			gayscale(i,j)=0;
			int s=size/2;
			for (int k=-s; k<=s; k++)
			{
				for (int l=-s; l<=s; l++)
				{
					gayscale(i,j)+=(*this)(i+k,j+l)*mask[(k+s)*size+(l+s)];
				}
			}
		}
	}
	return gayscale;
}



GreyScale GreyScale::Blur()
{
    GreyScale gayscale = (*this);
	float mask[]={0,1./5,0,1./5,1./5,1./5,0,1./5,0};
	gayscale=this -> Convolve(mask);
	return gayscale;
}

GreyScale GreyScale::Kirsch()
{
    GreyScale gayscale = (*this);
	float mask[]={1,3,3,-1,0,1,-3,-3,-1};
	gayscale=this -> Convolve(mask);
	return gayscale;
}

GreyScale GreyScale::Laplace()
{
    GreyScale gayscale = (*this);
	float mask[]={0,-1,0,-1,4,-1,0,-1,0};
	gayscale=this -> Convolve(mask);
	return gayscale;
}

GreyScale GreyScale::Sobel()
{
    GreyScale gayscale = (*this);
	GreyScale temp1=GreyScale(Height,Width);
	GreyScale temp2=GreyScale(Height,Width);
	gayscale.Resize(Height,Width);
	float DX[]={-1,0,1,-2,0,2,-1,0,1};
	float DY[]={1,2,1,0,0,0,-1,-2,-1};
	temp1=this -> Convolve(DX);
	temp2=this -> Convolve(DY);
	for(int i=0; i<Height; i++)
    {
		for (int j=0; j<Width; j++)
		{
			gayscale(i,j)=sqrt(temp1(i,j)*temp1(i,j) + temp2(i,j)*temp2(i,j));
		}
	}
	return gayscale;
}

GreyScale GreyScale::Median()
{
    GreyScale gayscale = (*this);
	//gayscale.Resize(Height,Width);
	for(int i=0; i<Height; i++)
    {
		for (int j=0; j<Width; j++)
		{
			float temp[9] = {(*this)(i,j),(*this)(i,j+1),(*this)(i+1,j),(*this)(i,j-1),(*this)(i-1,j),(*this)(i+1,j+1),(*this)(i-1,j-1),(*this)(i+1,j-1),(*this)(i-1,j+1)};
			for(int k=0; k<9; k++)
			{
       				for (int l=8; l>k; l--)
       				{
            				if(temp[l]<temp[l-1])
            				{
                                float t = temp[l];
                                temp[l]=temp[l-1];
                				temp[l-1]=t;
            				}
       				}
            }
			gayscale(i,j)=temp[4];
		}
	}
	return gayscale;
}



void comment(std::istream& ifs)
{
	std::string str;
	while (ifs.peek() == '#')
    {
		std::getline(ifs, str);
	}
}

std::istream& operator >> (std::istream& ifs, GreyScale& x)
{
	int d, width, height, n;
	ifs.get();
	ifs.get(); //die magic Number ist nicht relevant
	ifs.get();
	comment(ifs);
	ifs >> width;
	comment(ifs);
	ifs >> height;
	comment(ifs);
	ifs >> d;
	x.Resize(height, width);
	for (int i=0; i<height; i++)
    {
		for (int j=0; j<width; j++)
		{
			comment(ifs);
			ifs >> n;
			x(i,j)=((float) n)/d;
		}
	}
	if (ifs.rdstate()!=std::ios::eofbit)
    {
        std::cout << "oupsidoupsiiiii loser" << endl;
    }
	return ifs;
}

std::ofstream& operator << (std::ofstream& ofs, const GreyScale& x)
{
	ofs << "P2" << endl;
	ofs << x.Width << " " << x.Height << endl;
	ofs << 255 << endl;
	GreyScale gayscale=x.Clamp();
	for (int i=0; i<x.Height; i++)
    {
		for (int j=0; j<x.Width; j++)
		{
			ofs << " " << std::round(gayscale(i,j)*255);
		}
		ofs << endl;
	}

	return ofs;
}

// ----- Ausgabe der Fehlermeldung "str" -----

void GreyScale::PicError (const std::string& str)
{
    std::cerr << "\nPictureError: " << str << '\n' << std::endl;
    exit(1);
}
