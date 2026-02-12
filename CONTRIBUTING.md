# Contributing to SOXIM

Thank you for your interest in contributing to SOXIM! We welcome contributions from everyone.

## Table of Contents

1. [Getting Started](#getting-started)
2. [Development Workflow](#development-workflow)
3. [Code Style](#code-style)
4. [Testing](#testing)
5. [Documentation](#documentation)
6. [Pull Request Process](#pull-request-process)
7. [Code of Conduct](#code-of-conduct)

## Getting Started

### Prerequisites

- C++17 compiler (GCC 7+ or Clang 5+)
- CMake 3.14+
- Python 3.8+
- Git

### Setting Up Development Environment

1. Fork the repository
2. Clone your fork:
   ```bash
   git clone https://github.com/your-username/soxim.git
   cd soxim
   ```

3. Install dependencies:
   ```bash
   # C++ dependencies (Ubuntu/Debian)
   sudo apt-get install build-essential cmake

   # Python dependencies
   pip install -r scripts/requirements.txt
   ```

4. Build the project:
   ```bash
   mkdir build && cd build
   cmake .. -DBUILD_TESTS=ON
   make -j$(nproc)
   ```

5. Run tests:
   ```bash
   cd tests/unit
   ./test_data_structures
   ./test_clock
   # ... run other tests
   ```

## Development Workflow

### Branch Naming

Use descriptive branch names:
- `feature/<feature-name>` - New features
- `bugfix/<bug-description>` - Bug fixes
- `docs/<documentation-topic>` - Documentation updates
- `refactor/<refactoring-topic>` - Code refactoring
- `test/<test-topic>` - Test additions

### Commit Messages

Follow conventional commits format:
```
<type>(<scope>): <description>

<body>

<footer>
```

Types:
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, semicolons, etc.)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `chore`: Build process, CI/CD, tooling changes

Example:
```
feat(routing): add ROMM routing algorithm

Implemented Randomized Oblivious Multi-phase Minimal (ROMM) routing
algorithm for mesh networks. This provides better load balancing
compared to DOR.

Closes #123
```

### Code Style

#### C++ Style Guide

1. **Naming Conventions**
   - Classes: `PascalCase` (e.g., `Router`, `TrafficOperator`)
   - Functions: `camelCase` (e.g., `runOneCycle`, `generateTraffic`)
   - Variables: `camelCase` (e.g., `packetSize`, `injectionRate`)
   - Constants: `UPPER_SNAKE_CASE` (e.g., `MAX_BUFFER_SIZE`)
   - Private members: `m_` prefix (e.g., `m_routerID`)

2. **Formatting**
   - Use 4 spaces for indentation
   - Line length: 120 characters
   - Use `.clang-format` configuration
   - Format before committing:
     ```bash
     clang-format -i src/*.cpp src/*.h tests/unit/*.cpp
     ```

3. **Braces**
   ```cpp
   // K&R style
   if (condition) {
       // code
   } else {
       // code
   }
   ```

4. **Pointers and References**
   ```cpp
   // Pointers: type* name
   Router* router;
   
   // References: type& name
   Port& port;
   ```

5. **Include Order**
   ```cpp
   // 1. System includes
   #include <vector>
   #include <string>
   
   // 2. Project includes
   #include "Router.h"
   #include "DataStructures.h"
   ```

#### Python Style Guide

1. **PEP 8 Compliance**
   - Use `black` formatter
   - Line length: 120 characters
   - Use `flake8` for linting

2. **Naming Conventions**
   - Functions: `snake_case` (e.g., `generate_traffic`)
   - Variables: `snake_case` (e.g., `injection_rate`)
   - Classes: `PascalCase` (e.g., `TrafficOperator`)

3. **Docstrings**
   ```python
   def function_name(param1, param2):
       """
       Brief description.
       
       Detailed description.
       
       Args:
           param1: Description
           param2: Description
           
       Returns:
           Description of return value
           
       Raises:
           ExceptionType: Description
       """
   ```

## Testing

### Unit Tests

1. **Location**: `tests/unit/`
2. **Framework**: Google Test
3. **Naming**: `test_<component>.cpp`

### Adding New Tests

1. Create test file in `tests/unit/`
2. Add to `tests/unit/CMakeLists.txt`
3. Run tests locally:
   ```bash
   cd build
   make test_<name>
   cd tests/unit
   ./test_<name>
   ```

### Test Coverage

- Aim for 80%+ code coverage
- Coverage reports generated for Debug builds
- Use `lcov` for C++ coverage

### Integration Tests

- Located in `tests/integration/`
- Test end-to-end functionality
- Run with `./scripts/run_tests.sh`

## Documentation

### C++ Documentation

Use Doxygen-style comments:
```cpp
/**
 * @brief Brief description
 * 
 * Detailed description
 * 
 * @param param1 Description
 * @param param2 Description
 * @return Description
 */
```

### Python Documentation

Use Google-style docstrings (see above).

### User Documentation

- Update `README.md` for user-facing changes
- Update `docs/` for API changes
- Add examples for new features

## Pull Request Process

1. **Create Branch**
   ```bash
   git checkout -b feature/my-new-feature
   ```

2. **Make Changes**
   - Follow code style guidelines
   - Add tests for new functionality
   - Update documentation

3. **Run Tests**
   ```bash
   cd build
   make -j$(nproc)
   cd tests/unit
   for test in test_*; do ./$test; done
   ```

4. **Format Code**
   ```bash
   clang-format -i src/*.cpp src/*.h tests/unit/*.cpp
   black scripts/
   ```

5. **Commit Changes**
   ```bash
   git add .
   git commit -m "feat: add my new feature"
   ```

6. **Push to Fork**
   ```bash
   git push origin feature/my-new-feature
   ```

7. **Create Pull Request**
   - Go to GitHub repository
   - Click "Compare & pull request"
   - Fill out PR template
   - Request review

8. **Address Review Comments**
   - Make requested changes
   - Update PR with new commits
   - Resolve conversations

9. **Merge**
   - Once approved and CI passes
   - Use "Squash and merge" for clean history

## Code of Conduct

### Our Pledge

We pledge to foster an open and welcoming environment. We do not tolerate harassment or discrimination.

### Our Standards

**Positive Behavior:**
- Using welcoming and inclusive language
- Being respectful of differing viewpoints
- Gracefully accepting constructive criticism
- Focusing on what is best for the community

**Unacceptable Behavior:**
- Harassment or discrimination
- Trolling or insulting comments
- Unwelcome sexual attention
- Publishing others' private information

### Enforcement

Report violations to project maintainers. Violations may result in:
- Temporary ban from the community
- Permanent removal from the project

## Getting Help

- **Issues**: Use GitHub Issues for bugs and feature requests
- **Discussions**: Use GitHub Discussions for questions
- **Email**: Contact maintainers for private matters

## Recognition

Contributors will be recognized in:
- Release notes
- Contributors list in README
- Project documentation

## License

By contributing, you agree that your contributions will be licensed under the project's license (MIT License).
