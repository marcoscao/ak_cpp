#ifndef AK_CORE_OPTION_DEF_H__
#define AK_CORE_OPTION_DEF_H__

#include "Option.h"
#include "Factory.h"
#include "ParseOptionsBase.h"
#include "Visitor.h"

#include <memory>
#include <string>

/*
 * core Option sub-macros used by the final below AK_... macros to help define classes
 */



//using OptionStorageType = boost::value_semanticssss;
//
//
//class base_class = Option
//
//template<typename base_class, int ID>
//class t_option : public base_class {
//   static const int ID = a_ID; \
//   static bool is_registered; \
//   std::string cmdline_id() const override { return CMDLINE_ID; } \
//   std::string name() const override { return NAME; } \
//   std::string description() const override { return DESC; } \
//   /*virtual void execute( ParseOptionsBase const & pob ) { ; } */  \
//   void accept( Visitor & v ) override { v.visit( *this ); } \
//   \
//private: \
//   static bool register_option() \
//   { \
//      get_options_factory().register_item( CLASS_NAME::ID, \
//            std::make_unique< factory_item_creator<CLASS_NAME> >() ); \
//      return true; \
//   }
//	static const int id = ID;
//	std::string cmdline_id();
//	std::string name();
//	ak::OptionStorageType storage_type();
//};
//
//
//#define __AK_T_OPTION_METHOD_CMDLINE__( STR ) \
//	std::string cmdline_id() override { return STR; }
//
//#define __AK_
//
//
//
//using SourcesOp = t_option< Option, 200 >;
//
//
//#define __jj_declare_op_class( ID, CLASS_NAME)\
//	using CLASS_NAME = t_option< Option, ID >;
//
//#define __jj_define_op_class( CLASS_NAME ) \
//
//
//
//




/*
 * submacro: Starting Common option class code
 */
#define __AK_START_DEFINE_OPTION_CLASS__( a_ID, CLASS_NAME, CMDLINE_ID, NAME, DESC ) \
class CLASS_NAME : public Option { \
   template<typename> friend class factory; \
   friend std::unique_ptr<CLASS_NAME> std::make_unique<CLASS_NAME>(); \
   \
public: \
   \
   static const int ID = a_ID; \
   static bool is_registered; \
   /*static std::unique_ptr<Option> create() { return std::make_unique<CLASS_NAME>(); } */  \
   std::string cmdline_id() const override { return CMDLINE_ID; } \
   std::string name() const override { return NAME; } \
   std::string description() const override { return DESC; } \
   /*virtual void execute( ParseOptionsBase const & pob ) { ; } */  \
   void accept( Visitor & v ) override { v.visit( *this ); } \
   \
private: \
   static bool register_option() \
   { \
      get_options_factory().register_item( CLASS_NAME::ID, \
            std::make_unique< factory_item_creator<CLASS_NAME> >() ); \
      return true; \
   }
//private:\
   //static option_creator_impl< CLASS_NAME > creator_;

   

/*
 * submacro: end common option class code
 */
#define __AK_END_DEFINE_OPTION_CLASS__( CLASS_NAME ) \
private: \
   /*CLASS_NAME() { set_registered_id( ID ); }*/ \
   CLASS_NAME() { ; }\
};


/*
 * submacro: end option class holding variable
 */
#define __AK_END_DEFINE_OPTION_CLASS_WITH_VAR__( CLASS_NAME ) \
public: \
   STORAGE_DATA & get_data() { return storage_data_; } \
   const STORAGE_DATA & get_data() const { return storage_data_; } \
private: \
   STORAGE_DATA storage_data_; \
   /*CLASS_NAME() : storage_data_() { set_registered_id( ID ); } */ \
   CLASS_NAME() : storage_data_() { ; } \
}; 


/*
 * submacro: register option
 */
#define __AK_REGISTER_OPTION_CLASS__( CLASS_NAME ) \
   bool CLASS_NAME::is_registered = CLASS_NAME::register_option();

//option_creator_impl< CLASS_NAME > CLASS_NAME::creator_= option_creator_impl<CLASS_NAME>( ID ); \
// Factory<Option>::instance().register_item< CLASS_NAME >( ID ); 


//#define AK_REGISTER_OPTION_CLASS( CLASS_NAME ) \
//   get_options_factory().register_item( CLASS_NAME::REGISTERED_ID, \
//         std::make_unique< factory_item_creator<CLASS_NAME> >() ); } \




/*
 * Below usage final macros
 * -------------------------
 */

/*
 * Final Macro: defines an Option Class without vars
 */
#define AK_DEFINE_OPTION( CLASS_NAME, ID, CMDLINE_ID, NAME, DESC ) \
   __AK_START_DEFINE_OPTION_CLASS__( ID, CLASS_NAME, CMDLINE_ID, NAME, DESC ) \
   __AK_END_DEFINE_OPTION_CLASS__( CLASS_NAME ) \
   //__AK_REGISTER_OPTION_CLASS__( CLASS_NAME, ID )


/*
 * Final Macro: defines an Option class with the storage of a unique value
 */
#define AK_DEFINE_OPTION_WITH_UNIQUE_VALUE( CLASS_NAME, ID, CMDLINE_ID, NAME, DESC, VAR_TYPE, VAR_DEFAULT ) \
   __AK_START_DEFINE_OPTION_CLASS__( ID, CLASS_NAME, CMDLINE_ID, NAME, DESC ) \
   public: \
      \
      using VALUE_TYPE = VAR_TYPE; \
      using STORAGE_DATA = VALUE_TYPE; \
      /*using STORAGE_FN = ParseOptionsBase::set_unique< VALUE_TYPE >; */\
      \
      virtual ParseOptionsBase::StorageType * storage_type() override \
      { \
         return ParseOptionsBase::set_unique< VALUE_TYPE >( &storage_data_, VAR_DEFAULT ); \
      } \
      \
   __AK_END_DEFINE_OPTION_CLASS_WITH_VAR__( CLASS_NAME ) \
   //__AK_REGISTER_OPTION_CLASS__( CLASS_NAME, ID )


/*
 * Final Macro: defines an Option with multiple values
 */
#define AK_DEFINE_OPTION_WITH_MULTIPLE_VALUES( CLASS_NAME, ID, CMDLINE_ID, NAME, DESC, VAR_TYPE ) \
   __AK_START_DEFINE_OPTION_CLASS__( ID, CLASS_NAME, CMDLINE_ID, NAME, DESC ) \
   public: \
   \
      using VALUE_TYPE = VAR_TYPE; \
      using STORAGE_DATA = std::vector< VALUE_TYPE >; \
      /*using STORAGE_FN = ParseOptionsBase::set_multiple< VALUE_TYPE >;*/ \
      \
      virtual ParseOptionsBase::StorageType * storage_type() override \
      { \
         return ParseOptionsBase::set_multiple< VALUE_TYPE >( &storage_data_ ); \
      } \
      \
   __AK_END_DEFINE_OPTION_CLASS_WITH_VAR__( CLASS_NAME ) \
   //__AK_REGISTER_OPTION_CLASS__( CLASS_NAME, ID )


/*
 * Final Macro: defines an Option with multiple values
 */
#define AK_DEFINE_REQUIRED_OPTION_WITH_MULTIPLE_VALUES( CLASS_NAME, ID, CMDLINE_ID, NAME, DESC, VAR_TYPE ) \
   __AK_START_DEFINE_OPTION_CLASS__( ID, CLASS_NAME, CMDLINE_ID, NAME, DESC ) \
   public: \
   \
      using VALUE_TYPE = VAR_TYPE; \
      using STORAGE_DATA = std::vector< VALUE_TYPE >; \
      /*using STORAGE_FN = ParseOptionsBase::set_multiple< VALUE_TYPE >;*/ \
      \
      virtual ParseOptionsBase::StorageType * storage_type() override \
      { \
         return ParseOptionsBase::set_multiple_required< VALUE_TYPE >( &storage_data_ ); \
      } \
      \
   __AK_END_DEFINE_OPTION_CLASS_WITH_VAR__( CLASS_NAME ) \
   //__AK_REGISTER_OPTION_CLASS__( CLASS_NAME, ID )




#endif  // end Option.h





