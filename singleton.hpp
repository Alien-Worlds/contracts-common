#include <eosio/eosio.hpp>
#include <eosio/multi_index.hpp>
#include <eosio/permission.hpp>
#include <eosio/singleton.hpp>
#include <eosio/time.hpp>

/**
 * simple getter/setter
 **/
#define PROPERTY(type, name)                                                                                           \
    type get_##name() const {                                                                                          \
        return get<type>(#name);                                                                                       \
    }                                                                                                                  \
    auto set_##name(const type &value) {                                                                               \
        return set(#name, value);                                                                                      \
    }

/**
 * A slightly more complicated getter/setter macro that allows optional values.
 * If value is not set, it returns a null optional. To unset a previously set
 * value, it has an unset function.
 * Since our variant can only hold certain number types, we sometimes need to
 * convert our desired type to a storage_type. Before storing, it will convert
 * the value to the storage_type and before returning, the getter will
 * automatically convert back to type.
 **/
#define PROPERTY_OPTIONAL_TYPECASTING(type, storage_type, name)                                                        \
    std::optional<type> maybe_get_##name() const {                                                                     \
        return get_maybe<storage_type>(#name);                                                                         \
    }                                                                                                                  \
    void set_##name(const type &value) {                                                                               \
        set(#name, static_cast<storage_type>(value));                                                                  \
    }                                                                                                                  \
    void unset_##name() {                                                                                              \
        unset(#name);                                                                                                  \
    }

#define SINGLETON(table_name, contract_name, ...)                                                                      \
    struct table_name##_struct;                                                                                        \
    using table_name##_singleton = eosio::singleton<#table_name##_n, table_name##_struct>;                             \
    struct [[eosio::table(#table_name), eosio::contract(#contract_name)]] table_name##_struct {                        \
        std::map<std::string, state_value_variant> data = {};                                                          \
    };                                                                                                                 \
    struct table_name : Singleton<table_name##_singleton, table_name##_struct> {                                       \
        using Singleton::Singleton;                                                                                    \
        __VA_ARGS__                                                                                                    \
    };

using state_value_variant = std::variant<int8_t, uint8_t, int32_t, uint32_t, int64_t, uint64_t, int128_t, uint128_t,
    bool, std::vector<int64_t>, eosio::name, std::string, eosio::time_point_sec, eosio::asset, eosio::extended_asset>;

template <typename Table, typename Struct>
struct Singleton {
  private:
    eosio::name contract;
    uint64_t    scope;
    Table       table;
    Struct      row;
    bool        dirty = false;

  public:
    explicit Singleton(const eosio::name &c, const eosio::name &s)
        : contract(c), scope(s.value), table(c, scope), row(table.get_or_default()) {}

    explicit Singleton(const eosio::name &c, const uint64_t &s)
        : contract(c), scope(s), table(c, scope), row(table.get_or_default()) {}

    ~Singleton() {
        if (dirty) {
            save();
        }
    }

  protected:
    void save() {
        table.set(row, contract);
    };

  public:
    auto set(const std::string &key, const state_value_variant &value) {
        dirty = true;
        return row.data.insert_or_assign(key, value);
    }

    void unset(const std::string &key) {
        dirty             = true;
        const auto search = row.data.find(key);
        check(search != row.data.end(), "Cannot unset %s, no value set", key);
        row.data.erase(key);
    }

    template <typename T>
    T get(const std::string &key) const {
        const auto search = row.data.find(key);
        if (search != row.data.end()) {
            return std::get<T>(search->second);
        } else {
            return T{};
        }
    }

    template <typename T>
    std::optional<T> get_maybe(const std::string &key) const {
        const auto search = row.data.find(key);
        if (search != row.data.end()) {
            return std::get<T>(search->second);
        } else {
            return {};
        }
    }
};
