#include "../base/json_config.h"
#include <assert.h>

class json_config_impl : public chef::json_config {
  public:
    int unmarshal() {
      try {
        name_ = root_["name"].asString();
        age_ = root_["age"].asInt();
        for (Json::ArrayIndex i = 0; i < root_["nums"].size(); i++) {
          nums_.push_back(root_["nums"][i].asInt());
        }
        //std::string notexist = root_["notexist"].asString();
      } catch(...) {
        std::cerr << "json_config_impl::unmarshal failed.\n";
        return -1;
      }
      return 0;
    }

  public:
    std::string name() const { return name_; }
    int age() const { return age_; }
    std::vector<int> nums() { return nums_; }

  private:
    std::string name_;
    int age_;
    std::vector<int> nums_;
};

int main() {
  printf("Check json_config.\n");
  json_config_impl jci;
  assert(jci.load_file("config.json") == 0);
  printf("%s\n", jci.styled_string().c_str());
  assert(jci.unmarshal() == 0);
  assert(jci.name() == "chef");
  assert(jci.nums().size() == 3);
  assert(jci.nums()[0] == 1);
  assert(jci.nums()[1] == 2);
  assert(jci.nums()[2] == 3);

  printf("Check json_config done.\n");
  return 0;
}
