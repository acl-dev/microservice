#include "http_rpc.h"
#include "hello_proto.h"
#include "hello_proto_gson.h"
#include "acl_cpp/serialize/gson_helper.ipp"
namespace acl
{
    acl::json_node& gson(acl::json &$json, const hello_protoc::say_hello_req &$obj)
    {
        acl::json_node &$node = $json.create_node();

        if (check_nullptr($obj.name))
            $node.add_null("name");
        else
            $node.add_text("name", acl::get_value($obj.name));


        return $node;
    }
    
    acl::json_node& gson(acl::json &$json, const hello_protoc::say_hello_req *$obj)
    {
        return gson ($json, *$obj);
    }


    acl::string gson(const hello_protoc::say_hello_req &$obj)
    {
        acl::json $json;
        acl::json_node &$node = acl::gson ($json, $obj);
        return $node.to_string ();
    }


    std::pair<bool,std::string> gson(acl::json_node &$node, hello_protoc::say_hello_req &$obj)
    {
        acl::json_node *name = $node["name"];
        std::pair<bool, std::string> $result;

        if(!name ||!($result = gson(*name, &$obj.name), $result.first))
            return std::make_pair(false, "required [hello_protoc::say_hello_req.name] failed:{"+$result.second+"}");
     
        return std::make_pair(true,"");
    }


    std::pair<bool,std::string> gson(acl::json_node &$node, hello_protoc::say_hello_req *$obj)
    {
        return gson($node, *$obj);
    }


    std::pair<bool,std::string> gson(const acl::string &$str, hello_protoc::say_hello_req &$obj)
    {
        acl::json _json;
        _json.update($str.c_str());
        if (!_json.finish())
        {
            return std::make_pair(false, "json not finish error");
        }
        return gson(_json.get_root(), $obj);
    }


    acl::json_node& gson(acl::json &$json, const hello_protoc::say_hello_resp &$obj)
    {
        acl::json_node &$node = $json.create_node();

        if (check_nullptr($obj.str))
            $node.add_null("str");
        else
            $node.add_text("str", acl::get_value($obj.str));


        return $node;
    }
    
    acl::json_node& gson(acl::json &$json, const hello_protoc::say_hello_resp *$obj)
    {
        return gson ($json, *$obj);
    }


    acl::string gson(const hello_protoc::say_hello_resp &$obj)
    {
        acl::json $json;
        acl::json_node &$node = acl::gson ($json, $obj);
        return $node.to_string ();
    }


    std::pair<bool,std::string> gson(acl::json_node &$node, hello_protoc::say_hello_resp &$obj)
    {
        acl::json_node *str = $node["str"];
        std::pair<bool, std::string> $result;

        if(!str ||!($result = gson(*str, &$obj.str), $result.first))
            return std::make_pair(false, "required [hello_protoc::say_hello_resp.str] failed:{"+$result.second+"}");
     
        return std::make_pair(true,"");
    }


    std::pair<bool,std::string> gson(acl::json_node &$node, hello_protoc::say_hello_resp *$obj)
    {
        return gson($node, *$obj);
    }


    std::pair<bool,std::string> gson(const acl::string &$str, hello_protoc::say_hello_resp &$obj)
    {
        acl::json _json;
        _json.update($str.c_str());
        if (!_json.finish())
        {
            return std::make_pair(false, "json not finish error");
        }
        return gson(_json.get_root(), $obj);
    }


}///end of acl.
