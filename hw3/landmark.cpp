class Landmark
{
public:
  Landmark(string name);
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
