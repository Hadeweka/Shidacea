#pragma once

#include <mruby.h>
#include <mruby/dump.h>
#include <mruby/variable.h>
#include <mruby/data.h>
#include <mruby/compile.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/string.h>
#include <cstring>
#include <string>
#include <filesystem>
#include <functional>

#include <SFML/Graphics.hpp>

using MrbIntRect = sf::Rect<mrb_int>;

//! Preprocessor shenanigans to switch between script file loading and pre-compiled bytecode
//! If debug mode is on (NDEBUG not defined), the macro will directly load the script file "scripts/XXX.rb"
//! If debug mode is off, the macro will execute the bytecode in the respective array compiled_ruby_XXX
//! In the latter case, don't forget to include "compiled_scripts/XXX.h" (but ONLY then), or else the array will not be defined
//! Sadly, there is no easy way to circumwent the conditioned include directive
#ifndef NDEBUG
#define MRB_LOAD_SCRIPT(mrb, name, path) MrbWrap::execute_script_file(mrb, #path ".rb")
#else
#define MRB_LOAD_SCRIPT(mrb, name, path) MrbWrap::execute_bytecode(mrb, compiled_ruby_##name)
#endif

//! If there are any scripts in the scripts folder of the release version, load them if DYNAMIC_LOADING is set
//! This way scripts can be loaded at runtime, e.g. for a precompiled engine

#ifndef NDEBUG
#define MRB_LOAD_SCRIPT_FOLDER(mrb, name, path) MrbWrap::load_all_scripts_recursively(mrb, #path)
#else
#ifdef DYNAMIC_LOADING
#define MRB_LOAD_SCRIPT_FOLDER(mrb, name, path) MrbWrap::execute_bytecode(mrb, compiled_ruby_##name); \
MrbWrap::load_all_scripts_recursively(mrb, #path)
#else
#define MRB_LOAD_SCRIPT_FOLDER(mrb, name, path) MrbWrap::execute_bytecode(mrb, compiled_ruby_##name)
#endif
#endif

//! TODO: Rewrite functions above and includes so that only the core and the custom scenes and entities need to be loaded
//! TODO: Do this by defining macros which may either load one single header or a folder with scripts recursively

#include <iostream>

//! Alias for the lamdba type used for mruby functions

#define MRUBY_FUNC [](mrb_state* mrb, mrb_value self) noexcept -> mrb_value


namespace MrbWrap {

	//! Will be defined below
	template <class T> mrb_value ruby_class_default_init_copy(mrb_state* mrb, mrb_value self);

	//! Some different ways to execute mruby code

	//! Execute a string containing mruby code
	//! Only useful for user-generated input
	//! If C++ code needs to execute ruby commands, don't use this
	void execute_string(mrb_state* mrb, std::string const& code);

	//! Ececute a mruby script file
	//! Easiest way to execute script files, but also the slowest
	void execute_script_file(mrb_state* mrb, std::string const& filename);

	//! Execute a file containing mruby bytecode
	//! WARNING: Does not work correctly for some reason, so this should NEVER be used!
	[[deprecated("May not work in many situations")]] void execute_bytecode_file(mrb_state* mrb, std::string const& filename);

	//! Execute embedded bytecode from a C array
	//! Fastest way to load a script, but needs some setup work
	void execute_bytecode(mrb_state* mrb, const uint8_t* symbol_array);

	//! Load mods
	void load_mods(mrb_state* mrb);

	//! Load all scripts in a given path
	void load_all_scripts_recursively(mrb_state* mrb, std::string path);

	//! Universal destructor wrapped into a C representation
	//! Will be used as callback for ruby object deallocation
	template <class T> static void free_data(mrb_state* mrb, void* object_ptr) {

		delete static_cast<T*>(object_ptr);

	}

	//! Create and returns a ruby class which can be used as a C++ class wrapper
	//! Use this when setting up a ruby class
	RClass* define_data_class(mrb_state* mrb, const char* name, RClass* super_class = nullptr);
	RClass* define_data_class_under(mrb_state* mrb, const char* name, RClass* ruby_module, RClass* super_class = nullptr);

	//! Define a new function for ruby
	void define_mruby_function(mrb_state* mrb, RClass* ruby_class, const char* name, mrb_value(*func)(mrb_state* mrb, mrb_value self) noexcept, mrb_aspec aspec = MRB_ARGS_NONE());

	//! Define a copy method automatically for any wrapped C++ object
	//! Use this when setting up a ruby class
	template <class T> void define_default_copy_init(mrb_state* mrb, RClass* ruby_class) {

		mrb_define_method(mrb, ruby_class, "initialize_copy", MrbWrap::ruby_class_default_init_copy<T>, MRB_ARGS_REQ(1));

	}

	//! Creates a direct ruby wrapper for any C++ object and stores it in 'self'
	//! Constructor arguments can be given as 'TArgs', if needed
	//! The name of the class T will be used as the argument for the ruby-internal name
	//! This may cause some weird names from certain compilers, but this shouldn't pose a problem
	//! DO NOT destroy the created object manually, the mruby garbage collector will do this for you!
	//! IMPORTANT: This works only with ruby classes specifically declared as data object
	//! Do this by using the function 'define_data_class' defined here
	//! Use this in callbacks
	template <class T, class ... TArgs> T* convert_to_object(mrb_state* mrb, mrb_value self, TArgs ... args) {

		auto new_object = new T(args...);

		static const mrb_data_type data_type = {

			typeid(T).name(), free_data<T>

		};

		DATA_PTR(self) = new_object;
		DATA_TYPE(self) = &data_type;

		return new_object;

	}

	//! Return the wrapped C++ object of class 'T' from 'self'
	//! This can be used to modify the internal properties of a wrapped C++ object
	//! Use this in callbacks
	template <class T> T* convert_from_object(mrb_state* mrb, mrb_value self) {

		auto type = DATA_TYPE(self);

		return static_cast<T*>(mrb_data_get_ptr(mrb, self, type));

	}

	//! Copy the content of the wrapped C++ structure to the new wrapper in 'self'
	//! Use this in callbacks
	template <class T> void copy_object(mrb_state* mrb, mrb_value self) {

		mrb_value original;

		mrb_get_args(mrb, "o", &original);

		auto old_value = MrbWrap::convert_from_object<T>(mrb, original);
		auto new_value = MrbWrap::convert_to_object<T>(mrb, self);

		*new_value = *old_value;

	}

	//! Default copy routine which can be used for trivial copy constructors
	//! Use this as a callback routine
	template <class T> mrb_value ruby_class_default_init_copy(mrb_state* mrb, mrb_value self) {

		MrbWrap::copy_object<T>(mrb, self);

		return self;
	}

	//! Creates a C++ instance of the class 'T' and wraps it directly into the ruby instance variable of the ruby object 'self'
	//! Use this in callbacks
	template <class T, class ... TArgs> T* convert_to_instance_variable(mrb_state* mrb, mrb_value self, const char* var_c_str, TArgs ... args) {

		auto new_object = new T(args...);

		static const struct mrb_data_type data_type = {

			typeid(T).name(), free_data<T>

		};

		//! mrb_intern_lit does not work here, because string lengths are pretty weird in mruby
		//! Also, the symbols do not need to be computed more than once, so this variable is made static (also see below)
		static auto symbol = mrb_intern_static(mrb, var_c_str, strlen(var_c_str));
		auto wrapper = Data_Wrap_Struct(mrb, mrb->object_class, &data_type, new_object);

		mrb_iv_set(mrb, self, symbol, mrb_obj_value(wrapper));

		return new_object;

	}

	//! Obtains a pointer to the C++ object of class 'T' back from the instance variable with name 'var_c_str' from the ruby object 'self'
	//! Use this in callbacks
	template <class T> T* convert_from_instance_variable(mrb_state* mrb, mrb_value self, const char* var_c_str) {

		static auto symbol = mrb_intern_static(mrb, var_c_str, strlen(var_c_str));
		mrb_iv_get(mrb, self, symbol);
		auto type = DATA_TYPE(mrb_iv_get(mrb, self, symbol));

		return static_cast<T*>(mrb_data_get_ptr(mrb, mrb_iv_get(mrb, self, symbol), type));

	}

	//! Will later be used for format strings

	constexpr char get_type_format_char(mrb_float) { return 'f'; }
	constexpr char get_type_format_char(mrb_int) { return 'i'; }
	constexpr char get_type_format_char(char*) { return 'z'; }
	constexpr char get_type_format_char(mrb_bool) { return 'b'; }
	constexpr char get_type_format_char(mrb_value) { return 'o'; }

	template <class T> mrb_value cast_value_to_ruby(mrb_state* mrb, T value);

	template <class T> constexpr std::string get_format_string(const T& argument) {

		std::string result;
		result += get_type_format_char(argument);
		return result;

	}

	template <class T, class ... TArgs> constexpr std::string get_format_string(const T& argument, const TArgs& ... args) {

		std::string result;
		result += get_type_format_char(argument);
		result += get_format_string(args...);
		return result;

	}

	//! TODO: Here is still much work to do.

	template <class C, class D, class TRuby, class TCpp, TCpp (D::*member)()> void define_function_with_no_args(mrb_state* mrb, RClass* ruby_class, const char* name) {

		MrbWrap::define_mruby_function(mrb, ruby_class, name, MRUBY_FUNC {

			auto content = MrbWrap::convert_from_object<C>(mrb, self);

			auto return_value = ((*content).*member)();

			return cast_value_to_ruby(mrb, static_cast<TRuby>(return_value));

		}, MRB_ARGS_NONE());

	}

	template <class C, class D, void (D::*member)()> void define_function_with_no_args(mrb_state* mrb, RClass* ruby_class, const char* name) {

		MrbWrap::define_mruby_function(mrb, ruby_class, name, MRUBY_FUNC {

			auto content = MrbWrap::convert_from_object<C>(mrb, self);

			((*content).*member)();

			return mrb_nil_value();

		}, MRB_ARGS_NONE());

	}

	template <class C, class TRuby, class TCpp, TCpp C::*member> void define_getter(mrb_state* mrb, RClass* ruby_class, const char* name) {

		MrbWrap::define_mruby_function(mrb, ruby_class, name, MRUBY_FUNC {

			auto content = MrbWrap::convert_from_object<C>(mrb, self);

			return cast_value_to_ruby(mrb, static_cast<TRuby>((*content).*member));

		}, MRB_ARGS_NONE());

	}

	template <class C, class TRuby, class TCpp, TCpp C::*member> void define_setter(mrb_state* mrb, RClass* ruby_class, const char* name) {

		MrbWrap::define_mruby_function(mrb, ruby_class, name, MRUBY_FUNC {

			TRuby new_value;

			mrb_get_args(mrb, get_format_string(new_value).c_str(), &new_value);

			auto content = MrbWrap::convert_from_object<C>(mrb, self);
			(*content).*member = static_cast<TCpp>(new_value);

			return cast_value_to_ruby(mrb, new_value);

		}, MRB_ARGS_REQ(1));

	}

}
