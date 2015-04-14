#pragma once
#ifndef NDEBUG
  #define assert(s) if (s) {} else while(1);
#else
  #define assert(s) {}
#endif
