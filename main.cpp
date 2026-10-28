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
      
      // C1: p1 = 0
      rel(*this, p[1], IRT_EQ, 0);
      // C2: p_{i-1} <= p_i
      for (int i = 2; i <= n; i++) rel(*this, p[i - 1], IRT_LQ, p[i]);
      // C3: x_j < y_j неактуально для орграфа
      // C4: x_j <= x_{j+1}
      for (int j = 0; j + 1 < m; j++) rel(*this, x[j], IRT_LQ, x[j + 1]);
   
      // C9: (x_j, y_j) <lex (x_{j+1}, y_{j+1})
      for (int j = 0; j + 1 < m; j++) {
		  IntVarArgs a, b;
          a << x[j] << y[j];
          b << x[j + 1] << y[j + 1];
          rel(*this, a, IRT_LE, b);  
		}
      //ветвление по приципу берем неиниц переменную и начинаем с минимального значения домена
	  IntVarArgs tree;
      for (int i = 1; i < n; i++) tree << p[i];
      branch(*this, tree, INT_VAR_NONE(), INT_VAL_MIN()); 
      IntVarArgs back;
      for (int j = 0; j < m - (n-1); j++) back << x[j] << y[j];
      branch(*this, back, INT_VAR_NONE(), INT_VAL_MIN());
		  
	 }
	  
	BFSModel(BFSModel& s) : Space(s), n(s.n), m(s.m)
	{
        p.update(*this, s.p);
        x.update(*this, s.x);
        y.update(*this, s.y);
    }
    
    virtual Space* copy() { return new BFSModel(*this); }
	
	void print() const {
        for (int i = 1; i < n; i++) std::cout << p[i].val();
        for (int j = 0; j < x.size(); j++) std::cout << "," << x[j].val() << y[j].val();
        std::cout << "\n";
    }	
	
};
	
int main(){
	int n, m;
	BFSModel* root = new BFSModel(n, m);
    DFS<BFSModel> e(root);
    delete root;

    int count = 0;
    while (BFSModel* s = e.next()) {
        count++;
        s->print();  
    }
    std::cout << "всего решений: " << count << "\n";
	
	return 0;
}
