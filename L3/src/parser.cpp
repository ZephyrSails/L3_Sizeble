// by: Zhiping

// #include <L3.h>
#include <pegtl.hh>
#include <pegtl/analyze.hh>
#include <pegtl/contrib/raw_string.hh>
#include <parser.h>

using namespace pegtl;
using namespace std;

namespace L3 {
  // meta element
  struct comment:
    pegtl::disable<
      pegtl::one< ';' >,
      pegtl::until< pegtl::eolf >
    > {};

  struct seps:
    pegtl::star<
      pegtl::sor<
        pegtl::ascii::space,
        comment
      >
    > {};

  struct op:
    pegtl::sor<
      pegtl::string< '+' >,
      pegtl::string< '-' >,
      pegtl::string< '*' >,
      pegtl::string< '&' >,
      pegtl::string< '<', '<' >,
      pegtl::string< '>', '>' >
    > {};

  struct cmp:
    pegtl::sor<
      pegtl::string< '<', '=' >,
      pegtl::string< '<' >,
      pegtl::string< '=' >,
      pegtl::string< '>', '=' >,
      pegtl::string< '>' >
    > {};

  struct runtime_system_func:
    pegtl::sor<
      pegtl::string< 'p', 'r', 'i', 'n', 't' >,
      pegtl::string< 'a', 'l', 'l', 'o', 'c', 'a', 't', 'e' >,
      pegtl::string< 'a', 'r', 'r', 'a', 'y', '-', 'e', 'r', 'r', 'o', 'r' >
    > {};

  // struct call:
  //   pegtl::string < 'c', 'a', 'l', 'l' > {};

  struct left_arrow:
    pegtl::string< '<', '-' > {};


  struct L3_var:
    pegtl::seq<
      pegtl::plus<
        pegtl::sor<
          pegtl::alpha,
          pegtl::one< '_' >
        >
      >,
      pegtl::star<
        pegtl::sor<
          pegtl::alpha,
          pegtl::one< '_' >,
          pegtl::digit
        >
      >
    > {};

  struct L3_Num:
    pegtl::seq<
      pegtl::opt<
        pegtl::sor<
          pegtl::one< '-' >,
          pegtl::one< '+' >
        >
      >,
      pegtl::plus<
        pegtl::digit
      >
    > {};

  struct L3_t:
    pegtl::sor<
      L3_var,
      L3_Num
    > {};

  struct L3_label:
    pegtl::seq<
      pegtl::one<':'>,
      L3_var
    > {};

  struct L3_u:
    pegtl::sor<
      L3_var,
      L3_label
    > {};


  struct L3_callee:
    pegtl::sor<
      L3_u,
      runtime_system_func
    > {};

  struct var:
    L3_var {};

  struct Num:
    L3_Num {};

  struct t:
    L3_t {};

  struct label:
    L3_label {};

  struct u:
    L3_u {};

  struct callee:
    L3_callee {};

  // struct

  struct vars:
    pegtl::rep_max< 1,
      pegtl::seq<
        var,
        seps,
        pegtl::star<
          pegtl::seq<
            pegtl::one< ',' >,
            seps,
            var,
            seps
          >
        >
      >
    > {};

  struct args:
    pegtl::seq<
      t,
      seps,
      pegtl::star<
        pegtl::seq<
          pegtl::one< ',' >,
          seps,
          t,
          seps
        >
      >
    > {};

  struct call:
    pegtl::string< 'c', 'a', 'l', 'l' > {};

  struct L3_ins_call:
    pegtl::seq<
      call,
      seps, callee, seps,
      pegtl::one< '(' >, seps, args, seps, pegtl::one< ')' >
    > {};

  struct load:
    pegtl::string< 'l', 'o', 'a', 'd' > {};

  struct ins_v_start:
    pegtl::seq<
      var, seps, left_arrow, seps,
      pegtl::sor<
        label,
        pegtl::seq<
          load, seps, var
        >,
        L3_ins_call,
        pegtl::seq<
          t,
          pegtl::rep_max< 1,
            pegtl::seq< seps, pegtl::sor< op, cmp >, seps, t>
          >
        >
      >
    > {};

  struct store:
    pegtl::string< 's', 't', 'o', 'r', 'e' > {};

  struct ins_store:
    pegtl::seq<
      store,
      seps,
      var,
      seps,
      left_arrow,
      seps,
      pegtl::sor< label, t >
    > {};

  struct ins_br_start:
    pegtl::seq<
      pegtl::string< 'b', 'r' >,
      seps,
      pegtl::sor<
        label,
        pegtl::seq< var, seps, label, seps, label >
      >
    > {};

  struct ins_label:
    L3_label {};

  struct ins_return:
    pegtl::sor<
      pegtl::seq<
        pegtl::string< 'r', 'e', 't', 'u', 'r', 'n' >,
        seps,
        pegtl::sor< Num, var >
      >,
      pegtl::string< 'r', 'e', 't', 'u', 'r', 'n' >
    > {};

  struct ins_call:
    L3_ins_call {};

  struct L3_instruction:
    pegtl::sor<
      ins_return,
      ins_store,
      ins_call,
      ins_br_start,
      ins_label,
      ins_v_start
    > {};

  struct function_name:
    L3_label {};

  struct L3_function:
    pegtl::seq<
      pegtl::string< 'd', 'e', 'f', 'i', 'n', 'e' >,
      seps, function_name, seps,
      pegtl::one< '(' >, seps, vars, seps, pegtl::one< ')' >,
      seps,
      pegtl::one< '{' >,
      seps,
      pegtl::star<
        L3_instruction,
        seps
      >,
      seps,
      pegtl::one< '}' >
    > {};

  struct entry_point_rule:
    pegtl::seq<
      seps,
      pegtl::plus<
        L3_function,
        seps
      >
    > {};

  struct L3_grammer:
    pegtl::must< entry_point_rule > {};

  /*
   * Helpers
   */


  /*
   * Actions attached to grammar rules.
   */
  template< typename Rule >
  struct action:
    pegtl::nothing< Rule > {};

  template<> struct action < function_name > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      L3::Function *newF = new L3::Function();
      std::string token = in.string();
      newF->name = token;
      for (int k = 0; k < v.size(); k++) {
        newF->arguments.push_back(new L3::Var(v[k]));
      }

      p.functions.push_back(newF);
      v.clear();
    }
  };


  // Instructions builder

  template<> struct action < ins_v_start > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      L3::Function *currentF = p.functions.back();
      L3::Instance *newIns = new L3::Var(v[0]);
      if (v[1] == "call") {
        std::vector<std::string> cv (v.begin()+1, v.end());
        newIns->instances.push_back(new L3::Call(cv));
      } else if (v[1] == "load") {
        newIns->instances.push_back(new L3::Load(v));
      } else if (v.size() == 4) { // cmp || op
        newIns->instances.push_back(new L3::Op(v));
      } else { // var <- s
        newIns->instances.push_back(new L3::Var(v[1]));
      }

      currentF->instructions.push_back(newIns);
      v.clear();
    }
  };

  template<> struct action < ins_store > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      L3::Function *currentF = p.functions.back();

      L3::Instance *newIns = new L3::Store(v);

      currentF->instructions.push_back(newIns);
      v.clear();
    }
  };

  template<> struct action < ins_br_start > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      L3::Function *currentF = p.functions.back();

      L3::Instance *newIns = new L3::Br(v);

      currentF->instructions.push_back(newIns);

      v.clear();
    }
  };

  template<> struct action < ins_label > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      L3::Function *currentF = p.functions.back();

      L3::Instance *newIns = new L3::Var(v[0]);

      currentF->instructions.push_back(newIns);
      v.clear();
    }
  };

  template<> struct action < ins_return > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      L3::Function *currentF = p.functions.back();

      L3::Instance *newIns = new L3::Return(v);

      currentF->instructions.push_back(newIns);

      v.clear();
    }
  };

  template<> struct action < ins_call > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      L3::Function *currentF = p.functions.back();

      currentF->instructions.push_back(new L3::Call(v));

      v.clear();
    }
  };


  //
  // Actions to collect string from rules, should be a better way.
  //
  template<> struct action < var > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      v.push_back(in.string());
    }
  };

  template<> struct action < Num > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      v.push_back(in.string());
    }
  };

  template<> struct action < t > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      v.push_back(in.string());
    }
  };

  template<> struct action < label > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      v.push_back(in.string());
    }
  };

  template<> struct action < u > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      v.push_back(in.string());
    }
  };

  template<> struct action < callee > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      v.push_back(in.string());
    }
  };

  template<> struct action < op > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      v.push_back(in.string());
    }
  };

  template<> struct action < cmp > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      v.push_back(in.string());
    }
  };

  template<> struct action < load > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      v.push_back(in.string());
    }
  };

  template<> struct action < call > {
    static void apply( const pegtl::input & in, L3::Program & p, std::vector<std::string> & v ) {
      v.push_back(in.string());
    }
  };



  /*
   * Data structures required to parse
   */
  Program L3_parse_file (char *fileName) {
    /*
     * Check the grammar for some possible issues.
     */
    pegtl::analyze< L3::L3_grammer >();
    /*
     * Parse.
     */
    L3::Program p;
    // L3::Instruction ti; // temp instruction
    std::vector< std::string > v;
    pegtl::file_parser(fileName).parse< L3::L3_grammer, L3::action > (p, v);

    return p;
  }
}
