#ifndef __M19_SEMANTICS_SYMBOL_H__
#define __M19_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>

namespace m19 {

  class symbol {
    basic_type *_type;
    std::string _name;
    int _qualifier;
    bool _initialized;
    int _offset = 0;
    int _value; // hack!

  public:
    symbol(basic_type *type, const std::string &name, int qualifier) :
        _type(type), _name(name), _qualifier(qualifier){
    }

    symbol(basic_type *type, const std::string &name, int qualifier, bool initialized, int value) :
        _type(type), _name(name), _qualifier(qualifier), _initialized(initialized), _value(0) {
    }

    virtual ~symbol() {
      delete _type;
    }

    basic_type *type() const {
      return _type;
    }
    const std::string &name() const {
      return _name;
    }
    int qualifier() const {
      return _qualifier;
    }
    int offset() const {
      return _offset;
    }
    void set_offset(int offset) {
      _offset = offset;
    }
    int value() const {
      return _value;
    }
    int value(long v) {
      return _value = v;
    }
    
  };

} // m19

#endif
