// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dictAnaly

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "Analyzer.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *Analyzer_Dictionary();
   static void Analyzer_TClassManip(TClass*);
   static void *new_Analyzer(void *p = 0);
   static void *newArray_Analyzer(Long_t size, void *p);
   static void delete_Analyzer(void *p);
   static void deleteArray_Analyzer(void *p);
   static void destruct_Analyzer(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Analyzer*)
   {
      ::Analyzer *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Analyzer));
      static ::ROOT::TGenericClassInfo 
         instance("Analyzer", "Analyzer.h", 32,
                  typeid(::Analyzer), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Analyzer_Dictionary, isa_proxy, 0,
                  sizeof(::Analyzer) );
      instance.SetNew(&new_Analyzer);
      instance.SetNewArray(&newArray_Analyzer);
      instance.SetDelete(&delete_Analyzer);
      instance.SetDeleteArray(&deleteArray_Analyzer);
      instance.SetDestructor(&destruct_Analyzer);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Analyzer*)
   {
      return GenerateInitInstanceLocal((::Analyzer*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Analyzer*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Analyzer_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Analyzer*)0x0)->GetClass();
      Analyzer_TClassManip(theClass);
   return theClass;
   }

   static void Analyzer_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_Analyzer(void *p) {
      return  p ? new(p) ::Analyzer : new ::Analyzer;
   }
   static void *newArray_Analyzer(Long_t nElements, void *p) {
      return p ? new(p) ::Analyzer[nElements] : new ::Analyzer[nElements];
   }
   // Wrapper around operator delete
   static void delete_Analyzer(void *p) {
      delete ((::Analyzer*)p);
   }
   static void deleteArray_Analyzer(void *p) {
      delete [] ((::Analyzer*)p);
   }
   static void destruct_Analyzer(void *p) {
      typedef ::Analyzer current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Analyzer

namespace {
  void TriggerDictionaryInitialization_dictAnaly_Impl() {
    static const char* headers[] = {
"Analyzer.h",
0
    };
    static const char* includePaths[] = {
"/Users/Shimizu/local/root/include",
"/Users/Shimizu/local/root/include",
"/local/crabat/al26/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "dictAnaly dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$Analyzer.h")))  Analyzer;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "dictAnaly dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Analyzer.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Analyzer", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("dictAnaly",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dictAnaly_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dictAnaly_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dictAnaly() {
  TriggerDictionaryInitialization_dictAnaly_Impl();
}
