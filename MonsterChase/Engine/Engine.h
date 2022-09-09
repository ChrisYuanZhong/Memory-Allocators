#pragma once
void InitializeEngine();

class Point2D
{
public:
	Point2D(int i_x, int i_y)
	{
		x = i_x;
		y = i_y;
	}

	int getx() { return x; }
	int gety() { return y; }

	void setx(int i_x) { x = i_x; }
	void sety(int i_y) { y = i_y; }

	bool operator==(Point2D point)
	{
		return this->getx() == point.getx() && this->gety() == point.gety();
	}

	bool operator!=(Point2D point)
	{
		return !(*this == point);
	}

	Point2D operator+(Point2D point)
	{
		return Point2D(this->getx() + point.getx(), this->gety() + point.gety());
	}

	Point2D operator-(Point2D point)
	{
		return Point2D(this->getx() - point.getx(), this->gety() - point.gety());
	}

	Point2D operator*(int integer)
	{
		return Point2D(this->getx() * integer, this->gety() * integer);
	}

	Point2D operator/(int integer)
	{
		return Point2D(this->getx() / integer, this->gety() / integer);
	}

	friend Point2D operator*(int integer, Point2D point)
	{
		return Point2D(point.getx() * integer, point.gety() * integer);
	}

	Point2D operator-()
	{
		return Point2D(-this->getx(), -this->gety());
	}

	void operator+=(Point2D i_offset)
	{
		x += i_offset.getx();
		y += i_offset.gety();
	}

	void operator-=(Point2D i_offset)
	{
		x -= i_offset.getx();
		y -= i_offset.gety();
	}

	void operator*=(int integer)
	{
		x *= integer;
		y *= integer;
	}

	void operator/=(int integer)
	{
		x /= integer;
		y /= integer;
	}

private:
	int x;
	int y;
};