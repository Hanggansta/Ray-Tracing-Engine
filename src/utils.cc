// File: utils.cc
// Date: Wed Apr 10 03:53:11 2013 +0800
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#include "utils.hh"

string TERM_COLOR(int k) {
	// k = 0 ~ 4
	ostringstream ss;
	ss << "\x1b[3" << k + 2 << "m";
	return ss.str();
}

void c_printf(const char* col, const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	printf("%s", col);
	vprintf(fmt, ap);
	printf(COLOR_RESET);
	va_end(ap);
}

void c_fprintf(const char* col, FILE* fp, const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	fprintf(fp, "%s", col);
	vfprintf(fp, fmt, ap);
	fprintf(fp, COLOR_RESET);
	va_end(ap);
}


HWTimer::HWTimer()
{ gettimeofday(&m_start, nullptr); }

double HWTimer::get_sec() const
{
	timeval tv;
	gettimeofday(&tv, nullptr);
	double ret = tv.tv_sec - m_start.tv_sec +
		1e-6*(tv.tv_usec - m_start.tv_usec);
	if (ret < 0)
		ret += 24 * 3600;
	return ret;
}
