// By: Zhiping

#include <L3.h>
#include <parser.h>

int main(int argc, char **argv) {
  bool verbose;

  /* Check the input */
  if( argc < 2 ) {
  std::cerr << "Usage: " << argv[ 0 ] << " SOURCE [-v]" << std::endl;
    return 1;
  }
  int32_t opt;
  while ((opt = getopt(argc, argv, "v")) != -1) {
    switch (opt) {
      case 'v':
        verbose = true;
        break;

      default:
        std::cerr << "Usage: " << argv[ 0 ] << "[-v] SOURCE" << std::endl;
        return 1;
    }
  }

  std::ofstream outputFile;
  outputFile.open("prog.L2");

  L3::Program p = L3::L3_parse_file(argv[optind]);

  outputFile << "(:main\n";
  for (auto f : p.functions) {
    outputFile << "\t(" << f->name << "\n";
    outputFile << "\t\t" << std::to_string(f->arguments.size()) << " 0\n";

    for (auto i : f->instructions) {
      std::cout << "hello\n";
      outputFile << i->toString();
    }
    outputFile << "\n\t)\n";
  }

  outputFile << ")\n";
  outputFile.close();
}
