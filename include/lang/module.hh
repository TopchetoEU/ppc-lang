#include <set>
#include <unordered_map>
#include "lang/common.hh"

namespace ppc::lang {
    struct type_t {
        namespace_name_t name;
        size_t ptr_n;
    };

    struct statement_t {
    private:
        enum kind_t {
            CALL,
            STACK,
            RETURN,
        } kind;
        union val_t {
            namespace_name_t *call;
            int64_t stack;
        } val;

        ~statement_t();

        statement_t(kind_t kind, val_t val) {
            this->kind = kind;
            this->val = val;
        }
        statement_t(kind_t kind) {
            this->kind = kind;
        }

    public:
        bool is_call() const { return kind == CALL; }
        bool is_stack() const { return kind == STACK; }
        bool is_return() const { return kind == RETURN; }

        auto &call() const {
            if (!is_call()) throw (std::string)"Statement is not a call.";
            return val.call;
        }
        auto stack() const {
            if (!is_call()) throw (std::string)"Statement is not a stack.";
            return val.stack;
        }

        static statement_t call(const namespace_name_t &func);
        static statement_t stack(int64_t stack);
        static statement_t ret();
    };

    struct field_t {
        type_t type;
    };
    struct struct_t {
        std::unordered_map<std::string, type_t> fields;
    };
    struct function_t {
        std::unordered_map<std::string, type_t> args;
        type_t type;

        std::string get_signature();
    };

    struct definition_t {
    private:
        enum {
            FUNCTION,
            STRUCT,
            FIELD,
        } kind;
        union {
            field_t *field;
            struct_t *str;
            function_t *func;
        } val;

    public:
        ~definition_t();
        definition_t(field_t val);
        definition_t(struct_t val);
        definition_t(function_t val);
        definition_t(const definition_t &other);

        bool is_func() const { return kind == FUNCTION; }
        bool is_struct() const { return kind == STRUCT; }
        bool is_field() const { return kind == FIELD; }

        function_t &get_func();
        struct_t &get_struct();
        field_t &get_field();

        const function_t &get_func() const { return ((definition_t&)*this).get_func(); }
        const struct_t &get_struct() const { return ((definition_t&)*this).get_struct(); }
        const field_t &get_field() const { return ((definition_t&)*this).get_field(); }
    };

    struct module_t {
    private:
        using fields_t = std::unordered_map<namespace_name_t, field_t>;
        using structs_t = std::unordered_map<namespace_name_t, struct_t>;
        using funcs_t = std::unordered_map<namespace_name_t, function_t>;
        struct resolve_res_t {
            namespace_name_t name;
            definition_t def;
        };
    public:
        fields_t fields;
        structs_t structs;
        funcs_t funcs;

        const definition_t &def(namespace_name_t name);
        void add_def(namespace_name_t name, const definition_t &def) {
            if (def.is_field()) fields.emplace(name, def.get_field());
            if (def.is_func()) funcs.emplace(name, def.get_func());
            if (def.is_struct()) structs.emplace(name, def.get_struct());
        }
        bool exists(namespace_name_t name) {
            return
                fields.find(name) != fields.end() ||
                structs.find(name) != structs.end() ||
                funcs.find(name) != funcs.end();
        }
    };

    template <class T>
    struct resolve_res_t {
        namespace_name_t name;
        T value;
    };


    bool resolve_name(
        const std::vector<namespace_name_t> &names, const std::set<namespace_name_t> &imports,
        const namespace_name_t &name, namespace_name_t &res
    );

    template <class MapT>
    bool resolve_name_map(
        const MapT &defs, const std::set<namespace_name_t> &imports,
        const namespace_name_t &name, namespace_name_t &res
    ) {
        std::vector<namespace_name_t> names;
        for (auto &it : defs) {
            const namespace_name_t &val = it.first;
            names.push_back(val);
        }
        return resolve_name(names, imports, name, res);
    }

    template <class MapT>
    bool resolve(
        const MapT &defs, const std::set<namespace_name_t> &imports,
        const namespace_name_t &name, typename MapT::iterator &res
    ) {
        if (resolve_name_map(defs, imports, name, res.name)) {
            res.value = defs.find(res.name)->second;
            return true;
        }
        return false;
    }
}