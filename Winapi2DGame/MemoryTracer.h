#pragma once

void* operator new(size_t size, const char* file, int line);
void* operator new[](size_t size, const char* file, int line);
void  operator delete(void* p, const char* file, int line);
void  operator delete[](void* p, const char* file, int line);

#define new new( __FILE__, __LINE__)


