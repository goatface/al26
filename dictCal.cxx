// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dictCal

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
#include "Calibration.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *Calibration_Dictionary();
   static void Calibration_TClassManip(TClass*);
   static void *new_Calibration(void *p = 0);
   static void *newArray_Calibration(Long_t size, void *p);
   static void delete_Calibration(void *p);
   static void deleteArray_Calibration(void *p);
   static void destruct_Calibration(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Calibration*)
   {
      ::Calibration *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Calibration));
      static ::ROOT::TGenericClassInfo 
         instance("Calibration", "Calibration.h", 40,
                  typeid(::Calibration), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Calibration_Dictionary, isa_proxy, 0,
                  sizeof(::Calibration) );
      instance.SetNew(&new_Calibration);
      instance.SetNewArray(&newArray_Calibration);
      instance.SetDelete(&delete_Calibration);
      instance.SetDeleteArray(&deleteArray_Calibration);
      instance.SetDestructor(&destruct_Calibration);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Calibration*)
   {
      return GenerateInitInstanceLocal((::Calibration*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::Calibration*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Calibration_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Calibration*)0x0)->GetClass();
      Calibration_TClassManip(theClass);
   return theClass;
   }

   static void Calibration_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_Calibration(void *p) {
      return  p ? new(p) ::Calibration : new ::Calibration;
   }
   static void *newArray_Calibration(Long_t nElements, void *p) {
      return p ? new(p) ::Calibration[nElements] : new ::Calibration[nElements];
   }
   // Wrapper around operator delete
   static void delete_Calibration(void *p) {
      delete ((::Calibration*)p);
   }
   static void deleteArray_Calibration(void *p) {
      delete [] ((::Calibration*)p);
   }
   static void destruct_Calibration(void *p) {
      typedef ::Calibration current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Calibration

namespace {
  void TriggerDictionaryInitialization_dictCal_Impl() {
    static const char* headers[] = {
"Calibration.h",
0
    };
    static const char* includePaths[] = {
"/Users/Shimizu/local/root/include",
"/Users/Shimizu/local/root/include",
"/local/crabat/al26/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "dictCal dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$Calibration.h")))  Calibration;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "dictCal dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Calibration.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Calibration", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("dictCal",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_dictCal_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_dictCal_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_dictCal() {
  TriggerDictionaryInitialization_dictCal_Impl();
}
