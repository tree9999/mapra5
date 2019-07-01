#include <vector>
#include <fstream>
#include <string>


class GreyScale
{
private:
	std::vector<float> pic;
        int Height;
        int Width;
public:
	explicit GreyScale(int m=0, int n=0);
	GreyScale (const GreyScale&);

	float& operator () (int, int);
    	float operator() (int, int) const;
	int GetWidth();
	int GetHeight();
	int GetWidth() const;
	int GetHeight() const;
	std::vector<float> GetPicture();

	GreyScale& operator = (const GreyScale&);
	GreyScale& operator += (const GreyScale&);
	GreyScale& operator -= (const GreyScale&);
	friend void comment(std::istream&);
	friend std::istream& operator >> (std::istream&, GreyScale&);
	friend std::ofstream& operator << (std::ofstream&, const GreyScale&);

	static void PicError (const std::string& str);

	GreyScale& Resize (int, int);

	GreyScale Convolve(const float mask[], int size = 3);
	GreyScale LinTrans(float a, float b);
	GreyScale Binarize(float c);
	GreyScale Blur();
	GreyScale Clamp() const;
	GreyScale Contrast();
	GreyScale Kirsch();
	GreyScale Laplace();
	GreyScale Invert();
	GreyScale Median();
	GreyScale Sobel();

};
