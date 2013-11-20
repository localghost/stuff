#include <boost/filesystem.hpp>
namespace fs = boost::filesystem3;

void recursive_copy_dir(const fs::path& from, const fs::path& to)
{
    fs::copy(from, to);
    std::for_each(fs::recursive_directory_iterator(from),
                  fs::recursive_directory_iterator(),
                  [&](const fs::path& path)
                  {
                      const auto& path_native = path.native();
                      const auto& from_native = from.native();
                      size_t pos = path_native.find(from_native);
                      if (0 == pos)
                      {
                          auto dest = path_native.substr(from_native.size());
                          dest.insert(0, to.native());
                          fs::copy(path, dest);
                      }
                  });
}

int main()
{
    const char* const from = "/tmp/from";
    const char* const to = "/tmp/boost_copy";
    recursive_copy_dir(from, to);
}
