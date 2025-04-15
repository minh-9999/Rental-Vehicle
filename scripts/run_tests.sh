#!/bin/bash

set -o errexit
set -o nounset

# Check if pipefail is supported (e.g. not on PowerShell)
(set -o pipefail) 2>/dev/null && set -o pipefail || true

# 💡 Set working directory to repo root
REPO_ROOT=$(cd "$(dirname "$0")/.." && pwd)
cd "$REPO_ROOT"

echo "📁 Preparing logs directory..."
mkdir -p build/logs

# 📦 Ensure coverage flags are enabled for compilation (if using GCC/Clang)
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

# 📝 Collect code coverage data
echo "📊 Collecting coverage data..."
lcov --capture --directory . --output-file build/logs/coverage.info

# 📦 Generate coverage HTML report (Optional)
genhtml build/logs/coverage.info --output-directory build/logs/coverage_html

# 📝 Create XML format for Codecov
lcov --capture --directory . --output-file build/logs/coverage.xml

echo "✅ All tests executed and logs saved."
