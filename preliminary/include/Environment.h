#pragma once

#include <vector>
#include <unordered_map>
using namespace std;

// Environment use for simulate scope when evaluate
template<typename T>
class Environment {
public:
	Environment() : cur_scope_(0) {
		scopes_.push_back(unordered_map<string, T>());
	}

	// Check if var exists
	bool is_var_exist(string var_name);
	// Find var in scopes
	T lookup_var(string var_name);
	// Add a new var in current scope
	bool add_var(string var_name);
	// Update the var
	bool update_var(string var_name, T value);

	// Create a new scope when get in a new block
	bool getin_new_scope();
	// Delete a scope when get out the block
	bool getout_a_scope();
private:
	vector<unordered_map<string, T>> scopes_;

	// Helper method to recursion of checking if var exists
	bool is_var_exist(string var_name, int scope_layer);
	// Helper method to recursion of looking up var
	T lookup_var(string var_name, int scope_layer);
	// Helper method to recursion of update var
	bool update_var(string var_name, T value, int scope_layer);
	int cur_scope_;
};




template<typename T>
bool Environment<T>::getin_new_scope() {
	scopes_.push_back(unordered_map<string, T>());
	cur_scope_ += 1;
	return true;
}

template<typename T>
bool Environment<T>::getout_a_scope() {
	if (cur_scope_ == 0) {
		return false;
	}
	scopes_.erase(scopes_.begin() + cur_scope_);
	cur_scope_ -= 1;
	return true;
}

template<typename T>
bool Environment<T>::is_var_exist(string var_name) {
	return is_var_exist(var_name, cur_scope_);
}

template<typename T>
bool Environment<T>::is_var_exist(string var_name, int scope_layer) {
	if (scope_layer < 0) {
		return false;
	}
	if (scopes_[scope_layer].count(var_name)) {
		return true;
	}
	return is_var_exist(var_name, scope_layer - 1);
}

template<typename T>
T Environment<T>::lookup_var(string var_name) {
	return lookup_var(var_name, cur_scope_);
}

template<typename T>
T Environment<T>::lookup_var(string var_name, int scope_layer) {
	if (scope_layer < 0) {
		return T();
	}
	auto ret = scopes_[scope_layer].find(var_name);
	if (ret != scopes_[scope_layer].end()) {
		return ret->second;
	}
	return lookup_var(var_name, scope_layer - 1);
}

template<typename T>
bool Environment<T>::add_var(string var_name) {
	scopes_[cur_scope_][var_name] = 0;
	return true;
}

template<typename T>
bool Environment<T>::update_var(string var_name, T value) {
	return update_var(var_name, value, cur_scope_);
}

template<typename T>
bool Environment<T>::update_var(string var_name, T value, int scope_layer) {
	if (scope_layer < 0) {
		return false;
	}
	if (scopes_[scope_layer].count(var_name)) {
		scopes_[scope_layer][var_name] = value;
		return true;
	}
	return update_var(var_name, value, scope_layer - 1);
}