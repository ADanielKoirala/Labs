// filled/hollow figures demonstrates Bridge Design Pattern,
// square is either hollow or square and is painted with a particular character,
// it is bridged over Figure --> Fill abstract body/handle
// Wayne Cheng
// 3/30/2018

#include <iostream>

using std::cout; 
using std::endl; 
using std::cin;

// abstract body
class Fill
{
	public:
		Fill(char border, char internal): borderChar_(border), internalChar_(internal){}

		virtual char getBorder() { return borderChar_; }
		virtual char getInternal() { return internalChar_; }

		virtual ~Fill() {}

	protected:
		char borderChar_;
        char internalChar_;
};

// concrete body
class Hollow: public Fill
{
	public:
		Hollow(char borderChar): Fill(borderChar, ' ') {}
};


// another concrete body
class Filled: public Fill 
{
	public:
		Filled(char fillChar) : Fill(fillChar, fillChar) {}
};

class EnhancedFilled : public Filled
{
    public:
        EnhancedFilled(char borderChar, char internalChar) : Filled(borderChar)
        {
            internalChar_ = internalChar;
        }
};

// abstract handle
class Figure 
{
	public:
		Figure(int size, Fill* fill): size_(size), fill_(fill){}
		virtual void draw() =0;
		virtual ~Figure(){}

	protected:
		int size_;
		Fill *fill_;
};

// concrete handle
class Square: public Figure
{
    public:
        Square(int size, Fill* fill): Figure(size, fill){}

        void draw() override
        {
            for (int i = 0; i < size_; ++i)
            {
                for (int j = 0; j < size_; ++j)
                {
                    if (i == 0 || i == size_ - 1 || j == 0 || j == size_ - 1)
                        cout << fill_->getBorder();
                    else
                        cout << fill_->getInternal();
                }
                cout << endl;
            }
        }
};


int main()
{
	Fill* hollow = new Hollow('#');
    Figure* hollowBox = new Square(6, hollow);

    Fill* filled = new Filled('*');
    Figure* filledBox = new Square(6, filled);

    Fill* enhanced = new EnhancedFilled('@', '+');
    Figure* enhancedBox = new Square(6, enhanced);

    cout << "Hollow (#, space):" << endl;
    hollowBox->draw();
    cout << endl;

    cout << "Filled (*):" << endl;
    filledBox->draw();
    cout << endl;

    cout << "Enhanced Filled (@ border, + inside):" << endl;
    enhancedBox->draw();
    cout << endl;

    return 0;
}
