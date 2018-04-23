#pragma once
#include <ostream>

template<typename T>
class KSS {
    size_t tp_, tn_, fp_, fn_;
    const T yes_;

public:
    KSS(const T yes) :
        tp_{},
        tn_{},
        fp_{},
        fn_{},
        yes_{ yes } {
    }
    void operator ()(T predicted, T actual) {
        if (predicted == yes_ && actual == yes_) {
            ++tp_;
        }
        else if (predicted != yes_ && actual != yes_) {
            ++tn_;
        }
        else if (predicted == yes_ && actual != yes_) {
            ++fp_;
        }
        else if (predicted != yes_ && actual == yes_) {
            ++fn_;
        }
    }
    double accuracy() const {
		if ((tp_ + tn_ + fp_ + fn_) != 0)
			return double(tp_ + tn_) / double(tp_ + tn_ + fp_ + fn_);
		else
			return 0;
    }
    double precision() const {
		if ((tp_ + fp_) != 0)
        return double(tp_) / double(tp_ + fp_);
		else return 0;
    }
    double TNR() const {
			if ((tn_ + fp_) != 0)
        return double(tn_) / double(tn_ + fp_);
		else return 0;
    }
    double TPR() const {
		if((tp_ + fn_) != 0)
        return double(tp_) / double(tp_ + fn_);
		else return 0;
    }
};


template<typename T> std::ostream& operator << (std::ostream& os, const KSS<T>& kss) {
    os << kss.accuracy() << ';' << kss.precision() << ';' << kss.TNR() << ';' << kss.TPR();
    return os;
}