
#include "vrdlib/common/include/CExtractor.h"

#include "vrdlib/utility/include/LoggerUtility.h"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <iostream>

namespace bfs = boost::filesystem;
namespace bpo = boost::program_options;

int main(int argc, char** argv)
{
   auto const executablePath(bfs::path(argv[0]).parent_path());
   
   bpo::options_description desc("Options");
   desc.add_options()
      ("help,h"
         ,"Print this help message")
      ("root,r"
         ,bpo::value<bfs::path>()->default_value(bfs::current_path())
         ,"Root directory path")
      ("white-list-regex, w"
         ,bpo::value<std::string>()->default_value(".*[.]CR(2|W)$")
         ,"White list regex to filter directory entries")
      ("dry,d"
         ,bpo::value<bool>()->default_value(true)
         ,"Dry mode, do not update any file")
      ("log-file,l"
         ,bpo::value<bfs::path>()->default_value(bfs::current_path() / "vrdextractor.log")
         ,"Path to log-file")
      ("logger-config-file"
         ,bpo::value<bfs::path>()->default_value(executablePath / "etc" / "loggerConfig.xml")
         ,"Path to logger configuration file")
      ;
   
   try
   {
      bpo::variables_map vm;
      bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
      bpo::notify(vm);

      if (vm.count("help"))
      {
         std::cout << desc << '\n';
         return 0;
      }
      
      VRD::Utility::initializeLogger(
          vm["logger-config-file"].as<bfs::path>()
         ,vm["log-file"].as<bfs::path>());
      VRD::CExtractor(
          vm["root"].as<bfs::path>()
         ,vm["white-list-regex"].as<std::string>()
         ,vm["dry"].as<bool>());
      return 0;
   }
   catch (std::exception const& e)
   {
      std::cerr << "Error: " << e.what() << '\n';
      return -1;
   }
}
