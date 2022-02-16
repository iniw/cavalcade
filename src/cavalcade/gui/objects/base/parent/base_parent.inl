#pragma once

template<typename T, typename... VA>
T* gui::objects::base_parent::add(VA&&... args) {
	auto child = std::make_unique<T>(std::forward<VA>(args)...);

	child->identify(this);

	child->init();

	// store the ptr before moving
	auto ptr = child.get();

	m_children_mgr.add(std::move(child));

	on_add_child(ptr);

	return ptr;
}