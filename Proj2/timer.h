#include <Windows.h>

const __int64 DELTA_EPOCH_IN_MICROSECS = 11644473600000000;

struct timezone2 {
	__int32 tz_minuteswest;
	bool tz_dsttime;
};
struct timeval2 {
	__int32 tv_sec; /* seconds */
	__int32 tv_usec; /* microseconds */
};

int gettimeofday(struct timeval2* tv, struct timezone2* tz) {
	FILETIME ft;
	__int64 tmpres = 0;
	TIME_ZONE_INFORMATION tz_winapi; int rez = 0;
	ZeroMemory(&ft, sizeof(ft));
	ZeroMemory(&tz_winapi, sizeof(tz_winapi));
	GetSystemTimeAsFileTime(&ft);
	tmpres = ft.dwHighDateTime;
	tmpres <<= 32;
	tmpres |= ft.dwLowDateTime;
	tmpres /= 10;
	tmpres -= DELTA_EPOCH_IN_MICROSECS;
	tv->tv_sec = (__int32)(tmpres * 0.000001);
	tv->tv_usec = (tmpres % 1000000);
	rez = GetTimeZoneInformation(&tz_winapi);
	tz->tz_dsttime = (rez == 2) ? true : false;
	tz->tz_minuteswest = tz_winapi.Bias + ((rez == 2) ? tz_winapi.DaylightBias : 0);
	return 0;
}
double dtime() {
	double tseconds = 0.0;
	struct timeval2 mytime;
	struct timezone2 myzone;
	gettimeofday(&mytime, &myzone);
	tseconds = (double)(mytime.tv_sec + mytime.tv_usec * 1.0e-6);
	return tseconds;
}