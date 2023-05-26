// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

/*
 * This short sample program demonstrates ALEX's API.
 */

#include "../core/alex.h"
#include "../examples/gapbs/wtime.h"
#include "../examples/gapbs/pvector.h"
#include "../examples/gapbs/platform_atomics.h"

#include <getopt.h>
using namespace std;

#define SANITY_TEST 0

#define ANALYSIS_PR 0
#define ANALYSIS_SSSP 1

#define VERTEX_TYPE int32_t
#define KEY_TYPE uint64_t
#define WEIGHT_TYPE int32_t
#define TIMESTAMP_TYPE int64_t
#define PAYLOAD_TYPE WEIGHT_TYPE
#define DEGREE_TYPE int32_t
//#define PAYLOAD_TYPE pair<TIMESTAMP_TYPE, WEIGHT_TYPE>

#define PUT_KEY(src, dst)         (((KEY_TYPE) src << 32) | dst)
#define GET_SRC(key)              ((VERTEX_TYPE) (key >> 32))   //taking from msb
#define GET_DST(key)              ((VERTEX_TYPE) (key))         //taking from lsb

int THD_COUNT = 0;
VERTEX_TYPE global_vcount = 0;
TIMESTAMP_TYPE global_time = 0;
int64_t num_edges = 0;
int source = 0;//text
int job; // 0 means PageRank, 1 means SSSP
string BASE_FILENAME, DYNAMIC_FILENAME, OUTPUT_FILENAME;
int64_t num_base_edges, num_dynamic_edges;
std::ofstream output_file;

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
  help += " --i2 -o: output file\n";
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
        {"i2",      required_argument,  0, 'o'},
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
      case 'o':
        OUTPUT_FILENAME = optarg;
        cout << "output file = " << OUTPUT_FILENAME << endl;
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

/// amazon graph
// taskset --cpu-list 0-70:2 ./build/el2kv -b /mnt/cci-files/dataset-mutablecsr/amazon0601_10-90/amazon0601.base.wel -x 488681 -d /mnt/cci-files/dataset-mutablecsr/amazon0601_10-90/amazon0601.dynamic.wel -y 4398135 -v 403394 -j 0 -s 0 -t 1

/// cit-patent graph (edge count mismatch)
// taskset --cpu-list 0-70:2 ./build/el2kv -b /mnt/cci-files/dataset-mutablecsr/cit-Patents_10-90/cit-Patents.base.wel -x 3303789 -d /mnt/cci-files/dataset-mutablecsr/cit-Patents_10-90/cit-Patents.dynamic.wel -y 29734105 -v 6009555 -j 0 -s 1 -t 1

/// live-journal graph (stuck in base graph building)
// taskset --cpu-list 0-70:2 ./build/el2kv -b /mnt/cci-files/dataset-mutablecsr/live-journal_10-90/soc-LiveJournal1.base.wel -x 8570247 -d /mnt/cci-files/dataset-mutablecsr/live-journal_10-90/soc-LiveJournal1.dynamic.wel -y 77132227 -v 4847571 -j 0 -s 0 -t 1

/// as-skitter graph (edge count mismatch) (segfault after graph insertion)
// taskset --cpu-list 0-70:2 ./build/el2kv -b /mnt/cci-files/dataset-mutablecsr/skitter_10-90/as-skitter.base.wel -x 2219060 -d /mnt/cci-files/dataset-mutablecsr/skitter_10-90/as-skitter.dynamic.wel -y 19971536 -v 1696415 -j 0 -s 0 -t 1

/// road graph (segfault during dynamic insertion)
// taskset --cpu-list 0-70:2 ./build/el2kv -b /mnt/cci-files/dataset-mutablecsr/roadNet_10-90/roadNet-CA.base.wel -x 553321 -d /mnt/cci-files/dataset-mutablecsr/roadNet_10-90/roadNet-CA.dynamic.wel -y 4979893 -v 1971281 -j 0 -s 0 -t 1

/// orkut graph (stuck in base graph building)
// taskset --cpu-list 0-70:2 ./build/el2kv -b /mnt/cci-files/dataset-mutablecsr/orkut_10-90/com-orkut.ungraph.base.wel -x 23437016 -d /mnt/cci-files/dataset-mutablecsr/orkut_10-90/com-orkut.ungraph.dynamic.wel -y 210933150 -v 3072627 -j 0 -s 1 -t 1

/// stackoverflow graph (edge count mismatch)
// taskset --cpu-list 0-70:2 ./build/el2kv -b /mnt/cci-files/dataset-mutablecsr/stackoverflow_10-90/sx-unique-undir.base.wel -x 5772481 -d /mnt/cci-files/dataset-mutablecsr/stackoverflow_10-90/sx-unique-undir.dynamic.wel -y 51952321 -v 6024271 -j 0 -s 1 -t 1

/// enron graph
// taskset --cpu-list 0-70:2 ./build/el2kv -b /mnt/cci-files/dataset-mutablecsr/enron_10-90/enron-unique-undir.base.wel -x 59492 -d /mnt/cci-files/dataset-mutablecsr/enron_10-90/enron-unique-undir.dynamic.wel -y 535420 -v 87274 -j 0 -s 1 -t 1

/// mathoverflow graph (segfault during dynamic insertion)
// taskset --cpu-list 0-70:2 ./build/el2kv -b /mnt/cci-files/dataset-mutablecsr/mathoverflow_10-90/sx-mathoverflow-unique-undir.base.wel -x 37598 -d /mnt/cci-files/dataset-mutablecsr/mathoverflow_10-90/sx-mathoverflow-unique-undir.dynamic.wel -y 338374 -v 88581 -j 0 -s 1 -t 1

/// facebook-wall graph
// taskset --cpu-list 0-70:2 ./build/el2kv -b /mnt/cci-files/dataset-mutablecsr/fbwall_10-90/facebook-wall-unique-undir.base.wel -x 36683 -d /mnt/cci-files/dataset-mutablecsr/fbwall_10-90/facebook-wall-unique-undir.dynamic.wel -y 330141 -v 63892 -j 0 -s 1 -t 1

void init_graph_metadata() {
  out_degree = (DEGREE_TYPE*) calloc(global_vcount, sizeof(DEGREE_TYPE));
  global_time = 0;
  num_edges = 0;
#ifdef SANITY_TEST
  sanity_graph.resize(global_vcount);
#endif
  output_file.open(OUTPUT_FILENAME);
  if (!output_file.is_open()) {
    std::cout << "Couldn't open file " << BASE_FILENAME << std::endl;
    std::exit(-2);
  }
}

/***********************************************************************************************/
/**                              Graph Loading                                                **/
/***********************************************************************************************/

void load_base_graph() {
  int32_t u, v;
  int32_t w;

  std::ifstream base_file(BASE_FILENAME);
  if (!base_file.is_open()) {
    std::cout << "Couldn't open file " << BASE_FILENAME << std::endl;
    std::exit(-2);
  }

  while (base_file >> u >> v >> w) {
    output_file << PUT_KEY(u, v) << "\n";
  }
  base_file.close();
  std::cout << "base graph written to file" << std::endl;
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

  while (dynamic_file >> u >> v >> w) {
    output_file << PUT_KEY(u, v) << "\n";
  }
  dynamic_file.close();
  std::cout << "dynamic graph written to file" << std::endl;

  // inserting a special key for the last vertex
  output_file << PUT_KEY(global_vcount, 0) << "\n";
}

/***********************************************************************************************/
/**                              Main Function                                                **/
/***********************************************************************************************/

int main(int argc, char* argv[]) {
  parse_cl_parameters(argc, argv);
  init_graph_metadata();

  load_base_graph();
  load_dynamic_graph();
  output_file.close();
}
