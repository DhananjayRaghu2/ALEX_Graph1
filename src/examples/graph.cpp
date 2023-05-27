// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

/*
 * This short sample program demonstrates ALEX's API
 */

#include "../core/alex.h"
#include "gapbs/wtime.h"
#include "gapbs/pvector.h"
#include "gapbs/platform_atomics.h"
#include <bits/stdc++.h>

#include <iostream>
#include <map>
#include <getopt.h>
using namespace std;

#define SANITY_TEST 0

#define ANALYSIS_PR 0
#define ANALYSIS_SSSP 1

#define VERTEX_TYPE int32_t
#define KEY_TYPE uint64_t
#define WEIGHT_TYPE int32_t
#define TIMESTAMP_TYPE int64_t
#define PAYLOAD_TYPE int32_t
#define DEGREE_TYPE int32_t
//#define PAYLOAD_TYPE pair<TIMESTAMP_TYPE, WEIGHT_TYPE>

#define PUT_KEY(src, dst)         (((KEY_TYPE) src << 32) | dst)
#define GET_SRC(key)              ((VERTEX_TYPE) (key >> 32))   //taking from msb
#define GET_DST(key)              ((VERTEX_TYPE) (key))         //taking from lsb

int THD_COUNT = 0;
VERTEX_TYPE global_vcount = 0;
TIMESTAMP_TYPE global_time = 0;
int64_t num_edges = 0;
//int64_t num_edges = 0;
int source = 0;//text
int job; // 0 means PageRank, 1 means SSSP
string BASE_FILENAME, DYNAMIC_FILENAME;
int64_t num_base_edges, num_dynamic_edges;

DEGREE_TYPE* out_degree;
alex::Alex<KEY_TYPE, PAYLOAD_TYPE> alex_graph;

#ifdef SANITY_TEST
vector<vector<int32_t>> sanity_graph;
#endif

void print_usage()
{
  std::string help = "./exe options.\n";
  help += " --help -h: This message.\n";
  help += " --vcount -v: Vertex count\n";
  help += " --i0 -b: first input file\n";
  help += " --bn -x: number of entries in the first input file\n";
  help += " --i1 -d: second input file\n";
  help += " --dn -y: number of entries in the second input file\n";
  help += " --job -j: job number. Default: 0\n";
  help += " --threadcount --t: Thread count. Default: Cores in your system - 1\n";
  help += " --source  -s: Source vertex-id for graph analysis. Default: 0\n";

  cout << help << endl;
}

const struct option longopts[] =
    {
        {"vcount",    required_argument,  0, 'v'},
        {"help",      no_argument,        0, 'h'},
        {"i0",      required_argument,  0, 'b'},
        {"bn",      required_argument,  0, 'x'},
        {"i1",      required_argument,  0, 'd'},
        {"dn",      required_argument,  0, 'y'},
        {"job",       required_argument,  0, 'j'},
        {"threadcount",  required_argument,  0, 't'},
        {"source",  required_argument,  0, 's'},
        {0,			  0,				  0,  0},
    };

inline void parse_cl_parameters(int argc, char* argv[]) {
  int o;
  int index = 0;
  while ((o = getopt_long(argc, argv, "b:d:j:o:t:v:s:x:y:h", longopts, &index)) != -1) {
    switch (o) {
      case 'v':
        sscanf(optarg, "%d", &global_vcount);
        cout << "Global vcount = " << global_vcount << endl;
        break;
      case 'h':
        print_usage();
        exit(0);
        break;
      case 'b':
        BASE_FILENAME = optarg;
        cout << "first input file = " << BASE_FILENAME << endl;
        break;
      case 'x':
        sscanf(optarg, "%lld", &num_base_edges);
        cout << "number of entries in the first input file = " << num_base_edges << endl;
        break;
      case 'd':
        DYNAMIC_FILENAME = optarg;
        cout << "second input file = " << DYNAMIC_FILENAME << endl;
        break;
      case 'y':
        sscanf(optarg, "%lld", &num_dynamic_edges);
        cout << "number of entries in the second input file = " << num_dynamic_edges << endl;
        break;
      case 'j':
        job = atoi(optarg);
        break;
      case 't':
        //Thread thing
        THD_COUNT = atoi(optarg);
        break;
      case 's':
        sscanf(optarg, "%d", &source);
        break;
      default:
        cout << "invalid input " << endl;
        print_usage();
        exit(0);
    }
  }
}

void init_graph_metadata() {
  out_degree = (DEGREE_TYPE*) calloc(global_vcount, sizeof(DEGREE_TYPE));
  global_time = 0;
  num_edges = 0;
#ifdef SANITY_TEST
  sanity_graph.resize(global_vcount);
#endif
}

/***********************************************************************************************/
/**                              Graph Loading                                                **/
/***********************************************************************************************/

void load_base_graph() {
  double start, end;
  int32_t u, v;
  int32_t w;

  std::ifstream base_file(BASE_FILENAME);
  if (!base_file.is_open()) {
    std::cout << "Couldn't open file " << BASE_FILENAME << std::endl;
    std::exit(-2);
  }

  std::pair<KEY_TYPE, PAYLOAD_TYPE>* base_values;
  base_values = (std::pair<KEY_TYPE, PAYLOAD_TYPE>*) malloc(num_base_edges * sizeof(std::pair<KEY_TYPE, PAYLOAD_TYPE>));
  std::vector<std::pair<KEY_TYPE,PAYLOAD_TYPE> > vec_base_values;
  int64_t t = 0;
  vec_base_values.resize(num_base_edges);
  while (base_file >> u >> v >> w) {
    base_values[t].first = PUT_KEY(u, v);
    base_values[t].second = w;
//    vec_base_values[t].first = PUT_KEY(u, v);
//    vec_base_values[t].second = w;



//    base_values[t].second = make_pair(num_edges, w);
    t += 1;
    num_edges += 1;
    out_degree[u] += 1;
#ifdef SANITY_TEST
    sanity_graph[u].push_back(v);
#endif
  }
  std::cout << "vec length" << vec_base_values.size() << endl;
  int edges = 0;
    unordered_set<::int64_t> stringSet;

    for(int i = 0; i < vec_base_values.size(); i++){
      auto pos = stringSet.find(vec_base_values[i].first);

      // print element position if it is present in set
      if (pos != stringSet.end())
          cout << "Element found at position : "
               << distance(stringSet.begin(), pos) << endl;
      else
          stringSet.insert(vec_base_values[i].first);
        stringSet.insert(vec_base_values[i].first);


    }

  std::cout << "Count of Base Values" << vec_base_values.size() << endl;
  base_file.close();
  std::cout << "base graph loaded to memory" << std::endl;

  start = mywtime();
  std::cout << "Node count" << alex_graph.nodeCounter;
  alex_graph.bulk_load(base_values, num_edges);
  std::cout << "Node count" << alex_graph.nodeCounter;

  end = mywtime();
  std::cout << "base graph bulk loaded into ALEX" << std::endl;
  cout << "B-graph load time = " << (end - start) << endl;

  free(base_values);

#ifdef SANITY_TEST
  int64_t num_edges_retrieve = 0;
  for (auto it = alex_graph.begin(); it != alex_graph.end(); it++) {
      num_edges_retrieve++;
  }
  if (num_edges_retrieve != num_edges) {
    std::cout << "Error! There should be " << num_edges << " edges in the graph, retrieved " << num_edges_retrieve
              << " edges." << std::endl;
  }
#endif
}

void load_dynamic_graph() {
  double start, end;
  int32_t u, v;
  int32_t w;

  std::ifstream dynamic_file(DYNAMIC_FILENAME);
  if (!dynamic_file.is_open()) {
    std::cout << "Couldn't open file " << DYNAMIC_FILENAME << std::endl;
    std::exit(-2);
  }

  int64_t t = 0;
  std::pair<KEY_TYPE, PAYLOAD_TYPE>* dynamic_values;
  dynamic_values = (std::pair<KEY_TYPE, PAYLOAD_TYPE>*) malloc(num_dynamic_edges * sizeof(std::pair<KEY_TYPE, PAYLOAD_TYPE>));
  std::cout << "Dynamic_values" << sizeof(dynamic_values)/sizeof(dynamic_values[0])<< std::endl;
  while (dynamic_file >> u >> v >> w) {
    dynamic_values[t].first = PUT_KEY(u, v);
    dynamic_values[t].second = w;

//    dynamic_values[t].second = make_pair(num_edges+t, w);
    t += 1;
    out_degree[u] += 1;
#ifdef SANITY_TEST
    sanity_graph[u].push_back(v);
#endif
  }
    std::cout << "Dynamic_vlues" << sizeof(dynamic_values)<< std::endl;

    dynamic_file.close();
  std::cout << "dynamic graph loaded into memory" << std::endl;

  // Insert dynamic edges.
  start = mywtime();
  for (int64_t i = 0; i < num_dynamic_edges; i++) {
    // std::cout << "i: " << i << " " << dynamic_values[i].first << " " << dynamic_values[i].second << std::endl;
//    if(i && i%1000000 == 0) std::cout << "dynamic edge inserted: " << i << std::endl;
    alex_graph.insert(dynamic_values[i].first, dynamic_values[i].second);
    num_edges += 1;
  }
  end = mywtime();
  cout << "D-graph load time = " << (end - start) << endl;

  // inserting a special key for the last vertex
  alex_graph.insert(PUT_KEY(global_vcount, 0), -1);
//  alex_graph.insert(PUT_KEY(global_vcount, 0), make_pair(num_edges, -1));
  num_edges += 1;
  free(dynamic_values);

#ifdef SANITY_TEST
 // std::cout << "num_dynamic_edges: " << num_dynamic_edges << std::endl;
  //std::cout << "dynamic values: " << sizeof(dynamic_values)/sizeof(dynamic_values[0]);
    std::cout << "ynamic_vlues" << sizeof(dynamic_values)<< std::endl;

    int64_t num_edges_retrieve = 0;


    for (auto it = alex_graph.begin(); it != alex_graph.end(); it++) {


        std:: cout << num_edges_retrieve++;
  }
  if (num_edges_retrieve != num_edges) {
      
    std::cout << "Error There should be " << num_edges << " edges in thEEe graph, retrieved " << num_edges_retrieve
              << " edges." << std::endl;
  }
#endif
}

/***********************************************************************************************/
/**                              Sanity Checker                                               **/
/***********************************************************************************************/

void check_sanity() {
  for(int i=0; i<global_vcount; i+=1) {
    assert(out_degree[i] == (int32_t) sanity_graph[i].size() && "Degree mismatched in the sanity graph");
    int j=0;

    for(auto it = alex_graph.lower_bound(PUT_KEY(i, 0)); it != alex_graph.lower_bound(PUT_KEY((i+1), 0)); it++) {
      assert(GET_DST(it.key()) == sanity_graph[i][j] && "Edge value mismatched in the sanity graph");
      j += 1;
    }
  }
  cout << "Congratulation! Sanity check have been completed without any error!" << endl;
}

void access_whole_graph() {
  int64_t weight_sum = 0;
  double start, end;

  start = mywtime();
  for(int i=0; i<global_vcount; i+=1) {
    for(auto it = alex_graph.lower_bound(PUT_KEY(i, 0)); it != alex_graph.lower_bound(PUT_KEY((i+1), 0)); it++) {
      weight_sum += it.payload();
//      weight_sum += it.payload().second;
    }
  }
  end = mywtime();
  cout << "Whole graph access time = " << (end - start) << endl;
}

/***********************************************************************************************/
/**                              GAP Utility Functions                                        **/
/***********************************************************************************************/

// Returns k pairs with largest values from list of key-value pairs
template<typename KeyT, typename ValT>
std::vector<std::pair<ValT, KeyT>> TopK(
    const std::vector<std::pair<KeyT, ValT>> &to_sort, size_t k) {
  std::vector<std::pair<ValT, KeyT>> top_k;
  ValT min_so_far = 0;
  for (auto kvp : to_sort) {
    if ((top_k.size() < k) || (kvp.second > min_so_far)) {
      top_k.push_back(std::make_pair(kvp.second, kvp.first));
      std::sort(top_k.begin(), top_k.end(),
                std::greater<std::pair<ValT, KeyT>>());
      if (top_k.size() > k)
        top_k.resize(k);
      min_so_far = top_k.back().first;
    }
  }
  return top_k;
}

/***********************************************************************************************/
/**                              PageRank Algorithm                                           **/
/***********************************************************************************************/

typedef float ScoreT;
const float kDamp = 0.85;

pvector<ScoreT> run_pr(int max_iters, double epsilon = 0) {
//  pgraph_t<T>* pgraph = (pgraph_t<T>*)get_plaingraph();
//  snap_t<T>* snaph = create_static_view(pgraph, STALE_MASK|V_CENTRIC);

//  mem_pagerank<dst_id_t>(snaph, 20);
//  delete_static_view(snaph);

  const ScoreT init_score = 1.0f / global_vcount;
  const ScoreT base_score = (1.0f - kDamp) / global_vcount;
  pvector<ScoreT> scores(global_vcount, init_score);
  pvector<ScoreT> outgoing_contrib(global_vcount);
  for (int iter=0; iter < max_iters; iter++) {
    double error = 0;
    VERTEX_TYPE v;
#pragma omp parallel for
    for (VERTEX_TYPE n=0; n < global_vcount; n++)
      outgoing_contrib[n] = scores[n] / out_degree[n];
#pragma omp parallel for reduction(+ : error) schedule(dynamic, 64)
    for (VERTEX_TYPE u=0; u < global_vcount; u++) {
      ScoreT incoming_total = 0;
      for (auto it = alex_graph.lower_bound(PUT_KEY(u, 0)); it != alex_graph.lower_bound(PUT_KEY((u+1), 0)); it++) {
        v = GET_DST(it.key());
        incoming_total += outgoing_contrib[v];
      }
      ScoreT old_score = scores[u];
      scores[u] = base_score + kDamp * incoming_total;
      error += fabs(scores[u] - old_score);
    }
//    printf(" %2d    %lf\n", iter, error);
//    if (error < epsilon)
//      break;
  }
  return scores;
}

void PrintTopPRScores(const pvector<ScoreT> &scores) {
  vector<pair<VERTEX_TYPE, ScoreT>> score_pairs(global_vcount);
  for (VERTEX_TYPE n=0; n < global_vcount; n++) {
    score_pairs[n] = make_pair(n, scores[n]);
  }
  int k = 5;
  vector<pair<ScoreT, VERTEX_TYPE>> top_k = TopK(score_pairs, k);
  k = min(k, static_cast<int>(top_k.size()));
  for (auto kvp : top_k)
    cout << kvp.second << ":" << kvp.first << endl;
}

/***********************************************************************************************/
/**                              SSSP Algorithm                                               **/
/***********************************************************************************************/

const WEIGHT_TYPE kDistInf = numeric_limits<WEIGHT_TYPE>::max()/2;
const size_t kMaxBin = numeric_limits<size_t>::max()/2;

pvector<WEIGHT_TYPE> run_sssp(VERTEX_TYPE source, WEIGHT_TYPE delta) {
  pvector<WEIGHT_TYPE> dist(global_vcount, kDistInf);
  dist[source] = 0;
  pvector<VERTEX_TYPE> frontier(num_edges);
  // two element arrays for double buffering curr=iter&1, next=(iter+1)&1
  size_t shared_indexes[2] = {0, kMaxBin};
  size_t frontier_tails[2] = {1, 0};
  frontier[0] = source;
#pragma omp parallel
  {
    vector<vector<VERTEX_TYPE> > local_bins(0);
    size_t iter = 0;
    while (shared_indexes[iter&1] != kMaxBin) {
      size_t &curr_bin_index = shared_indexes[iter&1];
      size_t &next_bin_index = shared_indexes[(iter+1)&1];
      size_t &curr_frontier_tail = frontier_tails[iter&1];
      size_t &next_frontier_tail = frontier_tails[(iter+1)&1];
#pragma omp for nowait schedule(dynamic, 64)
      for (size_t i=0; i < curr_frontier_tail; i++) {
        VERTEX_TYPE u = frontier[i];
        if (dist[u] >= delta * static_cast<WEIGHT_TYPE>(curr_bin_index)) {
//          for (WNode wn : g.out_neigh(u)) {
          for (auto it = alex_graph.lower_bound(PUT_KEY(u, 0)); it != alex_graph.lower_bound(PUT_KEY((u+1), 0)); it++) {
            VERTEX_TYPE dst = GET_DST(it.key());
            WEIGHT_TYPE old_dist = dist[dst];
            WEIGHT_TYPE new_dist = dist[u] + it.payload();
//            WEIGHT_TYPE new_dist = dist[u] + it.payload().second;
            if (new_dist < old_dist) {
              bool changed_dist = true;
              while (!compare_and_swap(dist[dst], old_dist, new_dist)) {
                old_dist = dist[dst];
                if (old_dist <= new_dist) {
                  changed_dist = false;
                  break;
                }
              }
              if (changed_dist) {
                size_t dest_bin = new_dist/delta;
                if (dest_bin >= local_bins.size()) {
                  local_bins.resize(dest_bin+1);
                }
                local_bins[dest_bin].push_back(dst);
              }
            }
          }
        }
      }
      for (size_t i=curr_bin_index; i < local_bins.size(); i++) {
        if (!local_bins[i].empty()) {
#pragma omp critical
          next_bin_index = min(next_bin_index, i);
          break;
        }
      }
#pragma omp barrier
#pragma omp single nowait
      {
//        PrintStep(curr_bin_index, t.Millisecs(), curr_frontier_tail);
        curr_bin_index = kMaxBin;
        curr_frontier_tail = 0;
      }
      if (next_bin_index < local_bins.size()) {
        size_t copy_start = fetch_and_add(next_frontier_tail,
                                          local_bins[next_bin_index].size());
        copy(local_bins[next_bin_index].begin(),
             local_bins[next_bin_index].end(), frontier.data() + copy_start);
        local_bins[next_bin_index].resize(0);
      }
      iter++;
#pragma omp barrier
    }
#pragma omp single
    cout << "took " << iter << " iterations" << endl;
  }
  return dist;
}

void PrintSSSPStats(const pvector<WEIGHT_TYPE> &dist) {
  auto NotInf = [](WEIGHT_TYPE d) { return d != kDistInf; };
  int64_t num_reached = count_if(dist.begin(), dist.end(), NotInf);
  cout << "SSSP Tree reaches " << num_reached << " nodes" << endl;
}

/***********************************************************************************************/
/**                              Main Function                                                **/
/***********************************************************************************************/

int main(int argc, char* argv[]) {
  parse_cl_parameters(argc, argv);
  init_graph_metadata();

  load_base_graph();
  load_dynamic_graph();

#ifdef SANITY_TEST
  access_whole_graph();
//  check_sanity();
#endif

  // Look at some stats
  auto stats = alex_graph.get_stats();
  std::cout << "Final num edges: " << stats.num_keys << std::endl;
  std::cout << "Num inserts: " << stats.num_inserts << std::endl;

  double start, end;
  if(job == ANALYSIS_PR) {
    start = mywtime();
    pvector<ScoreT> pr_ret = run_pr(20);
    end = mywtime();
    cout << "PageRank time = " << (end - start) << endl;
    PrintTopPRScores(pr_ret);
  }

  if(job == ANALYSIS_SSSP) {
    start = mywtime();
    pvector<WEIGHT_TYPE> sssp_ret = run_sssp(source, 1);
    end = mywtime();
    cout << "SSSP time = " << (end - start) << endl;
    PrintSSSPStats(sssp_ret);
  }
}