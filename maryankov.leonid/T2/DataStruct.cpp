#include "DataStruct.h"

namespace nspace {
   bool comparator(const DataStruct& left, const DataStruct& right) {
       if (left.key1 != right.key1) {
           return left.key1 < right.key1;
       }
       double left_ = std::abs(left.key2);
       double right_ = std::abs(right.key2);
       if (left_ != right_) {
           return left_ < right_;
       }
       return left.key3.length() < right.key3.length();
   }

   std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
       std::istream::sentry sentry(in);
       if (!sentry) {
           return in;
       }
       char ch;
       in >> ch;
       if (in && (ch != dest.exp)) {
           in.setstate(std::ios::failbit);
       }
       return in;
   }

   std::istream& operator>>(std::istream& in, UnsLongLongIO&& dest) {
       std::istream::sentry sentry(in);
       if (!sentry) {
           return in;
       }
       std::string hexNum;
       char c;
       while (in.get(c) && c != ':') {
           hexNum += c;
       }
       if (c == ':') {
           in.putback(c);
       }

       if (hexNum.size() < 3 || hexNum[0] != '0' || (toupper(hexNum[1]) != 'X')) {
           in.setstate(std::ios::failbit);
           return in;
       }

       for (size_t i = 2; i < hexNum.size(); ++i) {
           if (!isxdigit(hexNum[i])) {
               in.setstate(std::ios::failbit);
               return in;
           }
       }

       dest.ref = std::stoull(hexNum, nullptr, 16);
       return in;
   }

   std::istream& operator>>(std::istream& in, StringIO&& dest) {
       std::istream::sentry sentry(in);
       if (!sentry) {
           return in;
       }
       return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
   }

   std::istream& operator>>(std::istream& in, ComplexIO&& dest) {
       std::istream::sentry sentry(in);
       if (!sentry) {
           return in;
       }

       double real, imag;
       if (!(in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' } &&
           in >> real >> imag >> DelimiterIO{ ')' })) {
           in.setstate(std::ios::failbit);
           return in;
       }

       dest.ref = std::complex<double>(real, imag);
       return in;
   }

   std::istream& operator>>(std::istream& in, DataStruct& dest) {
       std::istream::sentry sentry(in);
       if (!sentry) {
           return in;
       }
       DataStruct input;
       bool key1 = false;
       bool key2 = false;
       bool key3 = false;

       using sep = DelimiterIO;
       using ull = UnsLongLongIO;
       using str = StringIO;
       using cmpl = ComplexIO;

       in >> sep{ '(' };
       while (!(key1 && key2 && key3)) {
           std::string temp;
           char ch;

           in >> ch;
           if (!in) {
               break;
           }

           if (ch == ':' && (in >> temp)) {
               if (temp == "key1") {
                   in >> ull{ input.key1 };
                   key1 = true;
               }
               else if (temp == "key2") {
                   in >> cmpl{ input.key2 };
                   key2 = true;
               }
               else if (temp == "key3") {
                   in >> str{ input.key3 };
                   key3 = true;
               }
           }
       }
       in >> sep{ ':' } >> sep{ ')' };

       if (in) {
           dest = input;
       }
       return in;
   }

   std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
       std::ostream::sentry sentry(out);
       if (!sentry) {
           return out;
       }
       iofmtguard fmtguard(out);

       out << "(:";
       out << "key1 0x" << std::hex << std::uppercase << src.key1 << ":";
       out << "key2 #c(" << std::fixed << std::setprecision(1);
       out << src.key2.real() << " " << src.key2.imag() << "):";
       out << "key3 \"" << src.key3 << "\"";
       out << ":)";

       return out;
   }

   iofmtguard::iofmtguard(std::basic_ios<char>& s) :
       s_(s),
       width_(s.width()),
       fill_(s.fill()),
       precision_(s.precision()),
       fmt_(s.flags())
   {
   }

   iofmtguard::~iofmtguard()
   {
       s_.width(width_);
       s_.fill(fill_);
       s_.precision(precision_);
       s_.flags(fmt_);
   }
}
