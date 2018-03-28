#pragma once
#include <ostream>

template<typename T>
class KSS {
    size_t tp_, tn_, fp_, fn_;
    const T yes_;
    const T no_;

public:
    KSS(const T yes, const T no) :
        tp_{},
        tn_{},
        fp_{},
        fn_{},
        yes_{ yes },
        no_{ no } {
    }
    void operator ()(T predicted, T actual) {
        if (predicted == yes_ && actual == yes_) {
            ++tp_;
        }
        else if (predicted == no_ && actual == no_) {
            ++tn_;
        }
        else if (predicted == yes_ && actual == no_) {
            ++fp_;
        }
        else if (predicted == no_ && actual == yes_) {
            ++fn_;
        }
    }
    double accuracy() const {
        return double(tp_ + tn_) / double(tp_ + tn_ + fp_ + fn_);
    }
    double precision() const {
        return double(tp_) / double(tp_ + fp_);
    }
    double TNR() const {
        return double(tn_) / double(tn_ + fp_);
    }
    double TPR() const {
        return double(tp_) / double(tp_ + fn_);
    }
};


template<typename T> std::ostream& operator << (std::ostream& os, const KSS<T>& kss) {
    os << kss.accuracy() << ';' << kss.precision() << ';' << kss.TNR() << ';' << kss.TPR();
    return os;
}