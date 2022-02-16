#pragma once

namespace gui::helpers {
	// stores a list and some information about children of the same type in a per-parent basis
	template<typename T>
	struct children_info {
	protected:
		// helper struct, used by the s_info variable
		struct info {
			// the list of the children
			std::vector<T*> list;

			// the id of the currently active object
			uint64_t active = 0;

			void add_obj(T* obj) {
				// this makes it so that the first object that gets added gets set as the active one
				if (!active)
					active = obj->id();

				list.push_back(obj);
			}
		};

		void add_to_list(T* obj) {
			s_info[obj->parent()].add_obj(obj);
		}

		auto& get(T* obj) {
			return s_info[obj->parent()];
		}

	private:
		// accessor for our helper struct
		static inline std::unordered_map<const objects::base_parent*, info> s_info{};

	public: // used by the children to access information about them/their relatives
		static bool is_active(T* obj) {
			return s_info[obj->parent()].active == obj->id();
		}

		static bool is_active(const T* obj) {
			return s_info[obj->parent()].active == obj->id();
		}

		static void set_active(T* obj) {
			s_info[obj->parent()].active = obj->id();
		}

		static void set_active(const T* obj) {
			s_info[obj->parent()].active = obj->id();
		}

	public: // used by parents to access information about the activity of their children
		static T* get_active(objects::base_parent* parent) {
			auto& info = s_info[parent];
			return *std::ranges::find_if(info.list, [&](auto& obj) {
				return obj->id() == info.active;
			});
		}

		static T* get_active(const objects::base_parent* parent) {
			auto& info = s_info[parent];
			return *std::ranges::find_if(info.list, [&](auto& obj) {
				return obj->id() == info.active;
			});
		}

		static uint32_t get_active_idx(objects::base_parent* parent) {
			auto& info = s_info[parent];
			auto it = std::ranges::find_if(info.list, [&](auto& obj) {
				return obj->id() == info.active;
			});
			return it - info.list.begin();
		}

		static uint32_t get_active_idx(const objects::base_parent* parent) {
			auto& info = s_info[parent];
			auto it = std::ranges::find_if(info.list, [&](auto& obj) {
				return obj->id() == info.active;
			});
			return it - info.list.begin();
		}
	};
}