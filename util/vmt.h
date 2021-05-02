#pragma once

struct MemoryProtection_t {
	MemoryProtection_t(LPVOID address, uint32_t size, DWORD flags) : m_address(address), m_size(size), m_flags(0) {
		if (!VirtualProtect(m_address, m_size, flags, &m_flags))
			throw std::system_error(GetLastError(), std::system_category(), XOR("Failed to protect the page-region"));
	}
	~MemoryProtection_t() {
		VirtualProtect(m_address, m_size, m_flags, &m_flags);
	}

	LPVOID m_address;
	uint32_t m_size;
	DWORD m_flags;
};

#define INIT_MEM_PROTECT_RW( address, size ) auto protect = MemoryProtection_t( address, size, PAGE_READWRITE );
#define INIT_MEM_PROTECT_R( address, size ) auto protect = MemoryProtection_t( address, size, PAGE_READONLY );
#define INIT_MEM_PROTECT_W( address, size ) auto protect = MemoryProtection_t( address, size, PAGE_WRITECOPY );
#define INIT_MEM_PROTECT( address, size, flags ) auto protect = MemoryProtection_t( address, size, flags );


class VMT {
public:

	VMT() = default;
	VMT(uintptr_t ptr) : m_vtable(reinterpret_cast<uintptr_t**>(ptr)), m_table_length(0), m_orig(nullptr), m_replace(nullptr) {};
	VMT(void* ptr) : m_vtable(reinterpret_cast<uintptr_t**>(ptr)), m_table_length(0), m_orig(nullptr), m_replace(nullptr) {};
	VMT(Address ptr) : m_vtable(ptr.as<uintptr_t**>()), m_table_length(0), m_orig(nullptr), m_replace(nullptr) {};

	uint32_t GetVirtualTableLength(Address table)
	{
		auto length = uint32_t{};

		// Walk through every function until it is no longer valid
		for (length = 0; table.as<uintptr_t*>()[length]; length++)
			if (IS_INTRESOURCE(table.as<uintptr_t*>()[length]))
				break;

		return length;
	}

	bool Init() {
		if (!m_vtable)
			return false;

		INIT_MEM_PROTECT_RW(m_vtable, sizeof(uintptr_t));

		/// Store old vtable
		m_orig = *m_vtable;

		m_table_length = GetVirtualTableLength(m_orig);

		/// Either faulty vtable or function fail
		if (!m_table_length)
			return false;

		/// Allocate new vtable ( +1 for RTTI )
		m_replace = std::make_unique<uintptr_t[]>(m_table_length + 1);

		/// instantiate all values with 0
		std::memset(m_replace.get(),
			NULL,
			m_table_length * sizeof(uintptr_t) + sizeof(uintptr_t));

		/// The following two memcpy's could be just made 
		/// into 1 call but for demonstration purposes
		/// I'll leave it like that

		/// Copy old table
		/// Skip first 4 bytes to later insert RTTI there
		std::memcpy(&m_replace[1],
			m_orig,
			m_table_length * sizeof(uintptr_t));

		/// Copy RTTI
		std::memcpy(m_replace.get(),
			&m_orig[-1],
			sizeof(uintptr_t));

		/// Apply new vtable, again skipping the first 4
		/// bytes since that's where the RTTI is now located
		*m_vtable = &m_replace[1];

		return true;
	}

	template< typename t >
	void add(const uint16_t index, t replace_function) {
		/// Is index out of bounds?
		if (index < 0 || index > m_table_length) {
			throw std::out_of_range(tfm::format(XOR("VMT::Hook - Trying to Hook at index %i while max index is %i"), index, m_table_length));
		}

		m_replace[index + 1] = reinterpret_cast<uintptr_t>(replace_function);
	}

	template< typename t >
	t GetOldMethod(const uint16_t index) {
		/// Is index out of bounds?
		if (index < 0 || index > m_table_length) {
			throw std::out_of_range(tfm::format(XOR("VMT::GetOldMethod - Trying to get original function of index %i while max index is %i"), index, m_table_length));
		}

		return reinterpret_cast<t>(m_orig[index]);
	}

	void UnHook(const uint16_t index) {
		/// Is index out of bounds?
		if (index < 0 || index > m_table_length) {
			throw std::out_of_range(tfm::format(XOR("VMT::UnHook - Trying to UnHook at index %i while max index is %i"), index, m_table_length));
		}

		m_replace[index + 1] = m_orig[index];
	}

	void UnHook() {
		/// Check if it was already restored
		if (!m_orig)
			return;

		INIT_MEM_PROTECT_RW(m_vtable, sizeof(uintptr_t));

		*m_vtable = m_orig;

		/// Prevent double unhook
		m_orig = nullptr;
	}

	uintptr_t** m_vtable;
	uint16_t m_table_length;
	uintptr_t* m_orig;
	std::unique_ptr<uintptr_t[]> m_replace;
};