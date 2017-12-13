/**************************************************************************
 *
 * Copyright 2017 Intel Corporation
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Authors:
 *   Mark Janes <mark.a.janes@intel.com>
 **************************************************************************/

#include <assert.h>
#include <map>
#include <vector>

#include "glframe_mesh_optimize.hpp"

typedef uint32_t Vertex;

class Edge {
 public:
  Edge(const Vertex &a, const Vertex &b)
      : m_a(a < b ? a : b),
        m_b(a < b ? b : a) {
    assert(a != b);
  }
  bool operator<(const Edge &o) const {
    if (m_a < o.m_a)
      return true;
    if (m_a > o.m_a)
      return false;
    return m_b < o.m_b;
  }
  bool operator==(const Edge &o) const {
    return m_a == o.m_a && m_b == o.m_b;
  }
  bool operator!=(const Edge &o) const {
    return !((*this) == o);
  }

 private:
  Vertex m_a, m_b;
};

typedef std::vector<Edge> EdgeVec;
class Triangle {
 public:
  explicit Triangle(Vertex _a, Vertex _b, Vertex _c)
      : a(_a), b(_b), c(_c) {}
  void edges(EdgeVec *e) {
    e->push_back(Edge(a, b));
    e->push_back(Edge(b, c));
    e->push_back(Edge(c, a));
  }
  bool operator==(const Triangle &o) const {
    return a == o.a && b == o.b && c == o.c;
  }
  void vertices(std::vector<Vertex> *v) {
    v->push_back(a);
    v->push_back(b);
    v->push_back(c);
  }

 private:
  Vertex a, b, c;
};

typedef std::map<Edge, std::vector<Triangle>> EdgeMap;
typedef std::vector<Triangle> TriVec;

void
reduce_edge_map(const Edge &e, EdgeMap *edge_map,
                TriVec *triangles) {
  EdgeVec unfollowed_edges;
  Edge current_edge = e;
  while (true) {
    const auto edge_list = edge_map->find(current_edge);
    if (edge_list == edge_map->end()) {
      if (unfollowed_edges.empty())
        break;
      current_edge = unfollowed_edges.back();
      unfollowed_edges.pop_back();
      continue;
    }
    for (auto tri : edge_list->second) {
      triangles->push_back(tri);
      EdgeVec new_edges;
      tri.edges(&new_edges);
      for (auto new_edge : new_edges) {
        if (new_edge != current_edge) {
          auto duplicate_entry = edge_map->find(new_edge);
          if (duplicate_entry != edge_map->end()) {
            for (auto dup_t = duplicate_entry->second.rbegin();
                 dup_t != duplicate_entry->second.rend(); ++dup_t) {
              if (*dup_t == tri) {
                *dup_t = duplicate_entry->second.back();
                duplicate_entry->second.pop_back();
              }
            }
          }
          unfollowed_edges.push_back(new_edge);
        }
      }
    }
    assert(edge_map->find(edge_list->first) != edge_map->end());
    edge_map->erase(edge_list);
    if (unfollowed_edges.empty())
      break;
    current_edge = unfollowed_edges.back();
    unfollowed_edges.pop_back();
  }
}

void
retrace::optimize_mesh(std::vector<uint32_t> *vertices) {
  EdgeMap edge_map;
  auto i = vertices->begin();
  while (i != vertices->end()) {
    Vertex a=*i++, b = *i++, c=*i++;
    Triangle t(a, b, c);
    std::vector<Edge> edges;
    t.edges(&edges);
    for (const auto &e : edges) {
      auto &tri_list = edge_map[e];
      bool seen = false;
      for (auto known_t : tri_list) {
        if (t == known_t) {
          seen = true;
          break;
        }
      }
      if (seen)
        continue;
      tri_list.push_back(t);
    }
  }
  TriVec triangles;
  while (!edge_map.empty()) {
    auto f = edge_map.begin();
    if (f->second.empty()) {
      assert(false);
      edge_map.erase(f);
      continue;
    }
    reduce_edge_map(f->first, &edge_map, &triangles);
  }
  vertices->clear();
  for (auto t : triangles)
    t.vertices(vertices);
}

