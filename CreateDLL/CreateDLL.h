#pragma once

extern "C" __declspec(dllexport) int __stdcall test01(int a, int b, int c);
extern "C" __declspec(dllexport) int __stdcall test02(int a, int b);
extern "C" __declspec(dllexport) int __stdcall test04(char* file_name);
extern "C" __declspec(dllexport) char* __stdcall get_license_str();