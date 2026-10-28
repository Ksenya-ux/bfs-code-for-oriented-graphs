#include <gecode/int.hh>
#include <gecode/search.hh>

using namespace Gecode;

class BFSModel : public Space
{
	
protected:
	int n, m; // кол-во вершин и ребер
	IntVarArray p; // родители
	IntVarArray x,y; // список обратных ребер из x в y
	
public:
	BFSModel(int n_, int m_)
	: n(n_), m(m_),
	  p(*this, n_ - 1, 0, n_ - 1),                  
      x(*this, m_ - (n_ - 1), 1, n_ - 1),        
      y(*this, m_ - (n_ - 1), 1, n_ - 1) 
      {

	  rel(*this, p[0], IRT_EQ, 0);
      for (int i = 1; i < n; i++) rel(*this, p[i], IRT_LQ, i - 1);
      
      //ветвление по приципу берем неиниц переменную и начинаем с минимального значения домена
	  IntVarArgs tree;
      for (int i = 1; i < n; i++) tree << p[i];
      branch(*this, tree, INT_VAR_NONE(), INT_VAL_MIN()); 
      IntVarArgs back;
      for (int j = 0; j < nb; j++) back << x[j] << y[j];
      branch(*this, back, INT_VAR_NONE(), INT_VAL_MIN());
		  
	 }
	  
	BFSModel(BFSModel& s) : Space(s), n(s.n), m(s.m)
	{
        p.update(*this, s.p);
        x.update(*this, s.x);
        y.update(*this, s.y);
    }
    
    virtual Space* copy() { return new BFSModel(*this); }
	
	
	
	
};
	
int main(){
	
	
	return 0;
}
