// ETH AlgoLab example code: Compute a Euclidean minimum spanning tree (EMST)
// for n points p_0,...,p_{n-1} in O(n log n) time. Output the edges as ordered
// pairs of vertex indices (smaller first) together with the squared length; for
// instance, an edge between p_4=(0,0) and p_2=(1,2) is printed as "2 4 5".

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <queue>
// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::FT                                                  Info;
typedef CGAL::Triangulation_vertex_base_2<K>                   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Info,K>      Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef Delaunay::Face_handle Face;
// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef K::Point_2 Point;
const K::FT INF(std::numeric_limits<double>::max());

void test_case() {
  int n, m; K::FT r; std::cin >> n >> m >> r;

  std::vector<Point> trees;
  trees.reserve(n);
  for (int i = 0; i < n; ++i) {
    long x, y; std::cin >> x >> y;
    trees.push_back(Point(x,y));
  }

  Delaunay t;
  t.insert(trees.begin(), trees.end());
  std::queue<Face> Q;
  
  for(auto it = t.all_faces_begin(); it != t.all_faces_end(); ++it) {
    if (!t.is_infinite(it)) {
      it->info() = CGAL::squared_radius(it->vertex(0)->point(),
					it->vertex(1)->point(),
					it->vertex(2)->point());
    } else {
      it->info() = INF;
    }
    Q.push(it);
  }

  while(!Q.empty()) {
    auto u = Q.front(); Q.pop();

    for (int i = 0; i < 3; ++i) {
      auto v = u->neighbor(i);
      K::FT d = CGAL::squared_distance(u->vertex((i+1)%3)->point(),
				       u->vertex((i+2)%3)->point());
      K::FT clearance = std::min(d, u->info());
      if (clearance > v->info()) {
	v->info() = clearance;
	Q.push(v);
      }
    }
  }

  for (int i = 0; i < m; ++i) {
    long x, y; K::FT s; std::cin >> x >> y >> s;
    Point b(x,y);
    Face f = t.locate(b);
	
    if (CGAL::squared_distance(t.nearest_vertex(b,f)->point(), b) < CGAL::square(r+s)) {
      std::cout << 'n';
      continue;
    }
    if (f->info() >= CGAL::square(2*(r+s))) std::cout << 'y';
    else std::cout << 'n';
    }

  std::cout<< std::endl;
}



int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) test_case();
}


