#include <fstream>
#include <ShlObj.h>
#include "archivex.hpp"
#include "config.hpp"

CConfig config_system;

void CConfig::run(const char* name) noexcept {
	PWSTR pathToDocuments;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &pathToDocuments))) {
		path = pathToDocuments;
		path /= name;
		CoTaskMemFree(pathToDocuments);
	}

	if (!std::filesystem::is_directory(path)) {
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}

	std::transform(std::filesystem::directory_iterator{ path },
		std::filesystem::directory_iterator{ },
		std::back_inserter(configs),
		[](const auto & entry) { return entry.path().filename().string(); });
}

void CConfig::load(size_t id) noexcept {
	if (!std::filesystem::is_directory(path)) {
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}

	std::ifstream in{ path / configs[id] };

	if (!in.good())
		return;

	ArchiveX<std::ifstream>{ in } >> item;
	in.close();
}

void CConfig::save(size_t id) const noexcept {
	if (!std::filesystem::is_directory(path)) {
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}

	std::ofstream out{ path / configs[id] };

	if (!out.good())
		return;

	ArchiveX<std::ofstream>{ out } << item;
	out.close();
}

void CConfig::add(const char* name) noexcept {
	if (*name && std::find(std::cbegin(configs), std::cend(configs), name) == std::cend(configs))
		configs.emplace_back(name);
}

void CConfig::remove(size_t id) noexcept {
	std::filesystem::remove(path / configs[id]);
	configs.erase(configs.cbegin() + id);
}

void CConfig::rename(size_t item, const char* newName) noexcept {
	std::filesystem::rename(path / configs[item], path / newName);
	configs[item] = newName;
}

void CConfig::reset() noexcept {
	item = { };
}
