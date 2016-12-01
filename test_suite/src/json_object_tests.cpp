// Copyright 2013 Daniel Parker
// Distributed under Boost license

#include <boost/test/unit_test.hpp>
#include <jsoncons/json.hpp>
#include <jsoncons/json_serializer.hpp>
#include <sstream>
#include <vector>
#include <utility>
#include <ctime>
#include <map>

using namespace jsoncons;

BOOST_AUTO_TEST_SUITE(json_object_test_suite)

BOOST_AUTO_TEST_CASE(test1)
{
    std::vector<std::string> u = { "a","c","a","d","e","e","f","a" };
    for (auto p = u.begin(); p != u.end(); ++p)
    {
        if (p != u.begin())
        {
            std::cout << ",";
        }
        std::cout << *p;
    }
    std::cout << std::endl;
    auto it = unique_unsorted(u.begin(),u.end(),
                              std::greater<std::string>(),
                              [](const std::string& a, const std::string& b){return a.compare(b) == 0;});
    for (auto p = u.begin(); p != it; ++p)
    {
        if (p != u.begin())
        {
            std::cout << ",";
        }
        std::cout << *p;
    }
    std::cout << std::endl;

}

BOOST_AUTO_TEST_CASE(test2)
{
    std::vector<std::string> u = {"a","b","a"};
    for (auto p = u.begin(); p != u.end(); ++p)
    {
        if (p != u.begin())
        {
            std::cout << ",";
        }
        std::cout << *p;
    }
    std::cout << std::endl;
    auto it = unique_unsorted(u.begin(),u.end(),
                               std::greater<std::string>(),
                              [](const std::string& a, const std::string& b){return a.compare(b) == 0;});
    for (auto p = u.begin(); p != it; ++p)
    {
        if (p != u.begin())
        {
            std::cout << ",";
        }
        std::cout << *p;
    }
    std::cout << std::endl;

}

BOOST_AUTO_TEST_CASE(test_no_dups)
{
    std::vector<std::string> u = { "a","b","f","e","c","d"};
    for (auto p = u.begin(); p != u.end(); ++p)
    {
        if (p != u.begin())
        {
            std::cout << ",";
        }
        std::cout << *p;
    }
    std::cout << std::endl;
    auto it = unique_unsorted(u.begin(),u.end(),
                              std::greater<std::string>(),
                              [](const std::string& a, const std::string& b){return a.compare(b) == 0;});
    for (auto p = u.begin(); p != it; ++p)
    {
        if (p != u.begin())
        {
            std::cout << ",";
        }
        std::cout << *p;
    }
    std::cout << std::endl;

}

BOOST_AUTO_TEST_CASE(test_multiple_values)
{
    json j1 = json::parse(R"({"first":1,"second":2,"third":3})");
    BOOST_CHECK_EQUAL(3,j1.size());
    BOOST_CHECK_EQUAL(1,j1["first"].as<int>());
    BOOST_CHECK_EQUAL(2,j1["second"].as<int>());
    BOOST_CHECK_EQUAL(3,j1["third"].as<int>());

    json j2 = json::parse(R"({"first":1,"second":2,"first":3})");
    BOOST_CHECK_EQUAL(2,j2.size());
    BOOST_CHECK_EQUAL(3,j2["first"].as<int>());
    BOOST_CHECK_EQUAL(2,j2["second"].as<int>());

    ojson oj1 = ojson::parse(R"({"first":1,"second":2,"third":3})");
    BOOST_CHECK_EQUAL(3,oj1.size());
    BOOST_CHECK_EQUAL(1,oj1["first"].as<int>());
    BOOST_CHECK_EQUAL(2,oj1["second"].as<int>());
    BOOST_CHECK_EQUAL(3,oj1["third"].as<int>());

    ojson oj2 = ojson::parse(R"({"first":1,"second":2,"first":3})");
    BOOST_CHECK_EQUAL(2,oj2.size());
    BOOST_CHECK_EQUAL(3,oj2["first"].as<int>());
    BOOST_CHECK_EQUAL(2,oj2["second"].as<int>());
}

BOOST_AUTO_TEST_CASE(test_erase_member)
{
    json o;
    o["key"] = "Hello";

    BOOST_CHECK(o.size() == 1);
    o.erase("key");
    BOOST_CHECK(o.size() == 0);

    json a;
    json b = json::object();
    b["input-file"] = "config_file";
    json b_copy = b;

    a["b"] = std::move(b);

    BOOST_CHECK_EQUAL(true,a["b"].is_object());
    BOOST_CHECK_EQUAL(a["b"],b_copy);
}

BOOST_AUTO_TEST_CASE(test_empty_object)
{
    json a;
    BOOST_CHECK(a.size() == 0);
    BOOST_CHECK(a.is_object());
    BOOST_CHECK(a.is<json::object>());

    json::object_iterator begin = a.object_range().begin();
    json::object_iterator end = a.object_range().end();

    for (json::object_iterator it = begin; it != end; ++it)
    {
        BOOST_CHECK(false);
    }

    a["key"] = "Hello";
    BOOST_CHECK(a.size() == 1);
    BOOST_CHECK(a.is_object());
    BOOST_CHECK(a.is<json::object>());
}

BOOST_AUTO_TEST_CASE(test_const_empty_object)
{
    const json b;
    BOOST_CHECK(b.size() == 0);
    BOOST_CHECK(b.is_object());
    BOOST_CHECK(b.is<json::object>());

    json::const_object_iterator begin = b.object_range().begin();
    json::const_object_iterator end = b.object_range().end();

    for (json::const_object_iterator it = begin; it != end; ++it)
    {
        BOOST_CHECK(false);
    }
}

BOOST_AUTO_TEST_CASE(test_empty_object_reserve)
{
    json c;
    BOOST_CHECK(c.size() == 0);
    BOOST_CHECK(c.is_object());
    BOOST_CHECK(c.is<json::object>());
    c.reserve(100);
    BOOST_CHECK(c.capacity() == 100);
    c["key"] = "Hello";
    BOOST_CHECK(c.size() == 1);
    BOOST_CHECK(c.is_object());
    BOOST_CHECK(c.is<json::object>());
    BOOST_CHECK(c.capacity() == 100);
}

BOOST_AUTO_TEST_CASE(test_empty_object_copy)
{
    json a;
    BOOST_CHECK(a.size() == 0);
    BOOST_CHECK(a.is_object());
    BOOST_CHECK(a.is<json::object>());

    json b = a;
    BOOST_CHECK(b.size() == 0);
    BOOST_CHECK(b.is_object());
    BOOST_CHECK(b.is<json::object>());
}

BOOST_AUTO_TEST_CASE(test_empty_object_assignment)
{
    json a;
    BOOST_CHECK(a.size() == 0);
    BOOST_CHECK(a.is_object());
    BOOST_CHECK(a.is<json::object>());

    json b = json::make_array<1>(10);
    BOOST_CHECK(b.size() == 10);
    BOOST_CHECK(b.is_array());
    BOOST_CHECK(b.is<json::array>());

    b = a;
    BOOST_CHECK(b.size() == 0);
    BOOST_CHECK(b.is_object());
    BOOST_CHECK(b.is<json::object>());

    json c;
    c["key"] = "value";
    BOOST_CHECK(c.size() == 1);
    BOOST_CHECK(c.is_object());
    BOOST_CHECK(c.is<json::object>());
    c = a;
    BOOST_CHECK(c.size() == 0);
    BOOST_CHECK(c.is_object());
    BOOST_CHECK(c.is<json::object>());
}

BOOST_AUTO_TEST_CASE(test_get)
{
    json a;

    a["field1"] = "value1";

    std::string s1 = a.at("field1").as<std::string>();
    std::string s1a = a.at("field1").as<std::string>();
    std::string s2 = a.get_with_default("field2","null");
    BOOST_REQUIRE_THROW(a.at("field2"), std::out_of_range);

    BOOST_CHECK(s1 == std::string("value1"));
    BOOST_CHECK(s1a == std::string("value1"));

    //std::cout << "s2=" << s2 << std::endl;
    BOOST_CHECK_EQUAL(std::string("null"),s2);
}

BOOST_AUTO_TEST_CASE(test_proxy_get)
{
    json a;

    a["object1"] = json();
    a["object1"]["field1"] = "value1";

    std::string s1 = a["object1"].at("field1").as<std::string>();
    std::string s1a = a["object1"].at("field1").as<std::string>();
    std::string s2 = a["object1"].get("field2",json::null()).as<std::string>();
    a["object1"].get("field2", json::null()).is_null();
    //std::cout << s2 << std::endl;
    BOOST_REQUIRE_THROW(a["object1"].at("field2").as<std::string>(), std::out_of_range);

    BOOST_CHECK_EQUAL(std::string("value1"),s1);
    BOOST_CHECK_EQUAL(std::string("value1"),s1a);
    BOOST_CHECK_EQUAL(std::string("null"),s2);
}

BOOST_AUTO_TEST_CASE(test_set_and_proxy_set)
{
    json a;

    a.set("object1",json());
    a.set("field1","value1");
    a["object1"].set("field2","value2");

    BOOST_CHECK_EQUAL(std::string("value1"),a["field1"].as<std::string>());
    BOOST_CHECK_EQUAL(std::string("value2"),a["object1"]["field2"].as<std::string>());
}

BOOST_AUTO_TEST_CASE(test_const_member_read)
{
    json a;

    a["field1"] = 10;

    a["field2"];

    const json b(a);

    int val1 = b["field1"].as<int>();
    BOOST_CHECK(val1 == 10);
    BOOST_REQUIRE_THROW(b["field2"], std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_proxy_const_member_read)
{
    json a;

    a["object1"] = json();
    a["object1"]["field1"] = "value1";
    a["object1"]["field2"]; // No throw yet

    const json b(a);

    std::string s1 = b["object1"]["field1"].as<std::string>();
    BOOST_REQUIRE_THROW(b["object1"]["field2"], std::out_of_range);

    BOOST_CHECK(s1 == std::string("value1"));
}

BOOST_AUTO_TEST_CASE(test_object_equals)
{
    json a;
    a["field1"] = "value1";

    json b;
    b["field1"] = "value1";

    BOOST_CHECK(a == b);

    json c;
    c["field1"] = 10;

    BOOST_CHECK(a != c);
}

BOOST_AUTO_TEST_CASE(test_json_object_iterator_1)
{
    json a;
    a["name1"] = "value1";
    a["name2"] = "value2";
    a["name3"] = "value3";

    json::object_iterator it = a.object_range().begin();
    BOOST_CHECK((*it).key() == "name1");
    BOOST_CHECK((*it).value() == json("value1"));
    ++it;
    BOOST_CHECK((*it).key() == "name2");
    BOOST_CHECK((*it).value() == json("value2"));

    BOOST_CHECK((*(it++)).key() == "name2");
    BOOST_CHECK((*it).key() == "name3");
    BOOST_CHECK((*it).value() == json("value3"));

    BOOST_CHECK((*(it--)).key() == "name3");
    BOOST_CHECK((*it).value() == json("value2"));
    BOOST_CHECK((*(--it)).value() == json("value1"));

    json::member_type member = *it;
    BOOST_CHECK(member.key() == "name1");
    BOOST_CHECK(member.value() == json("value1"));
}

BOOST_AUTO_TEST_CASE(test_json_object_iterator_2)
{
    json a;
    a["name1"] = "value1";
    a["name2"] = "value2";
    a["name3"] = "value3";

    json::const_object_iterator it = a.object_range().begin();
    BOOST_CHECK((*it).key() == "name1");
    BOOST_CHECK((*it).value() == json("value1"));
    ++it;
    BOOST_CHECK((*it).key() == "name2");
    BOOST_CHECK((*it).value() == json("value2"));

    BOOST_CHECK((*(it++)).key() == "name2");
    BOOST_CHECK((*it).key() == "name3");
    BOOST_CHECK((*it).value() == json("value3"));

    BOOST_CHECK((*(it--)).key() == "name3");
    BOOST_CHECK((*it).value() == json("value2"));

    BOOST_CHECK((*(--it)).value() == json("value1"));

    json::member_type member = *it;
    BOOST_CHECK(member.key() == "name1");
    BOOST_CHECK(member.value() == json("value1"));
}

BOOST_AUTO_TEST_CASE(test_json_object_iterator_3)
{
    json a;
    a["name1"] = "value1";
    a["name2"] = "value2";
    a["name3"] = "value3";

    json::const_object_iterator it = static_cast<const json&>(a).object_range().begin();
    BOOST_CHECK(it == a.object_range().begin());
    BOOST_CHECK(it != a.object_range().end());
    BOOST_CHECK((*it).key() == "name1");
    BOOST_CHECK((*it).value() == json("value1"));
    ++it;
    BOOST_CHECK(it != a.object_range().end());
    BOOST_CHECK((*it).key() == "name2");
    BOOST_CHECK((*it).value() == json("value2"));

    BOOST_CHECK((*(it++)).key() == "name2");
    BOOST_CHECK(it != a.object_range().end());
    BOOST_CHECK((*it).key() == "name3");
    BOOST_CHECK((*it).value() == json("value3"));

    BOOST_CHECK((*(it--)).key() == "name3");
    BOOST_CHECK((*it).value() == json("value2"));

    BOOST_CHECK((*(--it)).value() == json("value1"));
    BOOST_CHECK(it == a.object_range().begin());

    json::member_type member = *it;
    BOOST_CHECK(member.key() == "name1");
    BOOST_CHECK(member.value() == json("value1"));

    //*it = member; // Don't want this to compile
}

BOOST_AUTO_TEST_CASE(test_object_key_proxy)
{
    json a;
    a["key1"] = "value1";

    json b;
    b["key2"] = json();
    b["key2"]["key3"] = std::move(a);

    BOOST_CHECK(a.is_null());
}

BOOST_AUTO_TEST_SUITE_END()

