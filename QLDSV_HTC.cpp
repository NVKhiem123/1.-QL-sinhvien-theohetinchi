/*
 * QUAN LY DIEM SINH VIEN THEO HE TIN CHI
 * File : QLDSV_HTC.cpp
 * Cau truc du lieu dua tren: Khaibao_QLDSV_HTC.cpp
 *
 * a. Mo lop tin chi (Them/Xoa/Sua)
 * b. In DSSV da dang ky lop tin chi
 * c. Nhap/Cap nhat SV cua lop (Them/Xoa/Sua, DSSV sap xep tang dan theo TEN)
 * d. In DSSV cua lop (tang dan MASV)
 * e. Quan ly mon hoc (Them/Xoa/Sua, BST theo MAMH)
 * f. In DS mon hoc (tang dan TENMH)
 * g. Dang ky lop tin chi
 * h. Huy lop tin chi tu dong (so SV dang ky < min)
 * i. Nhap diem
 * j. In bang diem mon hoc
 * k. In bang diem trung binh khoa hoc (theo tin chi)
 * l. Bang diem tong ket (diem cao nhat tung mon)
 * Luu/Doc du lieu tu file
 */

#include <bits/stdc++.h>
using namespace std;

const int MAXLOP = 10000;
const int MAXLTC = 10000;

// ============================================================
// CAU TRUC DU LIEU
// ============================================================

struct Monhoc {
    string MAMH, TENMH;
    int STC_LT, STC_TH;
};
struct nodeMH {
    Monhoc mh;
    nodeMH *left, *right;
};
typedef nodeMH *treeMH;

struct Dangky {
    string MASV;
    float DIEM = -1.0f;
    bool HUYDK = false;
};
struct nodeDK {
    Dangky dk;
    nodeDK *next;
};
typedef nodeDK *PTRDK;

struct Loptinchi {
    int MALOPTC;
    string MAMH, NIENKHOA;
    int HOCKY, NHOM, SOSVMIN, SOSVMAX;
    bool HUYLOP = false;
    PTRDK dssvdk = NULL;
};
struct DS_LTC {
    Loptinchi *nodes[MAXLTC];
    int n = 0;
};

struct Sinhvien {
    string MASV, HO, TEN, PHAI, SODT;
};
struct nodeSV {
    Sinhvien sv;
    nodeSV *next;
};
typedef nodeSV *PTRSV;

struct Lop {
    string MALOP, TENLOP;
    PTRSV dssv = NULL;
};
struct DS_LOP {
    Lop nodes[MAXLOP];
    int n = 0;
};

int gMaLopTC = 0;

// ============================================================
// TIEN ICH
// ============================================================

string trim(const string &s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == string::npos) return "";
    return s.substr(b, s.find_last_not_of(" \t\r\n") - b + 1);
}
void printLine(int n = 70, char c = '-') { cout << string(n, c) << "\n"; }

string inputStr(const string &p) {
    string s; cout << p; getline(cin, s); return trim(s);
}
int inputInt(const string &p, int lo = INT_MIN, int hi = INT_MAX) {
    int v;
    for (;;) {
        cout << p;
        if (cin >> v) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (v >= lo && v <= hi) return v;
            cout << "  [!] Nhap lai (tu " << lo << " den " << hi << "): ";
        } else {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Khong hop le, nhap lai: ";
        }
    }
}
float inputFloat(const string &p, float lo = 0.0f, float hi = 10.0f) {
    float v;
    for (;;) {
        cout << p;
        if (cin >> v) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (v >= lo && v <= hi) return v;
            cout << "  [!] Nhap lai (tu " << lo << " den " << hi << "): ";
        } else {
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [!] Khong hop le, nhap lai: ";
        }
    }
}
int inputIntOpt(const string &p, int cur, int lo, int hi) {
    string s; cout << p << " [" << cur << "]: "; getline(cin, s); s = trim(s);
    if (s.empty()) return cur;
    try { int v = stoi(s); if (v < lo || v > hi) { cout << "  [!] Ngoai tam, giu nguyen.\n"; return cur; } return v; }
    catch (...) { cout << "  [!] Khong hop le, giu nguyen.\n"; return cur; }
}
string inputStrOpt(const string &p, const string &cur) {
    cout << p << " [" << cur << "]: "; string s; getline(cin, s); s = trim(s);
    return s.empty() ? cur : s;
}
string fmtFloat(float v) {
    if (v < 0) return "---";
    ostringstream o; o << fixed << setprecision(2) << v; return o.str();
}

// ============================================================
// BST MON HOC
// ============================================================

treeMH insertMH(treeMH r, const Monhoc &m) {
    if (!r) { nodeMH *p = new nodeMH; p->mh = m; p->left = p->right = NULL; return p; }
    if      (m.MAMH < r->mh.MAMH) r->left  = insertMH(r->left,  m);
    else if (m.MAMH > r->mh.MAMH) r->right = insertMH(r->right, m);
    else                           r->mh    = m;
    return r;
}
nodeMH *searchMH(treeMH r, const string &mamh) {
    if (!r) return NULL;
    if (mamh == r->mh.MAMH) return r;
    return mamh < r->mh.MAMH ? searchMH(r->left, mamh) : searchMH(r->right, mamh);
}
treeMH deleteMH(treeMH r, const string &mamh) {
    if (!r) return NULL;
    if      (mamh < r->mh.MAMH) r->left  = deleteMH(r->left,  mamh);
    else if (mamh > r->mh.MAMH) r->right = deleteMH(r->right, mamh);
    else {
        if (!r->left)  { nodeMH *t = r->right; delete r; return t; }
        if (!r->right) { nodeMH *t = r->left;  delete r; return t; }
        nodeMH *s = r->right; while (s->left) s = s->left;
        r->mh = s->mh; r->right = deleteMH(r->right, s->mh.MAMH);
    }
    return r;
}
void collectMH(treeMH r, vector<Monhoc> &v) {
    if (!r) return; collectMH(r->left, v); v.push_back(r->mh); collectMH(r->right, v);
}

// ============================================================
// LINKED LIST - SINH VIEN
// ============================================================

void insertSVSorted(PTRSV &head, const Sinhvien &sv) {
    nodeSV *p = new nodeSV; p->sv = sv; p->next = NULL;
    auto lt = [](const Sinhvien &a, const Sinhvien &b){ return a.TEN < b.TEN || (a.TEN == b.TEN && a.HO < b.HO); };
    if (!head || lt(sv, head->sv)) { p->next = head; head = p; return; }
    nodeSV *c = head; while (c->next && !lt(sv, c->next->sv)) c = c->next;
    p->next = c->next; c->next = p;
}
nodeSV *searchSV(PTRSV head, const string &masv) {
    for (; head; head = head->next) if (head->sv.MASV == masv) return head; return NULL;
}
bool deleteSV(PTRSV &head, const string &masv) {
    if (!head) return false;
    if (head->sv.MASV == masv) { nodeSV *t = head; head = head->next; delete t; return true; }
    for (nodeSV *p = head; p->next; p = p->next)
        if (p->next->sv.MASV == masv) { nodeSV *t = p->next; p->next = t->next; delete t; return true; }
    return false;
}
void freeSVList(PTRSV &h) { while (h) { nodeSV *t = h->next; delete h; h = t; } }

// ============================================================
// LINKED LIST - DANG KY
// ============================================================

void insertDK(PTRDK &head, const Dangky &dk) {
    nodeDK *p = new nodeDK; p->dk = dk; p->next = head; head = p;
}
nodeDK *searchDK(PTRDK head, const string &masv) {
    for (; head; head = head->next) if (head->dk.MASV == masv) return head; return NULL;
}
void freeDKList(PTRDK &h) { while (h) { nodeDK *t = h->next; delete h; h = t; } }
int countDKActive(PTRDK h) { int n = 0; for (; h; h = h->next) if (!h->dk.HUYDK) n++; return n; }

// ============================================================
// TIM KIEM CHUNG
// ============================================================

int findLopIdx(const DS_LOP &dl, const string &ml) {
    for (int i = 0; i < dl.n; i++) if (dl.nodes[i].MALOP == ml) return i; return -1;
}
int findLTCIdx(const DS_LTC &dt, int maloptc) {
    for (int i = 0; i < dt.n; i++) if (dt.nodes[i] && dt.nodes[i]->MALOPTC == maloptc) return i; return -1;
}
int findLTCBy(const DS_LTC &dt, const string &nk, int hk, const string &mamh, int nhom) {
    for (int i = 0; i < dt.n; i++) {
        auto *l = dt.nodes[i];
        if (l && l->NIENKHOA == nk && l->HOCKY == hk && l->MAMH == mamh && l->NHOM == nhom) return i;
    }
    return -1;
}
Sinhvien *findSVGlobal(DS_LOP &dl, const string &masv) {
    for (int i = 0; i < dl.n; i++) { auto *n = searchSV(dl.nodes[i].dssv, masv); if (n) return &n->sv; }
    return NULL;
}

// ============================================================
// (a) MO LOP TIN CHI
// ============================================================

void themLTC(DS_LTC &dt, treeMH root) {
    if (dt.n >= MAXLTC) { cout << "  [!] DS LTC da day!\n"; return; }
    string mamh = inputStr("  Ma mon hoc     : ");
    if (mamh.empty() || !searchMH(root, mamh)) { cout << "  [!] Ma MH khong hop le hoac khong ton tai!\n"; return; }
    auto *p      = new Loptinchi();
    p->MALOPTC   = ++gMaLopTC;
    p->MAMH      = mamh;
    p->NIENKHOA  = inputStr("  Nien khoa      : ");
    p->HOCKY     = inputInt("  Hoc ky (1-3)   : ", 1, 3);
    p->NHOM      = inputInt("  Nhom            : ", 1, 200);
    p->SOSVMIN   = inputInt("  SV toi thieu    : ", 1, 500);
    p->SOSVMAX   = inputInt("  SV toi da       : ", p->SOSVMIN, 500);
    dt.nodes[dt.n++] = p;
    cout << "  [OK] Mo lop tin chi thanh cong. Ma LTC = " << p->MALOPTC << "\n";
}
void xoaLTC(DS_LTC &dt) {
    int id = inputInt("  Ma LTC can xoa: ");
    int idx = findLTCIdx(dt, id);
    if (idx == -1) { cout << "  [!] Khong tim thay!\n"; return; }
    freeDKList(dt.nodes[idx]->dssvdk); delete dt.nodes[idx];
    for (int i = idx; i < dt.n - 1; i++) dt.nodes[i] = dt.nodes[i+1];
    dt.nodes[--dt.n] = NULL;
    cout << "  [OK] Da xoa LTC!\n";
}
void suaLTC(DS_LTC &dt, treeMH root) {
    int id = inputInt("  Ma LTC can sua: ");
    int idx = findLTCIdx(dt, id);
    if (idx == -1) { cout << "  [!] Khong tim thay!\n"; return; }
    auto *p = dt.nodes[idx];
    cout << "  MAMH=" << p->MAMH << " NK=" << p->NIENKHOA << " HK=" << p->HOCKY
         << " Nhom=" << p->NHOM << " Min=" << p->SOSVMIN << " Max=" << p->SOSVMAX << "\n";
    string mamh = inputStrOpt("  Ma MH moi    ", p->MAMH);
    if (mamh != p->MAMH && !searchMH(root, mamh)) { cout << "  [!] MH khong ton tai, giu nguyen.\n"; mamh = p->MAMH; }
    p->MAMH     = mamh;
    p->NIENKHOA = inputStrOpt("  Nien khoa moi", p->NIENKHOA);
    p->HOCKY    = inputIntOpt("  Hoc ky moi   ", p->HOCKY,   1, 3);
    p->NHOM     = inputIntOpt("  Nhom moi     ", p->NHOM,    1, 200);
    p->SOSVMIN  = inputIntOpt("  SV min moi   ", p->SOSVMIN, 1, 500);
    p->SOSVMAX  = inputIntOpt("  SV max moi   ", p->SOSVMAX, p->SOSVMIN, 500);
    cout << "  [OK] Da cap nhat LTC!\n";
}
void moLopTinChi(DS_LTC &dt, treeMH root) {
    int ch; do {
        cout << "\n===== (a) MO LOP TIN CHI =====\n  1.Them  2.Xoa  3.Sua  0.Thoat\n";
        ch = inputInt("  >> ", 0, 3);
        if (ch == 1) themLTC(dt, root); else if (ch == 2) xoaLTC(dt); else if (ch == 3) suaLTC(dt, root);
    } while (ch != 0);
}

// ============================================================
// (b) IN DSSV DA DANG KY LTC
// ============================================================

void inDSSVDangKy(DS_LTC &dt, DS_LOP &dl) {
    cout << "\n===== (b) IN DSSV DA DANG KY =====\n";
    string nk = inputStr("  Nien khoa: "); int hk = inputInt("  Hoc ky: ", 1, 3);
    string mamh = inputStr("  Ma MH: "); int nhom = inputInt("  Nhom: ", 1, 200);
    int idx = findLTCBy(dt, nk, hk, mamh, nhom);
    if (idx == -1) { cout << "  [!] Khong tim thay LTC!\n"; return; }
    auto *ltc = dt.nodes[idx];
    vector<pair<string, nodeDK*>> v;
    for (auto *p = ltc->dssvdk; p; p = p->next) v.push_back({p->dk.MASV, p});
    sort(v.begin(), v.end());
    cout << "\nDSSV DA DANG KY (Ma LTC=" << ltc->MALOPTC << ")\n";
    printLine(75); cout << left << setw(5) << "STT" << setw(14) << "Ma SV" << setw(38) << "Ho Ten" << "Trang thai\n"; printLine(75);
    int stt = 1;
    for (auto &[masv, dk] : v) {
        auto *sv = findSVGlobal(dl, masv);
        cout << left << setw(5) << stt++ << setw(14) << masv
             << setw(38) << (sv ? sv->HO + " " + sv->TEN : "(?)")
             << ((dk->dk.HUYDK || ltc->HUYLOP) ? "Da huy" : "Dang ky") << "\n";
    }
    printLine(75); cout << "Tong: " << v.size() << " SV.\n";
}

// ============================================================
// (c) NHAP SV (Them/Xoa/Sua, sap xep theo TEN)
// ============================================================

void themSV(DS_LOP &dl, const string &ml) {
    int li = findLopIdx(dl, ml); if (li == -1) { cout << "  [!] Khong tim thay lop!\n"; return; }
    cout << "  (De trong Ma SV de ket thuc)\n";
    for (;;) {
        string masv = inputStr("  Ma SV : "); if (masv.empty()) break;
        if (searchSV(dl.nodes[li].dssv, masv)) { cout << "  [!] MASV da ton tai!\n"; continue; }
        Sinhvien sv; sv.MASV = masv;
        sv.HO = inputStr("  Ho    : "); sv.TEN = inputStr("  Ten   : ");
        sv.PHAI = inputStr("  Phai  : "); sv.SODT = inputStr("  So DT : ");
        insertSVSorted(dl.nodes[li].dssv, sv);
        cout << "  [OK] Da them SV " << masv << "\n";
    }
}
void xoaSV(DS_LOP &dl, const string &ml) {
    int li = findLopIdx(dl, ml); if (li == -1) { cout << "  [!] Khong tim thay lop!\n"; return; }
    string masv = inputStr("  Ma SV can xoa: ");
    cout << (deleteSV(dl.nodes[li].dssv, masv) ? "  [OK] Da xoa!\n" : "  [!] Khong tim thay!\n");
}
void suaSV(DS_LOP &dl, const string &ml) {
    int li = findLopIdx(dl, ml); if (li == -1) { cout << "  [!] Khong tim thay lop!\n"; return; }
    string masv = inputStr("  Ma SV can sua: ");
    auto *node = searchSV(dl.nodes[li].dssv, masv); if (!node) { cout << "  [!] Khong tim thay!\n"; return; }
    auto &sv = node->sv;
    cout << "  Ho=" << sv.HO << " Ten=" << sv.TEN << " Phai=" << sv.PHAI << " SDT=" << sv.SODT << "\n";
    sv.HO = inputStrOpt("  Ho moi    ", sv.HO);
    sv.PHAI = inputStrOpt("  Phai moi  ", sv.PHAI);
    sv.SODT = inputStrOpt("  So DT moi ", sv.SODT);
    string nt = inputStrOpt("  Ten moi   ", sv.TEN);
    if (nt != sv.TEN) { sv.TEN = nt; Sinhvien sc = sv; deleteSV(dl.nodes[li].dssv, masv); insertSVSorted(dl.nodes[li].dssv, sc); }
    cout << "  [OK] Da cap nhat!\n";
}
void nhapSV(DS_LOP &dl) {
    cout << "\n===== (c) NHAP SV =====\n";
    string ml = inputStr("  Ma lop: "); int li = findLopIdx(dl, ml);
    if (li == -1) { cout << "  [!] Khong tim thay lop!\n"; return; }
    cout << "  Lop: " << dl.nodes[li].TENLOP << "\n";
    int ch; do {
        cout << "\n  1.Them  2.Xoa  3.Sua  0.Thoat\n"; ch = inputInt("  >> ", 0, 3);
        if (ch == 1) themSV(dl, ml); else if (ch == 2) xoaSV(dl, ml); else if (ch == 3) suaSV(dl, ml);
    } while (ch != 0);
}

// ============================================================
// (d) IN DSSV CUA LOP (tang dan MASV)
// ============================================================

void inDSSVLop(DS_LOP &dl) {
    cout << "\n===== (d) IN DSSV CUA LOP =====\n";
    string ml = inputStr("  Ma lop: "); int li = findLopIdx(dl, ml);
    if (li == -1) { cout << "  [!] Khong tim thay!\n"; return; }
    vector<Sinhvien> v; for (auto *p = dl.nodes[li].dssv; p; p = p->next) v.push_back(p->sv);
    sort(v.begin(), v.end(), [](const Sinhvien &a, const Sinhvien &b){ return a.MASV < b.MASV; });
    cout << "\nDSSV LOP: " << dl.nodes[li].TENLOP << " [" << ml << "]\n";
    printLine(80); cout << left << setw(5) << "STT" << setw(14) << "Ma SV" << setw(22) << "Ho" << setw(14) << "Ten" << setw(8) << "Phai" << "So DT\n"; printLine(80);
    int stt = 1;
    for (auto &sv : v) cout << left << setw(5) << stt++ << setw(14) << sv.MASV << setw(22) << sv.HO << setw(14) << sv.TEN << setw(8) << sv.PHAI << sv.SODT << "\n";
    printLine(80); cout << "Tong: " << v.size() << " SV.\n";
}

// ============================================================
// (e) NHAP MON HOC
// ============================================================

void themMH(treeMH &root) {
    string mamh = inputStr("  Ma MH (<=10): "); if (mamh.empty() || mamh.size() > 10) { cout << "  [!] Khong hop le!\n"; return; }
    if (searchMH(root, mamh)) { cout << "  [!] Ma MH da ton tai!\n"; return; }
    Monhoc mh; mh.MAMH = mamh; mh.TENMH = inputStr("  Ten MH (<=50): ");
    if (mh.TENMH.empty()) { cout << "  [!] Ten MH khong duoc trong!\n"; return; }
    mh.STC_LT = inputInt("  TC Ly thuyet: ", 0, 10); mh.STC_TH = inputInt("  TC Thuc hanh: ", 0, 10);
    root = insertMH(root, mh); cout << "  [OK] Da them mon hoc!\n";
}
void xoaMH(treeMH &root) {
    string mamh = inputStr("  Ma MH can xoa: "); if (!searchMH(root, mamh)) { cout << "  [!] Khong tim thay!\n"; return; }
    cout << "  Xac nhan xoa '" << mamh << "'? (y/n): "; char c; cin >> c; cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (c == 'y' || c == 'Y') { root = deleteMH(root, mamh); cout << "  [OK] Da xoa!\n"; } else cout << "  Da huy.\n";
}
void suaMH(treeMH &root) {
    string mamh = inputStr("  Ma MH can sua: "); auto *node = searchMH(root, mamh);
    if (!node) { cout << "  [!] Khong tim thay!\n"; return; }
    auto &mh = node->mh; cout << "  Ten=" << mh.TENMH << " LT=" << mh.STC_LT << " TH=" << mh.STC_TH << "\n";
    mh.TENMH = inputStrOpt("  Ten MH moi ", mh.TENMH);
    mh.STC_LT = inputIntOpt("  TC LT moi  ", mh.STC_LT, 0, 10);
    mh.STC_TH = inputIntOpt("  TC TH moi  ", mh.STC_TH, 0, 10);
    cout << "  [OK] Da cap nhat!\n";
}
void nhapMonHoc(treeMH &root) {
    int ch; do {
        cout << "\n===== (e) QUAN LY MON HOC =====\n  1.Them  2.Xoa  3.Sua  0.Thoat\n"; ch = inputInt("  >> ", 0, 3);
        if (ch == 1) themMH(root); else if (ch == 2) xoaMH(root); else if (ch == 3) suaMH(root);
    } while (ch != 0);
}

// ============================================================
// (f) IN DS MON HOC (tang dan TENMH)
// ============================================================

void inDSMonHoc(treeMH root) {
    cout << "\n===== (f) DANH SACH MON HOC =====\n";
    if (!root) { cout << "  DS trong!\n"; return; }
    vector<Monhoc> v; collectMH(root, v);
    sort(v.begin(), v.end(), [](const Monhoc &a, const Monhoc &b){ return a.TENMH < b.TENMH; });
    printLine(80); cout << left << setw(5) << "STT" << setw(12) << "Ma MH" << setw(50) << "Ten Mon Hoc" << setw(8) << "TC LT" << "TC TH\n"; printLine(80);
    int stt = 1; for (auto &m : v) cout << left << setw(5) << stt++ << setw(12) << m.MAMH << setw(50) << m.TENMH << setw(8) << m.STC_LT << m.STC_TH << "\n";
    printLine(80); cout << "Tong: " << v.size() << " mon hoc.\n";
}

// ============================================================
// (g) DANG KY LOP TIN CHI
// ============================================================

void dangKyLTC(DS_LTC &dt, DS_LOP &dl, treeMH root) {
    cout << "\n===== (g) DANG KY LOP TIN CHI =====\n";
    string masv = inputStr("  Ma SV: "); auto *sv = findSVGlobal(dl, masv);
    if (!sv) { cout << "  [!] Khong tim thay SV!\n"; return; }
    cout << "  MASV=" << sv->MASV << " Ho=" << sv->HO << " Ten=" << sv->TEN << " Phai=" << sv->PHAI << " SDT=" << sv->SODT << "\n";
    string nk = inputStr("  Nien khoa: "); int hk = inputInt("  Hoc ky: ", 1, 3);
    vector<int> avail;
    cout << "\n  CAC LOP TC CO THE DANG KY:\n"; printLine(90);
    cout << left << setw(5) << "STT" << setw(8) << "MaLTC" << setw(12) << "Ma MH" << setw(38) << "Ten Mon Hoc" << setw(7) << "Nhom" << setw(10) << "Da DK" << "Con trong\n"; printLine(90);
    for (int i = 0; i < dt.n; i++) {
        auto *ltc = dt.nodes[i];
        if (!ltc || ltc->HUYLOP || ltc->NIENKHOA != nk || ltc->HOCKY != hk) continue;
        int daDK = countDKActive(ltc->dssvdk), con = ltc->SOSVMAX - daDK;
        if (con <= 0) continue;
        auto *dk = searchDK(ltc->dssvdk, masv); if (dk && !dk->dk.HUYDK) continue;
        auto *mhN = searchMH(root, ltc->MAMH);
        cout << left << setw(5) << (int)avail.size()+1 << setw(8) << ltc->MALOPTC << setw(12) << ltc->MAMH
             << setw(38) << (mhN ? mhN->mh.TENMH : "(?)") << setw(7) << ltc->NHOM << setw(10) << daDK << con << "\n";
        avail.push_back(i);
    }
    if (avail.empty()) { cout << "  Khong co lop TC phu hop!\n"; return; }
    printLine(90);
    int ch = inputInt("  Chon STT (0=thoat): ", 0, (int)avail.size()); if (ch == 0) return;
    auto *ltc = dt.nodes[avail[ch-1]];
    auto *ex = searchDK(ltc->dssvdk, masv);
    if (ex) { ex->dk.HUYDK = false; cout << "  [OK] Da kich hoat lai dang ky!\n"; }
    else { Dangky dk; dk.MASV = masv; dk.DIEM = -1.0f; dk.HUYDK = false; insertDK(ltc->dssvdk, dk); cout << "  [OK] Dang ky thanh cong! Ma LTC=" << ltc->MALOPTC << "\n"; }
}

// ============================================================
// (h) HUY LOP TIN CHI TU DONG
// ============================================================

void huyLopTinChi(DS_LTC &dt) {
    cout << "\n===== (h) HUY LOP TIN CHI =====\n";
    string nk = inputStr("  Nien khoa: "); int hk = inputInt("  Hoc ky: ", 1, 3);
    vector<int> toCancel;
    cout << "\n  CAC LOP TC SE BI HUY:\n"; printLine(60);
    cout << left << setw(8) << "Ma LTC" << setw(12) << "Ma MH" << setw(7) << "Nhom" << setw(10) << "So SV DK" << "SV Min\n"; printLine(60);
    for (int i = 0; i < dt.n; i++) {
        auto *ltc = dt.nodes[i]; if (!ltc || ltc->HUYLOP || ltc->NIENKHOA != nk || ltc->HOCKY != hk) continue;
        int daDK = countDKActive(ltc->dssvdk);
        if (daDK < ltc->SOSVMIN) { cout << left << setw(8) << ltc->MALOPTC << setw(12) << ltc->MAMH << setw(7) << ltc->NHOM << setw(10) << daDK << ltc->SOSVMIN << "\n"; toCancel.push_back(i); }
    }
    if (toCancel.empty()) { cout << "  Khong co lop nao can huy!\n"; return; }
    printLine(60); cout << "  Xac nhan huy " << toCancel.size() << " lop? (y/n): ";
    char c; cin >> c; cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (c != 'y' && c != 'Y') { cout << "  Da huy bo.\n"; return; }
    for (int idx : toCancel) { dt.nodes[idx]->HUYLOP = true; for (auto *p = dt.nodes[idx]->dssvdk; p; p = p->next) p->dk.HUYDK = true; }
    cout << "  [OK] Da huy " << toCancel.size() << " LTC!\n";
}

// ============================================================
// (i) NHAP DIEM
// ============================================================

void nhapDiem(DS_LTC &dt, DS_LOP &dl) {
    cout << "\n===== (i) NHAP DIEM =====\n";
    string nk = inputStr("  Nien khoa: "); int hk = inputInt("  Hoc ky: ", 1, 3);
    string mamh = inputStr("  Ma MH: "); int nhom = inputInt("  Nhom: ", 1, 200);
    int idx = findLTCBy(dt, nk, hk, mamh, nhom); if (idx == -1) { cout << "  [!] Khong tim thay LTC!\n"; return; }
    auto *ltc = dt.nodes[idx]; if (ltc->HUYLOP) { cout << "  [!] LTC da bi huy!\n"; return; }
    vector<nodeDK*> dkList;
    for (auto *p = ltc->dssvdk; p; p = p->next) if (!p->dk.HUYDK) dkList.push_back(p);
    sort(dkList.begin(), dkList.end(), [](nodeDK *a, nodeDK *b){ return a->dk.MASV < b->dk.MASV; });
    if (dkList.empty()) { cout << "  Khong co SV dang ky!\n"; return; }
    auto printTbl = [&]() {
        cout << "\n  NK=" << nk << " HK=" << hk << " MH=" << mamh << " Nhom=" << nhom << "\n";
        printLine(75); cout << left << setw(5) << "STT" << setw(14) << "Ma SV" << setw(22) << "Ho" << setw(14) << "Ten" << "Diem\n"; printLine(75);
        int s = 1;
        for (auto *dk : dkList) { auto *sv = findSVGlobal(dl, dk->dk.MASV); cout << left << setw(5) << s++ << setw(14) << dk->dk.MASV << setw(22) << (sv ? sv->HO : "?") << setw(14) << (sv ? sv->TEN : "?") << fmtFloat(dk->dk.DIEM) << "\n"; }
        printLine(75);
    };
    printTbl(); cout << "  Nhap STT de nhap/sua diem (0=xong):\n";
    for (;;) { int s = inputInt("  STT: ", 0, (int)dkList.size()); if (s == 0) break; dkList[s-1]->dk.DIEM = inputFloat("  Diem (0-10): "); printTbl(); }
    cout << "  [OK] Hoan thanh!\n";
}

// ============================================================
// (j) IN BANG DIEM MON HOC
// ============================================================

void inBangDiemMH(DS_LTC &dt, DS_LOP &dl, treeMH root) {
    cout << "\n===== (j) BANG DIEM MON HOC =====\n";
    string nk = inputStr("  Nien khoa: "); int hk = inputInt("  Hoc ky: ", 1, 3);
    string mamh = inputStr("  Ma MH: "); int nhom = inputInt("  Nhom: ", 1, 200);
    int idx = findLTCBy(dt, nk, hk, mamh, nhom); if (idx == -1) { cout << "  [!] Khong tim thay LTC!\n"; return; }
    auto *ltc = dt.nodes[idx]; auto *mhN = searchMH(root, mamh);
    vector<nodeDK*> dkList;
    for (auto *p = ltc->dssvdk; p; p = p->next) if (!p->dk.HUYDK) dkList.push_back(p);
    sort(dkList.begin(), dkList.end(), [](nodeDK *a, nodeDK *b){ return a->dk.MASV < b->dk.MASV; });
    cout << "\nBANG DIEM MON HOC " << (mhN ? mhN->mh.TENMH : mamh) << "\n";
    cout << "  Nien khoa: " << nk << "   Hoc ky: " << hk << "   Nhom: " << nhom << "\n";
    printLine(75); cout << left << setw(5) << "STT" << setw(14) << "Ma SV" << setw(22) << "Ho" << setw(14) << "Ten" << "Diem\n"; printLine(75);
    int stt = 1;
    for (auto *dk : dkList) { auto *sv = findSVGlobal(dl, dk->dk.MASV); cout << left << setw(5) << stt++ << setw(14) << dk->dk.MASV << setw(22) << (sv ? sv->HO : "?") << setw(14) << (sv ? sv->TEN : "?") << fmtFloat(dk->dk.DIEM) << "\n"; }
    printLine(75);
}

// ============================================================
// (k) BANG DIEM TB KHOA HOC (theo tin chi)
// ============================================================

void inDiemTBKhoaHoc(DS_LTC &dt, DS_LOP &dl, treeMH root) {
    cout << "\n===== (k) BANG DIEM TRUNG BINH KHOA HOC =====\n";
    string ml = inputStr("  Ma lop: "); int li = findLopIdx(dl, ml);
    if (li == -1) { cout << "  [!] Khong tim thay lop!\n"; return; }
    vector<Sinhvien> svList; for (auto *p = dl.nodes[li].dssv; p; p = p->next) svList.push_back(p->sv);
    sort(svList.begin(), svList.end(), [](const Sinhvien &a, const Sinhvien &b){ return a.MASV < b.MASV; });
    cout << "\nBANG THONG KE DIEM TRUNG BINH KHOA HOC\n  Lop: " << ml << "\n";
    printLine(70); cout << left << setw(5) << "STT" << setw(14) << "Ma SV" << setw(22) << "Ho" << setw(14) << "Ten" << "Diem TB\n"; printLine(70);
    int stt = 1;
    for (auto &sv : svList) {
        float tD = 0, tC = 0;
        for (int i = 0; i < dt.n; i++) {
            auto *ltc = dt.nodes[i]; if (!ltc || ltc->HUYLOP) continue;
            auto *dk = searchDK(ltc->dssvdk, sv.MASV); if (!dk || dk->dk.HUYDK || dk->dk.DIEM < 0) continue;
            auto *mhN = searchMH(root, ltc->MAMH); if (!mhN) continue;
            float tc = (float)(mhN->mh.STC_LT + mhN->mh.STC_TH); tD += dk->dk.DIEM * tc; tC += tc;
        }
        cout << left << setw(5) << stt++ << setw(14) << sv.MASV << setw(22) << sv.HO << setw(14) << sv.TEN
             << fixed << setprecision(2) << (tC > 0 ? tD/tC : 0.0f) << "\n";
    }
    printLine(70);
}

// ============================================================
// (l) BANG DIEM TONG KET (diem cao nhat tung mon)
// ============================================================

void inBangDiemTongKet(DS_LTC &dt, DS_LOP &dl, treeMH root) {
    cout << "\n===== (l) BANG DIEM TONG KET =====\n";
    string ml = inputStr("  Ma lop: "); int li = findLopIdx(dl, ml);
    if (li == -1) { cout << "  [!] Khong tim thay lop!\n"; return; }
    vector<Sinhvien> svList; for (auto *p = dl.nodes[li].dssv; p; p = p->next) svList.push_back(p->sv);
    sort(svList.begin(), svList.end(), [](const Sinhvien &a, const Sinhvien &b){ return a.MASV < b.MASV; });
    if (svList.empty()) { cout << "  Lop khong co SV!\n"; return; }
    set<string> mamhSet;
    for (auto &sv : svList) for (int i = 0; i < dt.n; i++) {
        auto *ltc = dt.nodes[i]; if (!ltc || ltc->HUYLOP) continue;
        auto *dk = searchDK(ltc->dssvdk, sv.MASV); if (dk && !dk->dk.HUYDK && dk->dk.DIEM >= 0) mamhSet.insert(ltc->MAMH);
    }
    vector<string> mamhList(mamhSet.begin(), mamhSet.end()); sort(mamhList.begin(), mamhList.end());
    int W = 42 + (int)mamhList.size() * 10;
    cout << "\nBANG DIEM TONG KET\n  Lop: " << ml << "\n"; printLine(W);
    cout << left << setw(5) << "STT" << setw(12) << "Ma SV" << setw(25) << "Ho Ten";
    for (auto &m : mamhList) cout << setw(10) << m; cout << "\n"; printLine(W);
    int stt = 1;
    for (auto &sv : svList) {
        cout << left << setw(5) << stt++ << setw(12) << sv.MASV << setw(25) << (sv.HO + " " + sv.TEN);
        for (auto &mamh : mamhList) {
            float mx = -1.0f;
            for (int i = 0; i < dt.n; i++) {
                auto *ltc = dt.nodes[i]; if (!ltc || ltc->HUYLOP || ltc->MAMH != mamh) continue;
                auto *dk = searchDK(ltc->dssvdk, sv.MASV); if (dk && !dk->dk.HUYDK && dk->dk.DIEM >= 0) mx = max(mx, dk->dk.DIEM);
            }
            if (mx >= 0) { ostringstream o; o << fixed << setprecision(1) << mx; cout << setw(10) << o.str(); }
            else cout << setw(10) << "---";
        }
        cout << "\n";
    }
    printLine(W);
}

// ============================================================
// QUAN LY LOP
// ============================================================

void quanLyLop(DS_LOP &dl) {
    int ch; do {
        cout << "\n===== QUAN LY LOP =====\n  1.Them  2.Xoa  3.Sua  4.Xem  0.Thoat\n"; ch = inputInt("  >> ", 0, 4);
        if (ch == 1) {
            if (dl.n >= MAXLOP) { cout << "  [!] Da day!\n"; continue; }
            auto &l = dl.nodes[dl.n]; l.MALOP = inputStr("  Ma lop : ");
            if (l.MALOP.empty()) { cout << "  [!] Khong duoc trong!\n"; continue; }
            if (findLopIdx(dl, l.MALOP) != -1) { cout << "  [!] Da ton tai!\n"; continue; }
            l.TENLOP = inputStr("  Ten lop: "); l.dssv = NULL; dl.n++; cout << "  [OK] Da them!\n";
        } else if (ch == 2) {
            string ml = inputStr("  Ma lop can xoa: "); int idx = findLopIdx(dl, ml);
            if (idx == -1) { cout << "  [!] Khong tim thay!\n"; continue; }
            freeSVList(dl.nodes[idx].dssv); for (int i = idx; i < dl.n-1; i++) dl.nodes[i] = dl.nodes[i+1]; dl.n--; cout << "  [OK] Da xoa!\n";
        } else if (ch == 3) {
            string ml = inputStr("  Ma lop can sua: "); int idx = findLopIdx(dl, ml);
            if (idx == -1) { cout << "  [!] Khong tim thay!\n"; continue; }
            dl.nodes[idx].TENLOP = inputStrOpt("  Ten lop moi", dl.nodes[idx].TENLOP); cout << "  [OK] Da cap nhat!\n";
        } else if (ch == 4) {
            printLine(50); cout << left << setw(5) << "STT" << setw(15) << "Ma lop" << setw(25) << "Ten lop" << "So SV\n"; printLine(50);
            for (int i = 0; i < dl.n; i++) { int cnt = 0; for (auto *p = dl.nodes[i].dssv; p; p = p->next) cnt++; cout << left << setw(5) << i+1 << setw(15) << dl.nodes[i].MALOP << setw(25) << dl.nodes[i].TENLOP << cnt << "\n"; }
        }
    } while (ch != 0);
}

// ============================================================
// FILE I/O
// ============================================================

const string FILE_MH  = "monhoc.txt";
const string FILE_LOP = "dslop.txt";
const string FILE_LTC = "dsltc.txt";

void saveToFile(treeMH root, DS_LOP &dl, DS_LTC &dt) {
    {
        ofstream f(FILE_MH); if (!f) { cout << "  [!] Loi mo file monhoc.txt!\n"; return; }
        vector<Monhoc> v; collectMH(root, v); f << v.size() << "\n";
        for (auto &m : v) f << m.MAMH << "\n" << m.TENMH << "\n" << m.STC_LT << "\n" << m.STC_TH << "\n";
    }
    {
        ofstream f(FILE_LOP); if (!f) { cout << "  [!] Loi mo file dslop.txt!\n"; return; }
        f << dl.n << "\n";
        for (int i = 0; i < dl.n; i++) {
            int cnt = 0; for (auto *p = dl.nodes[i].dssv; p; p = p->next) cnt++;
            f << dl.nodes[i].MALOP << "\n" << dl.nodes[i].TENLOP << "\n" << cnt << "\n";
            for (auto *p = dl.nodes[i].dssv; p; p = p->next)
                f << p->sv.MASV << "\n" << p->sv.HO << "\n" << p->sv.TEN << "\n" << p->sv.PHAI << "\n" << p->sv.SODT << "\n";
        }
    }
    {
        ofstream f(FILE_LTC); if (!f) { cout << "  [!] Loi mo file dsltc.txt!\n"; return; }
        f << gMaLopTC << "\n" << dt.n << "\n";
        for (int i = 0; i < dt.n; i++) {
            auto *ltc = dt.nodes[i]; int cnt = 0; for (auto *p = ltc->dssvdk; p; p = p->next) cnt++;
            f << ltc->MALOPTC << "\n" << ltc->MAMH << "\n" << ltc->NIENKHOA << "\n"
              << ltc->HOCKY << "\n" << ltc->NHOM << "\n" << ltc->SOSVMIN << "\n"
              << ltc->SOSVMAX << "\n" << ltc->HUYLOP << "\n" << cnt << "\n";
            for (auto *p = ltc->dssvdk; p; p = p->next)
                f << p->dk.MASV << "\n" << p->dk.DIEM << "\n" << p->dk.HUYDK << "\n";
        }
    }
    cout << "  [OK] Da luu du lieu vao file!\n";
}

void loadFromFile(treeMH &root, DS_LOP &dl, DS_LTC &dt) {
    {
        ifstream f(FILE_MH); if (f) {
            int n; f >> n; f.ignore(numeric_limits<streamsize>::max(), '\n');
            for (int i = 0; i < n; i++) { Monhoc m; getline(f, m.MAMH); getline(f, m.TENMH); f >> m.STC_LT >> m.STC_TH; f.ignore(numeric_limits<streamsize>::max(), '\n'); root = insertMH(root, m); }
            cout << "  Da tai " << n << " mon hoc.\n";
        }
    }
    {
        ifstream f(FILE_LOP); if (f) {
            int n; f >> n; f.ignore(numeric_limits<streamsize>::max(), '\n');
            for (int i = 0; i < n && i < MAXLOP; i++) {
                getline(f, dl.nodes[i].MALOP); getline(f, dl.nodes[i].TENLOP);
                int cnt; f >> cnt; f.ignore(numeric_limits<streamsize>::max(), '\n');
                for (int j = 0; j < cnt; j++) { Sinhvien sv; getline(f, sv.MASV); getline(f, sv.HO); getline(f, sv.TEN); getline(f, sv.PHAI); getline(f, sv.SODT); insertSVSorted(dl.nodes[i].dssv, sv); }
            }
            dl.n = n; cout << "  Da tai " << n << " lop.\n";
        }
    }
    {
        ifstream f(FILE_LTC); if (f) {
            f >> gMaLopTC; int n; f >> n; f.ignore(numeric_limits<streamsize>::max(), '\n');
            for (int i = 0; i < n && i < MAXLTC; i++) {
                auto *ltc = new Loptinchi();
                f >> ltc->MALOPTC; f.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(f, ltc->MAMH); getline(f, ltc->NIENKHOA);
                f >> ltc->HOCKY >> ltc->NHOM >> ltc->SOSVMIN >> ltc->SOSVMAX >> ltc->HUYLOP;
                int cnt; f >> cnt; f.ignore(numeric_limits<streamsize>::max(), '\n');
                for (int j = 0; j < cnt; j++) { Dangky dk; getline(f, dk.MASV); f >> dk.DIEM >> dk.HUYDK; f.ignore(numeric_limits<streamsize>::max(), '\n'); insertDK(ltc->dssvdk, dk); }
                dt.nodes[i] = ltc;
            }
            dt.n = n; cout << "  Da tai " << n << " lop tin chi.\n";
        }
    }
}

// ============================================================
// MAIN
// ============================================================

void printMenu() {
    cout << "\n"; printLine(62, '=');
    cout << "   QUAN LY DIEM SINH VIEN THEO HE TIN CHI\n"; printLine(62, '=');
    cout << "  a/ 1.  Mo lop tin chi           (Them/Xoa/Sua)\n"
         << "  b/ 2.  In DSSV da dang ky LTC\n"
         << "  c/ 3.  Nhap SV cua lop          (Them/Xoa/Sua)\n"
         << "  d/ 4.  In DSSV cua lop          (tang dan MASV)\n"
         << "  e/ 5.  Quan ly mon hoc          (Them/Xoa/Sua)\n"
         << "  f/ 6.  In danh sach mon hoc     (tang dan TENMH)\n"
         << "  g/ 7.  Dang ky lop tin chi\n"
         << "  h/ 8.  Huy lop tin chi tu dong\n"
         << "  i/ 9.  Nhap diem\n"
         << "  j/ 10. In bang diem mon hoc\n"
         << "  k/ 11. In bang diem TB khoa hoc (theo tin chi)\n"
         << "  l/ 12. In bang diem tong ket    (diem cao nhat)\n"
         << "     13. Quan ly lop              (Them/Xoa/Sua)\n"
         << "     14. Luu du lieu vao file\n"
         << "      0. Thoat\n";
    printLine(62, '=');
}

int main() {
    treeMH dsmh = NULL;
    DS_LTC dsltc; dsltc.n = 0;
    DS_LOP dslop; dslop.n = 0;
    for (int i = 0; i < MAXLTC; i++) dsltc.nodes[i] = NULL;
    cout << "Dang tai du lieu...\n"; loadFromFile(dsmh, dslop, dsltc);
    int ch; do {
        printMenu(); ch = inputInt("  >> Lua chon: ", 0, 14);
        switch (ch) {
            case  1: moLopTinChi(dsltc, dsmh);               break;
            case  2: inDSSVDangKy(dsltc, dslop);             break;
            case  3: nhapSV(dslop);                          break;
            case  4: inDSSVLop(dslop);                       break;
            case  5: nhapMonHoc(dsmh);                       break;
            case  6: inDSMonHoc(dsmh);                       break;
            case  7: dangKyLTC(dsltc, dslop, dsmh);         break;
            case  8: huyLopTinChi(dsltc);                    break;
            case  9: nhapDiem(dsltc, dslop);                 break;
            case 10: inBangDiemMH(dsltc, dslop, dsmh);      break;
            case 11: inDiemTBKhoaHoc(dsltc, dslop, dsmh);   break;
            case 12: inBangDiemTongKet(dsltc, dslop, dsmh); break;
            case 13: quanLyLop(dslop);                       break;
            case 14: saveToFile(dsmh, dslop, dsltc);         break;
        }
    } while (ch != 0);
    cout << "\n  Ban co muon luu du lieu? (y/n): ";
    char sv; cin >> sv; if (sv == 'y' || sv == 'Y') saveToFile(dsmh, dslop, dsltc);
    cout << "  Tam biet!\n"; return 0;
}
