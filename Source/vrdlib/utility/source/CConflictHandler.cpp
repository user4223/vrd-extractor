
#include "../include/CConflictHandler.h"

#include <iostream>
#include <sstream>
#include <map>

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/case_conv.hpp>

namespace VRD
{
namespace Utility
{   
   int AConflictHandlerBase::validate(int inValue, int maxValue) 
   {
      if (inValue < 0l)
      {  
         std::ostringstream os;
         os << "Inserted value '" << inValue << "' less than 0";
         throw std::out_of_range(os.str()); 
      }
      
      if (inValue > maxValue)
      {  
         std::ostringstream os;
         os << "Inserted value '" << inValue << "' greater than maximum: " << maxValue;
         throw std::out_of_range(os.str()); 
      }
      return inValue;
   }
   
   CManualConflictHandler::CManualConflictHandler(std::istream& is, std::ostream& os)
      :m_is(is)
      ,m_os(os)
   {}
   
   CManualConflictHandler::Result CManualConflictHandler::handle(std::vector<std::string> const& _options)
   {
      if (_options.empty())
      {  throw std::invalid_argument("Empty option list"); }
      
      std::map<std::string, std::string> options;
      for (unsigned int i(0); i < _options.size(); ++i)
      {  options.emplace(std::make_pair(std::to_string(i), _options[i])); }
      options.emplace(std::make_pair("x", "Abort further processing"));
      
      while (1)
      {
         if (!m_is.good())
         {  throw std::domain_error("Input stream failed"); }
         
         m_os << "Select one option:\n";
         for (auto const& option : options) 
         {  m_os << "   " << option.first << ": " << option.second << '\n'; }
         
         std::string input;
         m_is >> input;
         boost::algorithm::trim(input);
         boost::algorithm::to_lower(input);
         
         if (input.size() == 0)
         {
            m_os << "Input error: Empty string\n";
            continue;
         }
         
         if (input.size() == 1 && input[0] == 'x')
         {  
            /** \todo This should be an move specific exeption to enable users to catch exactly this case 
             */
            throw std::domain_error("Abort requested by user, terminate processing here"); 
         }
         
         if (!((input[0] >= '0' && input[0] <= '9') || input[0] == '-' || input[0] == '+'))
         {
            m_os << "Input error: Expected an integral number or 'x' for abort, got: " << input << "\n";
            continue;
         }
         
         int value(0);
         try
         {
            value = std::stoi(input);
            value = validate(value, static_cast<int>(_options.size()-1));
         }
         catch (std::invalid_argument const&)
         {
            m_os << "Input error: Expected an integral number or 'x' for abort, got: " << input << "\n";
            continue;
         }
         catch (std::out_of_range const& e)
         {
            m_os << "Input error: " << e.what() << "\n";
            continue;
         }
         
         return Result(value);
      }
   }
   
   CCustomConflictHandler::CCustomConflictHandler(FunctionType f)
      :m_f(f)
   {}
   
   CCustomConflictHandler::Result CCustomConflictHandler::handle(std::vector<std::string> const& options)
   {  return Result(validate(m_f(options), static_cast<int>(options.size()-1))); }
}}
