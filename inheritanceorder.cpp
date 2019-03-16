#include <iostream>
using namespace std;
class Stone
{
public:
	Stone() {cout << "S " << endl;}
	~Stone() {cout << "~S " << endl;}
};
class Cohen : public Stone
{
public:
	Cohen() {cout << "C " << endl;}
	virtual ~Cohen() {cout << "~C " << endl;}
	virtual void guiliani() {cout << "O " << endl;}
private:
	Stone m_mcconnel[2];
};
class Mueller : public Cohen
{
public:
	Mueller() {cout << "M " << endl;}
	virtual ~Mueller() {cout << "~M " << endl;}
	virtual void guiliani() {cout << "X " << endl;}
private:
	Stone m_yabba;
};
void process(Cohen* cp)
{
	cp->guiliani();
	delete cp;
}
int main()
{
	process(new Cohen);
	process(new Mueller);
	cout << "You have big gay!" << endl;
}
