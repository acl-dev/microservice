namespace acl
{
    //hello_protoc::say_hello_req
    acl::string gson(const hello_protoc::say_hello_req &$obj);
    acl::json_node& gson(acl::json &$json, const hello_protoc::say_hello_req &$obj);
    acl::json_node& gson(acl::json &$json, const hello_protoc::say_hello_req *$obj);
    std::pair<bool,std::string> gson(acl::json_node &$node, hello_protoc::say_hello_req &$obj);
    std::pair<bool,std::string> gson(acl::json_node &$node, hello_protoc::say_hello_req *$obj);
    std::pair<bool,std::string> gson(const acl::string &str, hello_protoc::say_hello_req &$obj);

    //hello_protoc::say_hello_resp
    acl::string gson(const hello_protoc::say_hello_resp &$obj);
    acl::json_node& gson(acl::json &$json, const hello_protoc::say_hello_resp &$obj);
    acl::json_node& gson(acl::json &$json, const hello_protoc::say_hello_resp *$obj);
    std::pair<bool,std::string> gson(acl::json_node &$node, hello_protoc::say_hello_resp &$obj);
    std::pair<bool,std::string> gson(acl::json_node &$node, hello_protoc::say_hello_resp *$obj);
    std::pair<bool,std::string> gson(const acl::string &str, hello_protoc::say_hello_resp &$obj);

}///end of acl.
