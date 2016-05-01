#include "ParseOptionsBase.h"
#include "Exception.h"
#include "LogSystem.h"
#include "Option.h"

#include <iostream>


using namespace std;
namespace po = boost::program_options;

namespace ak {

   ParseOptionsBase::ParseOptionsBase()
   :  bpo_desc_( "Available Options" ),
      bpo_positional_(),
      bpo_vm_(),
      added_options_()
   {
   }

   ParseOptionsBase::~ParseOptionsBase()
   {
   }

   void ParseOptionsBase::add_group( string const & title, OptionsList const & v )
   {
      po::options_description d( title.c_str() );

      for( auto i : v ) {

         added_options_.insert( OptionsMap::value_type( i->registered_id(), i ) );

         if( i->storage_type() )
            d.add_options()( i->cmdline_id().c_str(), i->storage_type(), i->description().c_str() );
         else
            d.add_options()( i->cmdline_id().c_str(), i->description().c_str() );
      }

      bpo_desc_.add( d );
   }

   // void ParseOptionsBase::add_positional( string const & i, int num_ocurrences )
   // {
   //    auto it = option_invokers_.find( i );
   //    if( it != option_invokers_.end() ) {
   //       //cout << "added option " << i << " as positional" << endl;
	//  bpo_positional_.add( i.c_str(), num_ocurrences );
   //    }
   // }
   //
   // void ParseOptionsBase::add_positional( std::vector< std::string > const & v )
   // {
   //    for( auto i : v ) 
   //       add_positional( i );
   // }

   bool ParseOptionsBase::has_entered_option( int registered_id ) const
   {
      auto it = added_options_.find( registered_id );
      if( it == added_options_.end() )
         return false;

      return bpo_vm_.count( it->second->name() );
   } 

   bool ParseOptionsBase::has_entered_option( std::string const & op_name ) const
   {
      return bpo_vm_.count( op_name );
   } 

   bool ParseOptionsBase::no_user_option( ) const
   {
      return bpo_vm_.empty();
   } 

   void ParseOptionsBase::print_usage( std::string const & title ) const
   {	
      //cout << title << "  usage " << endl;
      cout << bpo_desc_ << endl;
   }
      
   Option * ParseOptionsBase::option_ptr( int registered_id ) const
   {
      auto it = added_options_.find( registered_id );
      if ( it == added_options_.end() )
         throw ak_exception( "no registered_id to get its option_ptr" );

      return it->second;
   }

   //void ParseOptionsBase::process_command_line( int argc, char** argv )
   void ParseOptionsBase::process_command_line_arguments( int argc, char** argv )
   {
      try {
         // could throw
         po::store(  po::command_line_parser( argc, argv )
                     .options( bpo_desc_ ).positional( bpo_positional_ ).run(),
                     bpo_vm_ ); 

         // // just in case correct usage check if user asks for help
         // if( bpo_vm_.count("help" ) || bpo_vm_.empty() ) {
      	 //    // trigger user callback if it is possible
	 //    auto it = option_invokers_.find( "help" );
	 //    if ( it != option_invokers_.end() && it->second.first ) {
	 //       it->second.first();
	 //       exit(0);
	 //    }
         // }
         
         // throws on error
         po::notify( bpo_vm_ ); 
      } 
      catch( po::required_option & e ) { 
         string s = "Error! Not found some of the required options.\nPlease run again using --help to show available options.";
         s += "\n\n" + string( e.what() ) + " \n"; 
         throw ak_options_exception( s );
      } 
      catch( po::error & e ) { 
         string s = "Error! Something wrong. \nPlease run again using --help to show available options";
         s += "\n\n " + string( e.what() ) + " \n"; 
         throw ak_options_exception( s );
      } 
      catch( exception & e ) { 
         throw ak_options_exception( "Oops! something wrong with STL doing program options\n\n" + string( e.what() ) + " \n" );
      } 
   }

}


