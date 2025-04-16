#!/bin/bash

set -o errexit
set -o nounset

# Check if pipefail is supported (e.g. not on PowerShell)
(set -o pipefail) 2>/dev/null && set -o pipefail || true

# 💡 Set working directory to repo root
REPO_ROOT=$(cd "$(dirname "$0")/.." && pwd)
cd "$REPO_ROOT"

echo "📁 Preparing logs directory..."
mkdir -p build/logs || { echo "❌ Failed to create logs directory"; exit 1; } # Ensure logs directory exists

# 📦 Enable coverage flags (optional if already in CMakeLists)
export CXXFLAGS="--coverage"
export LDFLAGS="--coverage"

echo "📦 Moving to build directory..."
cd build

echo "🧪 Running Unit Tests..."
ctest -R "Unit" --output-on-failure | tee logs/unit_test.log || echo "⚠️ Unit tests failed"

echo "🔌 Running Integration Tests..."
ctest -R "Integration" --output-on-failure | tee logs/integration_test.log || echo "⚠️ Integration tests failed"

# 📦 Combine test logs into one file
echo "📦 Combining test logs..."
cat logs/*.log > logs/test.log || true


echo "⚙️ Checking for .gcno/.gcda files..."
find . -name "*.gcno" -or -name "*.gcda" || echo "❌ No coverage data files found!"


# 📝 Collect code coverage data
echo "📊 Collecting coverage data..."
# llvm-cov gcovr --root . --xml --output build/logs/coverage.xml  # make sure to use the correct path for your project
gcovr --root . --xml --output build/logs/coverage.xml

# ⬅️ return repo root to execute gcovr follow context
cd "$REPO_ROOT"

# 📦 Check if coverage data was generated
if [ -f "build/logs/coverage.xml" ]; then
  echo "✅ Coverage XML file created successfully."
else
  echo "❌ Failed to create coverage XML file."
  exit 1
fi

echo "📊 Verifying content of coverage.xml..."
if [ -s "build/logs/coverage.xml" ]; then
  echo "✅ coverage.xml has content."
else
  echo "❌ coverage.xml is empty."
  exit 1
fi


# 📦 Generate coverage HTML report (Optional)
genhtml build/logs/coverage.info --output-directory build/logs/coverage_html

echo "✅ All tests executed and logs saved."
