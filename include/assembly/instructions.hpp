#pragma once

#include "../architecture/vm_params.hpp"

#include <string>



namespace jftt::assembly::instruction {

class instruction_interface {
public:
    virtual ~instruction_interface() = default;
    virtual std::string str() const noexcept = 0;
};


class read : public instruction_interface {
public:
    read() = default;

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction;
    }

private:
    inline static const std::string _instruction{"READ"};
};

class write : public instruction_interface {
public:
    write() = default;

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction;
    }

private:
    inline static const std::string _instruction{"WRITE"};
};


class load : public instruction_interface {
public:
    load(const jftt::architecture::vm_register_discriminator rd)
    : _register(rd) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + jftt::architecture::as_string(this->_register);
    }

private:
    inline static const std::string _instruction{"LOAD "};
    const jftt::architecture::vm_register_discriminator _register;
};

class store : public instruction_interface {
public:
    store(const jftt::architecture::vm_register_discriminator rd)
    : _register(rd) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + jftt::architecture::as_string(this->_register);
    }

private:
    inline static const std::string _instruction{"STORE "};
    const jftt::architecture::vm_register_discriminator _register;
};


class add : public instruction_interface {
public:
    add(const jftt::architecture::vm_register_discriminator rd)
    : _register(rd) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + jftt::architecture::as_string(this->_register);
    }

private:
    inline static const std::string _instruction{"ADD "};
    const jftt::architecture::vm_register_discriminator _register;
};

class sub : public instruction_interface {
public:
    sub(const jftt::architecture::vm_register_discriminator rd)
    : _register(rd) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + jftt::architecture::as_string(this->_register);
    }

private:
    inline static const std::string _instruction{"SUB "};
    const jftt::architecture::vm_register_discriminator _register;
};

class get : public instruction_interface {
public:
    get(const jftt::architecture::vm_register_discriminator rd)
    : _register(rd) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + jftt::architecture::as_string(this->_register);
    }

private:
    inline static const std::string _instruction{"GET "};
    const jftt::architecture::vm_register_discriminator _register;
};

class put : public instruction_interface {
public:
    put(const jftt::architecture::vm_register_discriminator rd)
    : _register(rd) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + jftt::architecture::as_string(this->_register);
    }

private:
    inline static const std::string _instruction{"PUT "};
    const jftt::architecture::vm_register_discriminator _register;
};

class rst : public instruction_interface {
public:
    rst(const jftt::architecture::vm_register_discriminator rd)
    : _register(rd) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + jftt::architecture::as_string(this->_register);
    }

private:
    inline static const std::string _instruction{"RST "};
    const jftt::architecture::vm_register_discriminator _register;
};

class inc : public instruction_interface {
public:
    inc(const jftt::architecture::vm_register_discriminator rd)
    : _register(rd) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + jftt::architecture::as_string(this->_register);
    }

private:
    inline static const std::string _instruction{"INC "};
    const jftt::architecture::vm_register_discriminator _register;
};

class dec : public instruction_interface {
public:
    dec(const jftt::architecture::vm_register_discriminator rd)
    : _register(rd) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + jftt::architecture::as_string(this->_register);
    }

private:
    inline static const std::string _instruction{"DEC "};
    const jftt::architecture::vm_register_discriminator _register;
};

class shl : public instruction_interface {
public:
    shl(const jftt::architecture::vm_register_discriminator rd)
    : _register(rd) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + jftt::architecture::as_string(this->_register);
    }

private:
    inline static const std::string _instruction{"SHL "};
    const jftt::architecture::vm_register_discriminator _register;
};

class shr : public instruction_interface {
public:
    shr(const jftt::architecture::vm_register_discriminator rd)
    : _register(rd) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + jftt::architecture::as_string(this->_register);
    }

private:
    inline static const std::string _instruction{"SHR "};
    const jftt::architecture::vm_register_discriminator _register;
};


class jump : public instruction_interface {
public:
    jump(const jftt::architecture::instruction_counter_type ino)
    : _instruction_no(ino) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + std::to_string(this->_instruction_no);
    }

private:
    inline static const std::string _instruction{"JUMP "};
    const jftt::architecture::instruction_counter_type _instruction_no;
};

class jpos : public instruction_interface {
public:
    jpos(const jftt::architecture::instruction_counter_type ino)
    : _instruction_no(ino) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + std::to_string(this->_instruction_no);
    }

private:
    inline static const std::string _instruction{"JPOS "};
    const jftt::architecture::instruction_counter_type _instruction_no;
};

class jzero : public instruction_interface {
public:
    jzero(const jftt::architecture::instruction_counter_type ino)
    : _instruction_no(ino) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + std::to_string(this->_instruction_no);
    }

private:
    inline static const std::string _instruction{"JZERO "};
    const jftt::architecture::instruction_counter_type _instruction_no;
};


class strk : public instruction_interface {
public:
    strk(const jftt::architecture::vm_register_discriminator rd)
    : _register(rd) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + jftt::architecture::as_string(this->_register);
    }

private:
    inline static const std::string _instruction{"STRK "};
    const jftt::architecture::vm_register_discriminator _register;
};

class jumpr : public instruction_interface {
public:
    jumpr(const jftt::architecture::vm_register_discriminator rd)
    : _register(rd) {}

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction + jftt::architecture::as_string(this->_register);
    }

private:
    inline static const std::string _instruction{"JUMPR "};
    const jftt::architecture::vm_register_discriminator _register;
};


class halt : public instruction_interface {
public:
    halt() = default;

    [[nodiscard]] std::string str() const noexcept override {
        return this->_instruction;
    }

private:
    inline static const std::string _instruction{"HALT"};
};

} // namespace jftt::assembly::instruction
