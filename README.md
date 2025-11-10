# Alienworlds contracts-common

A collection of common C++ utilities and patterns for EOSIO smart contract development used by the alienworlds and eosdac contracts. This library provides safe mathematical operations, singleton table patterns, string formatting, and various utility functions to make EOSIO smart contract development safer and more efficient.

## Features

### Safe Math Operations (`safemath.hpp`)
- Template class `S<T>` for safe mathematical operations
- Overflow and underflow checks for arithmetic operations
- Support for various numeric types (integers and floating-point)
- Type-safe conversions between different numeric types
- Helpful error messages when operations would result in invalid states

### Singleton Pattern (`singleton.hpp`)
- Enhanced version of EOSIO's singleton pattern
- Type-safe key-value storage with variant support
- Automatic state management and persistence
- Convenient property macros for getter/setter generation
- Support for optional values

### String Formatting (`string_format.hpp`)
- Type-safe string formatting utilities
- Support for EOSIO-specific types in string conversions
- String splitting functionality
- Polymorphic toString implementation for common types

### Error Handling (`serr.hpp`)
- Thread-safe error message management
- Support for format strings in error messages
- RAII-style error scope management
- Integration with EOSIO's check system

### Utility Functions (`util.hpp`)
- Enhanced version of `eosio::check` with format string support
- ID combination utilities for complex indices
- Table upsert helpers
- Time utilities
- Extended symbol handling

## Requirements

- EOSIO.CDT 1.7.0 or higher
- C++17 or higher

## Installation

This is a header-only library. To use it in your EOSIO smart contract project:

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/contracts-common.git
   ```

2. Include the headers in your smart contract:
   ```cpp
   #include "contracts-common/safemath.hpp"
   #include "contracts-common/singleton.hpp"
   #include "contracts-common/string_format.hpp"
   #include "contracts-common/util.hpp"
   ```

## Usage Examples

### Safe Math Operations
```cpp
#include "contracts-common/safemath.hpp"

void example() {
    S<uint64_t> a{100};
    S<uint64_t> b{50};
    
    // Safe arithmetic operations
    auto sum = a + b;  // 150
    auto diff = a - b; // 50
    auto prod = a * b; // 5000
    
    // Will throw if operation would overflow/underflow
    check(sum <= S<uint64_t>::max(), "Sum would overflow");
}
```

### Singleton Pattern
```cpp
#include "contracts-common/singleton.hpp"

SINGLETON(config, mycontract,
    PROPERTY(uint64_t, counter)
    PROPERTY_OPTIONAL_TYPECASTING(asset, int64_t, balance)
)

class mycontract {
    config _config;
    
    void action() {
        _config.set_counter(123);
        auto counter = _config.get_counter(); // 123
        
        _config.set_balance(asset{1000, symbol{"EOS", 4}});
        auto balance = _config.maybe_get_balance(); // optional<asset>
    }
};
```

### String Formatting
```cpp
#include "contracts-common/string_format.hpp"

void example() {
    auto msg = fmt("Value: %s, Symbol: %s", 123, symbol{"EOS", 4});
    check(false, "Error occurred: %s", msg);
}
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details. 