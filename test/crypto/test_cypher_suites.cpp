#define BOOST_TEST_MODULE cypher_suites
#include <boost/test/included/unit_test.hpp>

#include <fc/crypto/public_key.hpp>
#include <fc/crypto/private_key.hpp>
#include <fc/crypto/signature.hpp>
#include <fc/utility.hpp>

using namespace fc::crypto;
using namespace fc;

BOOST_AUTO_TEST_SUITE(cypher_suites)
BOOST_AUTO_TEST_CASE(test_k1) try {
   auto private_key_string = std::string("5JndU9CotXTwVLGenHxad9VHmL8JwekssUqo9ZsKJxupXxV6ipZ");
   auto expected_public_key = std::string("ZYT6cAXzwY1swM1KM4FMzeTvaTy7HPyGMeBnRLe9qfrw2kynFcXaz");
   auto test_private_key = private_key(private_key_string);
   auto test_public_key = test_private_key.get_public_key();

   BOOST_CHECK_EQUAL(private_key_string, test_private_key.to_string());
   BOOST_CHECK_EQUAL(expected_public_key, test_public_key.to_string());
} FC_LOG_AND_RETHROW();

BOOST_AUTO_TEST_CASE(test_r1) try {
   auto private_key_string = std::string("PVT_R1_iyQmnyPEGvFd8uffnk152WC2WryBjgTrg22fXQryuGL9mU6qW");
   auto expected_public_key = std::string("PUB_R1_6EPHFSKVYHBjQgxVGQPrwCxTg7BbZ69H9i4gztN9deKTEXYne4");
   auto test_private_key = private_key(private_key_string);
   auto test_public_key = test_private_key.get_public_key();

   BOOST_CHECK_EQUAL(private_key_string, test_private_key.to_string());
   BOOST_CHECK_EQUAL(expected_public_key, test_public_key.to_string());
} FC_LOG_AND_RETHROW();

BOOST_AUTO_TEST_CASE(test_k1_recovery) try {
   auto payload = "Test Cases";
   auto digest = sha256::hash(payload, const_strlen(payload));
   auto key = private_key::generate<ecc::private_key_shim>();
   auto pub = key.get_public_key();
   auto sig = key.sign(digest);

   auto recovered_pub = public_key(sig, digest);
   std::cout << recovered_pub << std::endl;

   BOOST_CHECK_EQUAL(recovered_pub.to_string(), pub.to_string());
} FC_LOG_AND_RETHROW();

BOOST_AUTO_TEST_CASE(test_r1_recovery) try {
   auto payload = "Test Cases";
   auto digest = sha256::hash(payload, const_strlen(payload));
   auto key = private_key::generate<r1::private_key_shim>();
   auto pub = key.get_public_key();
   auto sig = key.sign(digest);

   auto recovered_pub = public_key(sig, digest);
   std::cout << recovered_pub << std::endl;

   BOOST_CHECK_EQUAL(recovered_pub.to_string(), pub.to_string());
} FC_LOG_AND_RETHROW();

BOOST_AUTO_TEST_CASE(test_k1_recyle) try {
   auto key = private_key::generate<ecc::private_key_shim>();
   auto pub = key.get_public_key();
   auto pub_str = pub.to_string();
   auto recycled_pub = public_key(pub_str);

   std::cout << pub << " -> " << recycled_pub << std::endl;

   BOOST_CHECK_EQUAL(pub.to_string(), recycled_pub.to_string());
} FC_LOG_AND_RETHROW();

BOOST_AUTO_TEST_CASE(test_r1_recyle) try {
   auto key = private_key::generate<r1::private_key_shim>();
   auto pub = key.get_public_key();
   auto pub_str = pub.to_string();
   auto recycled_pub = public_key(pub_str);

   std::cout << pub << " -> " << recycled_pub << std::endl;

   BOOST_CHECK_EQUAL(pub.to_string(), recycled_pub.to_string());
} FC_LOG_AND_RETHROW();


BOOST_AUTO_TEST_SUITE_END()
