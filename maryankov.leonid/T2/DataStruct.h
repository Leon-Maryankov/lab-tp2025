#pragma once
#ifndef DATA_STRUCT
#define DATA_STRUCT

#include<iostream>
#include<string>
#include<iterator>
#include<vector>
#include<iomanip>
#include<complex>
#include<algorithm>
#include<cctype>

namespace nspace {
   struct DataStruct {
       unsigned long long key1 = 0;
       std::complex<double> key2;
       std::string key3;
   };

   struct DelimiterIO {
       char exp;
   };

   struct ComplexIO {
       std::complex<double>& ref;
   };

   struct UnsLongLongIO {
       unsigned long long& ref;
   };

   struct StringIO {
       std::string& ref;
   };

   bool comparator(const DataStruct& left, const DataStruct& right);

   class iofmtguard {
   public:
       iofmtguard(std::basic_ios<char>& s);
       ~iofmtguard();
   private:
       std::basic_ios<char>& s_;
       std::streamsize width_;
       char fill_;
       std::streamsize precision_;
       std::basic_ios<char>::fmtflags fmt_;
   };

   std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
   std::istream& operator>>(std::istream& in, UnsLongLongIO&& dest);
   std::istream& operator>>(std::istream& in, StringIO&& dest);
   std::istream& operator>>(std::istream& in, ComplexIO&& dest);
   std::istream& operator>>(std::istream& in, DataStruct& dest);
   std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}

#endif
