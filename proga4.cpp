// It is a field for minesweeper in 3D. It has functions to set mines, click on
// the cells by entering commands. Also it has got a debugger which allows you
// to view any pytes of the program's memory.

#include <iostream>

using namespace std;

void error() { cout << "error\n"; }

void addMine(bool ***mines, int ***field, int X, int Y, int Z, int x, int y,
             int z) {
  if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) {
    cout << "Out of Bounds\n";
    return;
  }

  if (mines[x][y][z]) {
    cout << "The mine is already here\n";
  }
  mines[x][y][z] = true;

  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      for (int dz = -1; dz <= 1; ++dz) {
        if (dx == 0 && dy == 0 && dz == 0)
          continue;
        int nx = x + dx;
        int ny = y + dy;
        int nz = z + dz;
        if (nx >= 0 && nx < X && ny >= 0 && ny < Y && nz >= 0 && nz < Z) {
          ++field[nx][ny][nz];
        }
      }
    }
  }
}

void clickCell(bool ***mines, int ***field, int X, int Y, int Z, int x, int y,
               int z) {
  if (x < 0 || x >= X || y < 0 || y >= Y || z < 0 || z >= Z) {
    cout << "Out of Bounds\n";
    return;
  }
  if (mines[x][y][z]) {
    cout << "You are blown up :(\n";
    return;
  }
  cout << "there are " << field[x][y][z] << " mines nearby";
}

void *dbg_ptr = nullptr;

void dbg_set(void *p) { dbg_ptr = p; }

// next four functions made with commands AI recommended me

void dbg_move(std::ptrdiff_t bytes) {
  dbg_ptr = static_cast<char *>(dbg_ptr) + bytes;
}

void dbg_print() {
  std::cout << (*static_cast<bool *>(dbg_ptr) ? "true" : "false") << "  (at "
            << dbg_ptr << ")\n";
}

void dbg_bytes(std::size_t n) {
  auto *b = static_cast<unsigned char *>(dbg_ptr);
  for (std::size_t i = 0; i < n; ++i)
    std::cout << std::hex << (b[i] < 16 ? "0" : "") << (int)b[i] << ' ';
  std::cout << std::dec << '\n';
}

int main() {
  int X = 10, Y = 10, Z = 10;
  int ***field = new int **[X];
  for (int i = 0; i < X; ++i) {
    field[i] = new int *[Y];
    for (int j = 0; j < Y; ++j) {
      field[i][j] = new int[Z]();
    }
  }
  bool ***mines = new bool **[X];
  for (int i = 0; i < X; ++i) {
    mines[i] = new bool *[Y];
    for (int j = 0; j < Y; ++j) {
      mines[i][j] = new bool[Z];
    }
  }
  std::string cmd;
  while (std::cout << "> ", std::cin >> cmd) {
    if (cmd == "quit" || cmd == "exit")
      break;
    else if (cmd == "help")
      std::cout << "set field|mines X Y Z | set addr 0xADDR | move N | show\n"
                   "print | bytes N | addmine | click\n";
    else if (cmd == "set") {
      std::string w;
      std::cin >> w;
      if (w == "addr") {
        std::string a;
        std::cin >> a;
        dbg_ptr =
            reinterpret_cast<void *>(std::strtoull(a.c_str(), nullptr, 0));
      } else {
        int i, j, k;
        std::cin >> i >> j >> k;
        if (w == "field")
          dbg_ptr = &field[i][j][k];
        else if (w == "mines")
          dbg_ptr = &mines[i][j][k];
      }
    } else if (cmd == "move") {
      std::ptrdiff_t n;
      std::cin >> n;
      dbg_move(n);
    } else if (cmd == "show") {
      std::cout << dbg_ptr << '\n';
    } else if (cmd == "bytes") {
      std::size_t n;
      std::cin >> n;
      dbg_bytes(n);
    } else if (cmd == "addmine") {
      int x, y, z;
      cin >> x >> y >> z;
      addMine(mines, field, X, Y, Z, x, y, z);
    } else if (cmd == "click") {
      int x, y, z;
      cin >> x >> y >> z;
      clickCell(mines, field, X, Y, Z, x, y, z);
    } else if (cmd == "print")
      dbg_print();
    else
      std::cout << "unknown. type 'help'\n";
  }
}
