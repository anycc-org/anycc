#ifndef ANYCC_SUBSTRINGINFO_H
#define ANYCC_SUBSTRINGINFO_H

struct SubstringInfo {
    int start;
    int end;

    bool operator==(const SubstringInfo &rhs) const {
        return start == rhs.start &&
               end == rhs.end;
    }
};

inline bool compareSubstringInfo(const SubstringInfo &a, const SubstringInfo &b) {
    return a.start < b.start;
}

#endif //ANYCC_SUBSTRINGINFO_H
