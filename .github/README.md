# CI/CD Pipeline Documentation

This document describes the CI/CD pipeline for the SOXIM project.

## Overview

The CI/CD pipeline is implemented using GitHub Actions and includes the following stages:

1. **Build and Test** - Compile and run unit tests
2. **Python Tests** - Validate Python scripts
3. **Documentation** - Generate documentation
4. **Code Quality** - Linting and static analysis
5. **Benchmark** - Performance testing
6. **Release** - Create release packages
7. **Docker** - Build and publish Docker images
8. **Security Scan** - Security vulnerability scanning

## Pipeline Stages

### 1. Build and Test

Runs on multiple configurations:
- Compilers: GCC and Clang
- Build types: Release and Debug

**Steps:**
- Install dependencies (CMake, compilers, Python)
- Configure CMake with tests enabled
- Build the project
- Run all unit tests
- Generate coverage report (Debug builds only)
- Upload test results and coverage

**Coverage Reporting:**
- Uses `lcov` for C++ coverage
- Uploads to Codecov for visualization
- Coverage reports are generated for Debug builds only

### 2. Python Tests

Validates all Python scripts in the `scripts/` directory.

**Steps:**
- Install Python dependencies
- Run linting with `flake8` and `black`
- Test module imports
- Validate script help messages

**Tools:**
- `flake8` - Code style checking
- `black` - Code formatting
- `pytest` - Unit testing (if tests are added)

### 3. Documentation

Generates API documentation using Doxygen.

**Steps:**
- Install Doxygen and Graphviz
- Generate documentation from source code
- Upload documentation as artifact

**Output:**
- HTML documentation in `docs/html/`
- Can be deployed to GitHub Pages

### 4. Code Quality

Ensures code quality standards are met.

**Steps:**
- Check C++ formatting with `clang-format`
- Run static analysis with `cppcheck`
- Check for TODO comments

**Requirements:**
- All C++ code must be formatted according to `.clang-format`
- No TODO comments allowed in production code
- Static analysis must pass

### 5. Benchmark

Runs performance benchmarks.

**Steps:**
- Build in Release mode
- Run benchmark suite
- Upload results

**Note:** Currently runs basic smoke tests. Full benchmark suite can be added.

### 6. Release

Creates GitHub releases when tags are pushed.

**Trigger:**
- Push to `refs/tags/v*`

**Steps:**
- Build in Release mode
- Create release packages
- Upload to GitHub Releases

**Packages:**
- `soxim-release.tar.gz` - Main executable
- `soxim-tests.tar.gz` - Test suite

### 7. Docker

Builds and publishes Docker images.

**Steps:**
- Build Docker image
- Test Docker image
- Push to Docker Hub (on main branch)

**Environment Variables:**
- `DOCKER_USERNAME` - Docker Hub username (secret)
- `DOCKER_PASSWORD` - Docker Hub password (secret)

### 8. Security Scan

Scans for security vulnerabilities.

**Tools:**
- `bandit` - Python security scanner
- `trivy` - Container vulnerability scanner

**Reports:**
- JSON reports uploaded as artifacts
- Non-blocking (allows pipeline to continue)

## Configuration Files

### GitHub Actions Workflows

- `.github/workflows/ci.yml` - Main CI/CD pipeline

### Docker

- `Dockerfile` - Container image definition

### Documentation

- `docs/Doxyfile` - Doxygen configuration

## Secrets Required

The following secrets must be configured in GitHub repository settings:

| Secret | Description |
|--------|-------------|
| `DOCKER_USERNAME` | Docker Hub username |
| `DOCKER_PASSWORD` | Docker Hub password |
| `CODECOV_TOKEN` | Codecov token (optional) |

## Usage

### Running Locally

You can run the CI/CD pipeline locally using `act`:

```bash
# Install act
curl https://raw.githubusercontent.com/nektos/act/master/install.sh | sudo bash

# Run workflow
act -j build-and-test
```

### Manual Trigger

To manually trigger a workflow:

1. Go to GitHub repository
2. Click on "Actions" tab
3. Select the workflow
4. Click "Run workflow"

### Branch Protection

Configure branch protection rules to require:

- All CI checks to pass
- Code review for pull requests
- Coverage threshold (e.g., 80%)

## Performance Metrics

The pipeline tracks the following metrics:

- **Build time**: ~5-10 minutes
- **Test time**: ~2-5 minutes
- **Total pipeline time**: ~15-20 minutes

## Troubleshooting

### Build Failures

1. Check compiler version compatibility
2. Verify CMake configuration
3. Check for missing dependencies

### Test Failures

1. Review test output in artifacts
2. Check for flaky tests
3. Verify test environment

### Coverage Issues

1. Ensure Debug build type is used
2. Check that tests are instrumented
3. Verify lcov configuration

### Docker Issues

1. Check Dockerfile syntax
2. Verify Docker Hub credentials
3. Check for base image availability

## Future Enhancements

- [ ] Add performance regression testing
- [ ] Implement visual regression tests
- [ ] Add fuzzing tests
- [ ] Integrate with cloud testing services
- [ ] Add GPU acceleration support
- [ ] Implement distributed testing

## Contact

For CI/CD issues or questions:
- Create an issue in the repository
- Contact the maintainers
- Check GitHub Actions documentation

## References

- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [Codecov Documentation](https://docs.codecov.io)
- [Docker Documentation](https://docs.docker.com)
- [Doxygen Documentation](https://www.doxygen.nl)
