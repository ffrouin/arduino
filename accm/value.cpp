#include "basics.h"
#include "value.h"

value::value() {
	reset();
}

value &value::operator+=(float _v) {
	if (count) {
		if (_v<mmin) {
			if (avg_mm) {
				mmin+=_v;
				mmin/=2;
			} else {
				mmin=_v;
			}
		}
		if (_v>mmax) {
			if (avg_mm) {
				mmax+=_v;
				mmax/=2;
			} else {
				mmax=_v;
			}
		}
		sum+=_v;
	} else {
		if (_v<mmin)
			mmin=_v;
		if (_v>mmax)
			mmax=_v;
		sum=_v;
	}
	count++;
	last=_v;
	return(*this);
}

value &value::operator*=(float _v) {
	mmin *= _v;
	mmax *= _v;
	last *= _v;
	sum *= _v;
	return(*this);
}

value &value::operator/=(float _v) {
	mmin /= _v;
	mmax /= _v;
	last /= _v;
	sum /= _v;
	return(*this);
}

void value::reset(void) {
	mmin = DEFAULT_INIT_VALUE;
	mmax = -1*DEFAULT_INIT_VALUE;
	last = DEFAULT_INIT_VALUE;
	sum = DEFAULT_INIT_VALUE;
	count = 0;
}
