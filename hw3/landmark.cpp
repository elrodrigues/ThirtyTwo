#include <iostream>
#include <string>
using namespace std;

class Landmark
{
public:
  Landmark(string color);
  string name() const;
  virtual string icon() const = 0;
  virtual string color() const;
  virtual ~Landmark(){}
private:
  string m_name;
};

Landmark::Landmark(string name)
  : m_name(name)
{}

string Landmark::color() const
{
  return "yellow";
}

string Landmark::name() const
{
  return m_name;
}

class Hotel : public Landmark
{
public:
  Hotel(string name);
  virtual string icon() const;
  // virtual string color() const {}
  virtual ~Hotel();
};

Hotel::Hotel(string name)
  : Landmark(name)
{}

string Hotel::icon() const
{
  return "bed";
}
Hotel::~Hotel()
{
  cout << "Destroying the hotel " << name() <<"." << endl;
}

class Restaurant : public Landmark
{
public:
  Restaurant(string name, int size);
  virtual string icon() const;
  // virtual string color() const {}
  virtual ~Restaurant();

private:
  int m_size;
};

Restaurant::Restaurant(string name, int size)
  : Landmark(name)
{
  if(size < 0)
    exit(1);
  m_size = size;
}

string Restaurant::icon() const
{
  if(m_size < 40)
    return "small knife/fork";
  else
    return "large knife/fork";
}

Restaurant::~Restaurant()
{
  cout << "Destroying the restaurant " << name() <<"." << endl;
}

class Hospital : public Landmark
{
public:
  Hospital(string name);
  virtual string icon() const;
  virtual string color() const;
  virtual ~Hospital();
};

Hospital::Hospital(string name)
  : Landmark(name)
  {}
string Hospital::icon() const
{
  return "H";
}
string Hospital::color() const
{
  return "blue";
}
Hospital::~Hospital()
{
  cout << "Destroying the hospital " << name() <<"." << endl;
}

void display(const Landmark* lm)
{
    cout << "Display a " << lm->color() << " " << lm->icon() << " icon for "
	 << lm->name() << "." << endl;
}

int main()
{
    Landmark* landmarks[4];
    landmarks[0] = new Hotel("Westwood Rest Good");
      // Restaurants have a name and seating capacity.  Restaurants with a
      // capacity under 40 have a small knife/fork icon; those with a capacity
      // 40 or over have a large knife/fork icon.
    landmarks[1] = new Restaurant("Bruin Bite", 30);
    landmarks[2] = new Restaurant("La Morsure de l'Ours", 100);
    landmarks[3] = new Hospital("UCLA Medical Center");

    cout << "Here are the landmarks." << endl;
    for (int k = 0; k < 4; k++)
      display(landmarks[k]);

      // Clean up the landmarks before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
	   delete landmarks[k];
}
