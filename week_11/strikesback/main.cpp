#include <iostream>

//LP Stuff
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

//Triangulation stuff
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Point_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 Point;
// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


#define trace(x) //std::cout << #x << " = " << x << std::endl
#define step(x) //std::cout << "step: " << x << std::endl
void test_case() {
  step(1);
  int a, s, b, e; std::cin >> a>> s >> b >> e;
  
  std::vector<std::pair<Point, int>> asteroids;
  for (int i = 0; i < a; ++i) {
    int x, y, d; std::cin >> x >> y >> d;
    asteroids.push_back({Point(x,y), d});
  }

  std::vector<Point> shots;
  shots.reserve(s);
  for (int i = 0; i < s; ++i) {
    int x, y; std::cin >> x >> y;
    shots.push_back(Point(x,y));
  }
  std::vector<Point> hunters;
  hunters.reserve(b);
  for (int i = 0; i < b; ++i) {
    int x, y; std::cin >> x >> y;
    hunters.push_back(Point(x,y));
  }

  step(2);

  Triangulation t;
  t.insert(hunters.begin(), hunters.end());

  std::vector<std::vector<int>> hits(a);
  for (int i = 0; i < s; ++i) {
    if (b == 0) {
      for (int j = 0; j < a; ++j) {
	hits[j].push_back(i);
      }
      continue;
    }
    Point closest = t.nearest_vertex(shots[i])->point();
    trace(i);
    for (int j = 0; j < a; ++j) {
      trace(j);
      if (CGAL::has_smaller_distance_to_point(shots[i],asteroids[j].first, closest)) {
	hits[j].push_back(i);
      }
    }
  }

  step(3);
  Program lp (CGAL::LARGER, true, 0, false, 0); 
   
  for (int i = 0; i < s; ++i) {
    lp.set_c(i, 1);
  }
  
  for (int i = 0; i < a; ++i) {
    for (int j : hits[i]) {
      IT mult = CGAL::squared_distance(asteroids[i].first, shots[j]);
      if (mult < 1) mult = 1;
      lp.set_a(j, i, 1.0/mult);
    }
    lp.set_b(i, asteroids[i].second);
  }

  step(4);
  Solution sol = CGAL::solve_linear_program(lp, ET());
  if (sol.objective_value() > e || sol.is_infeasible()) std::cout << "n\n";
  else std::cout << "y\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while(t--) test_case();

}
