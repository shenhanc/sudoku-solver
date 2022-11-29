#include <iostream>
#include <vector>

using namespace std;

class SubRec {
public:
  SubRec(const int *d, int recid) : d_(d), p_(recid / 3 * 27 + (recid % 3) * 3) {
  }

  int operator[](int i) const {
    return d_[p_ + (i / 3) * 9 + (i % 3)];
  }

  bool can_have(int v) const {
    for (int i = 0; i < 9; ++i)
      if ((*this)[i] == v) return false;
    return true;
  }

private:
  const int *d_;
  const int p_;

};

class Row {
public:
  Row(int *d, int r): d_(d), r_(r) {}

  int operator[](int i) const {
    return d_[r_ * 9 + i];
  }

  bool can_have(int v) const {
    for (int i = 0; i < 9; ++i)
      if ((*this)[i] == v) return false;
    return true;
  }

private:
  int *d_;
  const int r_;
};

class Col {
public:
  Col(int *d , int c): d_(d), c_(c) {}

  int operator[](int i) const {
    return d_[c_ + i * 9];
  }

  bool can_have(int v) const {
    for (int i = 0; i < 9; ++i)
      if ((*this)[i] == v) return false;
    return true;
  }

private:
  int *d_;
  const int c_;
};

class MainRec {
public:
  MainRec(int d[81]) {
    for (int i = 0; i < 81; ++i)
      d_[i] = d[i];
  }

  const int *d() {
    return d_;
  }

  int to_row(int p) const {
    return p / 9;
  }

  int to_col(int p) const {
    return p % 9;
  }

  int to_subrec_id(int p) {
    int r = p / 9;
    int c = p % 9;
    return r / 3 * 3 + c / 3;
  }

  bool solve(int step);

  void print() {
    for (int i = 0; i < 81; ++i) {
      if (i && i % 9 == 0)
	cout << endl;
      cout << d_[i] << " ";
    }
    cout << endl;
  }

  void pretty_print();

private:
  int d_[81];

};

bool MainRec::solve(int step) {
  int to_solve = 0;
  for (int i = 0; i < 81; ++i)
    if (!d_[i]) ++to_solve;
  cout << "Solve rectange. Step " << step << ", " << to_solve << " unsolved" << endl;
  print();
  while(to_solve) {
    vector<vector<int>> candidates(81);
    bool found_one = false;
    for (int p = 0; p < 81; ++p) {
      if (!d_[p]) {
	SubRec sr(d_, to_subrec_id(p));
	Row row(d_, to_row(p));
	Col col(d_, to_col(p));
	vector<int> &can = candidates[p];
	for (int t = 1; t <= 9; ++t) {
	  if (sr.can_have(t) && row.can_have(t) && col.can_have(t)) {
	    can.push_back(t);
	  }
	}
	if (can.size() == 0) {
	  cout << "Failed !" << endl;
	  return false;
	}
	if (can.size() == 1) {
	  cout << "[" << to_row(p) << ", " << to_col(p) << "]: " << can.front() << endl;
	  d_[p] = can.front();
	  to_solve--;
	  found_one = true;
	  break;
	}
      }
    }
    if (to_solve && !found_one) {
      while(true) {
	int min_can_n = 10;
	int min_can_p = -1;
	for (int i = 0; i < 81; ++i) {
	  if (!d_[i] && candidates[i].size() > 0 && candidates[i].size() < min_can_n) {
	    min_can_n = candidates[i].size();
	    min_can_p = i;
	  }
	}
	if (min_can_p == -1) {
	  cerr << "Nothing to try" << endl;
	  return false;
	}
	for (int c : candidates[min_can_p]) {
	  d_[min_can_p] = c;
	  MainRec m(d_);
	  cout << "try [" << to_row(min_can_p) << ", "
               << to_col(min_can_p) << "]=" << c << endl;
	  if (!m.solve(step + 1)) {
	    d_[min_can_p] = 0;
	  } else {
	    return true;
	  }
	}
        return false;
      }
    }
  }

  cout << "Found: " << endl;
  pretty_print();
  cout << "****" << endl;

  return true;
}

void MainRec::pretty_print() {
  for (int r = 0; r < 9; ++r) {
    if (r % 3 == 0)
      cout << " +-------+-------+-------+" << endl;
    Row row(d_, r);
    for (int c = 0; c < 9; ++c) {
      if (c % 3 == 0)
        cout << " |";
      cout << " " << row[c];
    }
    cout << " |" <<endl;
  }
  cout << " +-------+-------+-------+" << endl;
}

int main() {
  int d[81] = {
    5, 0, 4, 0, 9, 0, 0, 0, 0,
    0, 7, 0, 0, 0, 3, 4, 8, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 6,
    0, 0, 0, 0, 0, 0, 0, 2, 0,
    0, 0, 7, 8, 0, 0, 0, 0, 0,
    0, 8, 0, 0, 0, 4, 1, 3, 0,
    0, 0, 0, 0, 0, 6, 0, 0, 7,
    0, 0, 9, 0, 3, 0, 6, 1, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 2};
  MainRec m(d);
  m.solve(1);

  // for (int r = 0; r < 9; ++r) {
  //   SubRec sr(m.d(), r);
  //   for (int k = 0; k < 9; ++k) {
  //     cout << sr[k] << " ";
  //   }
  //   cout << endl;
  // }

  // for (int r = 0; r < 9; ++r) {
  //   Row row(m, r);
  //   for (int i = 0; i < 9; ++i) {
  //     cout << row[i] << " ";
  //   }
  //   cout << endl;
  // }
}
