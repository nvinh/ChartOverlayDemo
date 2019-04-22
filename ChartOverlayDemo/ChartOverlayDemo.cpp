/*
Chart Overlay Demo
Author: Vinh Nguyen
For Testing purpose
This snippet can be run online at cpp.sh/5rhhb
*/
#include <iostream>
#include <algorithm> // for std::sort
#include <vector>
using namespace std;
// for simplifying the illustration, here combined both .h and .cpp file
struct Point {
	int x;	// for simplifying the example, here we use integer instead of float or double
	int y;
	Point (int px, int py):x(px), y(py){}
	Point() { x = 0; y = 0; }
	friend std::ostream& operator<<(std::ostream &out, const Point &p);
};
struct RGB {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	RGB (unsigned char cr, unsigned char cg, unsigned char cb):r(cr), g(cg), b(cb){}
	RGB() { r = 0; g = 0; b = 0; }
	friend std::ostream& operator<<(std::ostream &out, const RGB &rgb);
};
class Chart
{
private:
	Point m_p1;		// we assume that input data has been standardized, p1 is on the top left of p2
	Point m_p2;
	RGB m_rgb;
public:
	Chart(Point p1, Point p2, RGB rgb):m_p1(p1),m_p2(p2),m_rgb(rgb){}
	Chart() {}
	void setChart(Point p1, Point p2, RGB rgb) {
		m_p1 = p1;
		m_p2 = p2;
		m_rgb = rgb;
	}
	bool valueInRange(int value, int min, int max) const
	{
		if (min > max) {
			min += max;
			max = min - max;
			min -= max;
		}
		return (value >= min) && (value <= max);
	}
	bool chartOverlap(Chart b) const {
		bool xOverlap = valueInRange(m_p1.x, b.m_p1.x, b.m_p2.x) ||
			valueInRange(b.m_p1.x, m_p1.x, m_p2.x);
		bool yOverlap = valueInRange(m_p1.y, b.m_p1.y, b.m_p2.y) ||
			valueInRange(b.m_p1.y, m_p1.y, m_p2.y);
		return xOverlap && yOverlap;
	}
	bool pointInChart(Point p) const {
		bool xOverlap = valueInRange(p.x, m_p1.x, m_p2.x);
		bool yOverlap = valueInRange(p.y, m_p1.y, m_p2.y);
		return xOverlap && yOverlap;
	}
	RGB getColor() const {
		return m_rgb;
	}
	RGB getAverageColor(Chart b) const {
		unsigned char cr = m_rgb.r/2 + b.m_rgb.r/ 2 + (m_rgb.r & b.m_rgb.r & 1);	//avoid overflowing, (a / 2) + (b / 2) + (a & b & 1)
		unsigned char cg = m_rgb.g/2 + b.m_rgb.g / 2 + (m_rgb.g & b.m_rgb.g & 1);
		unsigned char cb = m_rgb.b/2 + b.m_rgb.b / 2 + (m_rgb.b & b.m_rgb.b & 1);
		RGB rgb(cr,cg,cb);
		return rgb;
	}
	friend std::ostream& operator<<(std::ostream &out, const Chart &c);
};
class View {
	Chart m_a;
	Chart m_b;
public:
	View(Chart a, Chart b):m_a(a),m_b(b){}
	View(Chart a):m_a(a) {
		Point p(0,0);				// if the 2nd chart is not initialized, we assume that it will be at {(0,0),(0,0)} and the colour is (0,0,0)
		RGB rgb(0,0,0);
		m_b = Chart(p, p, rgb);		// just for illustration for thr default value of m_b
	}
	void setCharts(Chart a, Chart b) {
		m_a = a;
		m_b = b;
	}
	bool doChartsOverlap() {
		return m_a.chartOverlap(m_b);
	}
	RGB getColour(Point p) {
		if (m_a.pointInChart(p) && m_b.pointInChart(p)) return m_a.getAverageColor(m_b);
		if (m_a.pointInChart(p) && !m_b.pointInChart(p)) return m_a.getColor();
		if (!m_a.pointInChart(p) && m_b.pointInChart(p)) return m_b.getColor();
		RGB rgb(0,0,0);
		return rgb;
	}
};
std::ostream& operator<<(std::ostream &out, const Point &p) {
	out << "point [" << p.x << "," << p.y << "]";
	return out;
}
std::ostream& operator<<(std::ostream &out, const RGB &rgb) {
	out << "rgb [" << +rgb.r << "," << +rgb.g << "," << +rgb.b << "]";
	return out;
}
std::ostream& operator<<(std::ostream &out, const Chart &chart) {
	out << "chart (" << chart.m_p1 << "," << chart.m_p2 << "," << chart.m_rgb << ")";
	return out;
}
int main()
{
Point p1(10,30);
	Point p2(20,15);
	Point p3(15, 20);
	Point p4(30, 10);
	RGB c1(50, 50, 50);
	RGB c2(150, 150, 150);
	Chart A(p1, p2, c1);
	Chart B(p3, p4, c2);
	View view(A, B);
	Point p(10, 30);
	RGB rgb(view.getColour(p));
	cout << "Testing point p: " << p << "\n";
	cout << A << "\n" << B << "\n";
	cout << "overlap: " << view.doChartsOverlap() << "\n";
	cout << "Check: " << p << " has color " << rgb << "\n";
	p.x = 15;
	p.y = 20;
	rgb = view.getColour(p);
	cout << "Check: " << p << " has color " << rgb << "\n";
	p.x = 15;
	p.y = 15;
	rgb = view.getColour(p);
	cout << "Check: " << p << " has color " << rgb << "\n";
	p.x = 30;
	p.y = 10;
	rgb = view.getColour(p);
	cout << "Check: " << p << " has color " << rgb << "\n";
	p.x = 1;
	p.y = 1;
	rgb = view.getColour(p);
	cout << "Check: " << p << " has color " << rgb << "\n";
	//--------------------------------
	p2.x = 10;
	p2.y = 30;	
	A.setChart(p1, p2, A.getColor());
	view.setCharts(A, B);
	cout << A << "\n" << B << "\n";
	cout << "overlap: " << view.doChartsOverlap() << "\n";
	p.x = 20;
	p.y = 25;
	rgb = view.getColour(p);
	cout << "Check: " << p << " has color " << rgb << "\n";
	return 0;
}
