# 🧪 C++ Unit Test Generator for orgChartApi

This project automates C++ unit test creation, refinement, and coverage improvement for the `orgChartApi` microservice. It leverages **LLM-driven YAML instructions**, **GoogleTest**, and **real PostgreSQL integration** — no mocks, just real data.

---

## 📁 Project Structure
```
unit-test-generator/
├── orgChartApi/ # Backend (clone separately, see setup)
├── tests/ # Generated/refined unit tests (GoogleTest)
├── yaml/ # YAML instructions for LLM
├── db/ # schema.sql & seed.sql for DB setup
├── build/ # Build/coverage artifacts
├── CMakeLists.txt
├── generate_tests.py # LLM-powered test generation script
├── refine_tests.py # Test deduplication/refinement
├── improve_coverage.py # Coverage-driven test improvement
└── README.md
```


---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/Hurshikesh/keploy-unit-test-generator.git
cd keploy-unit-test-generator
```
### 2.  Clone orgChartApi
Note: orgChartApi is not a submodule. Clone it manually into the root directory:
```
git clone https://github.com/keploy/orgChartApi.git
cd orgChartApi
git submodule update --init --recursive
cd ..
```

### 3. Set Up the Database
Install PostgreSQL (locally or via Docker).

Create and seed the test database:

```
createdb -U testuser2 -h localhost -p 5432 testdb
psql -U testuser2 -d testdb -f db/schema.sql
psql -U testuser2 -d testdb -f db/seed.sql

```

### 4. Install Dependencies
C++ / Coverage Tools
macOS:
```
brew install cmake lcov gcovr
pip install -r requirements.txt

```

### 5. Build and Run Tests
Build with coverage flags:
```
rm -rf build                                                                                      
mkdir build
cd build
cp ../config.json .
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="--coverage" ..
make
```
Run tests:
```
./AllTests
```

### 6. Generate Coverage Report

```
gcovr -r .. \
  --filter '/Users/hurshikeshsahu/keploy-unit-test-generator/orgChartApi/' \
  --exclude 'tests' \
  --exclude '_deps' \
  --exclude 'third_party' \
  --exclude '.*third_party.*' \
  --exclude '.*_deps.*' \
  --html --html-details -o coverage.html
```

## 🧩 Features
1. LLM-driven test generation: Uses YAML prompts to guide GPT-4o for test creation and refinement.

2. No DB mocking: Tests interact with a real PostgreSQL instance.

3. Automated coverage improvement: Scripts detect uncovered code and generate new tests.

4. Deduplication: Keeps only unique, compilable tests.

5. Easy DB reset: Rerun db/seed.sql to restore the DB state.




## 🧪 Test Workflow

```
| Step             | Command/Script               | Description                          |
| ---------------- | ---------------------------- | ------------------------------------ |
| Generate tests   | `python generate_tests.py`   | LLM creates new GoogleTest files     |
| Refine tests     | `python refine_tests.py`     | Deduplicate and clean up tests       |
| Improve coverage | `python improve_coverage.py` | Targets uncovered code for new tests |
| Run tests        | `ctest` or `./AllTests`      | Executes all C++ unit tests          |
| Coverage report  | `genhtml filtered.info ...`  | Generates HTML coverage summary      |

```


## 🧰 Testing Tools Used

```
| Tool       | Purpose                    |
| ---------- | -------------------------- |
| GoogleTest | C++ unit testing framework |
| lcov/gcovr | Code coverage analysis     |
| Python     | Test generation scripts    |
| PostgreSQL | Real DB for integration    |

```

## 📊 Coverage Report

![Screenshot 2025-07-07 at 8 53 28 AM](https://github.com/user-attachments/assets/3a6b5aed-3249-4722-807d-628ac7958554)



