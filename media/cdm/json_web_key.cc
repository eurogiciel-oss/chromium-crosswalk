// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "media/cdm/json_web_key.h"

#include "base/base64.h"
#include "base/json/json_reader.h"
#include "base/json/json_string_value_serializer.h"
#include "base/json/string_escape.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_util.h"
#include "base/values.h"

namespace media {

const char kKeysTag[] = "keys";
const char kKeyTypeTag[] = "kty";
const char kKeyTypeOct[] = "oct";  // Octet sequence.
const char kAlgTag[] = "alg";
const char kAlgA128KW[] = "A128KW";  // AES key wrap using a 128-bit key.
const char kKeyTag[] = "k";
const char kKeyIdTag[] = "kid";
const char kKeyIdsTag[] = "kids";
const char kBase64Padding = '=';
const char kTypeTag[] = "type";
const char kTemporarySession[] = "temporary";
const char kPersistentLicenseSession[] = "persistent-license";
const char kPersistentReleaseMessageSession[] = "persistent-release-message";

// Encodes |input| into a base64 string without padding.
static std::string EncodeBase64(const uint8* input, int input_length) {
  std::string encoded_text;
  base::Base64Encode(
      std::string(reinterpret_cast<const char*>(input), input_length),
      &encoded_text);

  // Remove any padding characters added by Base64Encode().
  size_t found = encoded_text.find_last_not_of(kBase64Padding);
  if (found != std::string::npos)
    encoded_text.erase(found + 1);

  return encoded_text;
}

// Decodes an unpadded base64 string. Returns empty string on error.
static std::string DecodeBase64(const std::string& encoded_text) {
  // EME spec doesn't allow padding characters.
  if (encoded_text.find_first_of(kBase64Padding) != std::string::npos) {
    DVLOG(1) << "Padding characters not allowed: " << encoded_text;
    return std::string();
  }

  // Since base::Base64Decode() requires padding characters, add them so length
  // of |encoded_text| is exactly a multiple of 4.
  size_t num_last_grouping_chars = encoded_text.length() % 4;
  std::string modified_text = encoded_text;
  if (num_last_grouping_chars > 0)
    modified_text.append(4 - num_last_grouping_chars, kBase64Padding);

  std::string decoded_text;
  if (!base::Base64Decode(modified_text, &decoded_text)) {
    DVLOG(1) << "Base64 decoding failed on: " << modified_text;
    return std::string();
  }

  return decoded_text;
}

std::string GenerateJWKSet(const uint8* key, int key_length,
                           const uint8* key_id, int key_id_length) {
  // Both |key| and |key_id| need to be base64 encoded strings in the JWK.
  std::string key_base64 = EncodeBase64(key, key_length);
  std::string key_id_base64 = EncodeBase64(key_id, key_id_length);

  // Create the JWK, and wrap it into a JWK Set.
  scoped_ptr<base::DictionaryValue> jwk(new base::DictionaryValue());
  jwk->SetString(kKeyTypeTag, kKeyTypeOct);
  jwk->SetString(kAlgTag, kAlgA128KW);
  jwk->SetString(kKeyTag, key_base64);
  jwk->SetString(kKeyIdTag, key_id_base64);
  scoped_ptr<base::ListValue> list(new base::ListValue());
  list->Append(jwk.release());
  base::DictionaryValue jwk_set;
  jwk_set.Set(kKeysTag, list.release());

  // Finally serialize |jwk_set| into a string and return it.
  std::string serialized_jwk;
  JSONStringValueSerializer serializer(&serialized_jwk);
  serializer.Serialize(jwk_set);
  return serialized_jwk;
}

// Processes a JSON Web Key to extract the key id and key value. Sets |jwk_key|
// to the id/value pair and returns true on success.
static bool ConvertJwkToKeyPair(const base::DictionaryValue& jwk,
                                KeyIdAndKeyPair* jwk_key) {
  std::string type;
  if (!jwk.GetString(kKeyTypeTag, &type) || type != kKeyTypeOct) {
    DVLOG(1) << "Missing or invalid '" << kKeyTypeTag << "': " << type;
    return false;
  }

  std::string alg;
  if (!jwk.GetString(kAlgTag, &alg) || alg != kAlgA128KW) {
    DVLOG(1) << "Missing or invalid '" << kAlgTag << "': " << alg;
    return false;
  }

  // Get the key id and actual key parameters.
  std::string encoded_key_id;
  std::string encoded_key;
  if (!jwk.GetString(kKeyIdTag, &encoded_key_id)) {
    DVLOG(1) << "Missing '" << kKeyIdTag << "' parameter";
    return false;
  }
  if (!jwk.GetString(kKeyTag, &encoded_key)) {
    DVLOG(1) << "Missing '" << kKeyTag << "' parameter";
    return false;
  }

  // Key ID and key are base64-encoded strings, so decode them.
  std::string raw_key_id = DecodeBase64(encoded_key_id);
  if (raw_key_id.empty()) {
    DVLOG(1) << "Invalid '" << kKeyIdTag << "' value: " << encoded_key_id;
    return false;
  }

  std::string raw_key = DecodeBase64(encoded_key);
  if (raw_key.empty()) {
    DVLOG(1) << "Invalid '" << kKeyTag << "' value: " << encoded_key;
    return false;
  }

  // Add the decoded key ID and the decoded key to the list.
  *jwk_key = std::make_pair(raw_key_id, raw_key);
  return true;
}

bool ExtractKeysFromJWKSet(const std::string& jwk_set,
                           KeyIdAndKeyPairs* keys,
                           MediaKeys::SessionType* session_type) {
  if (!base::IsStringASCII(jwk_set)) {
    DVLOG(1) << "Non ASCII JWK Set: " << jwk_set;
    return false;
  }

  scoped_ptr<base::Value> root(base::JSONReader().ReadToValue(jwk_set));
  if (!root.get() || root->GetType() != base::Value::TYPE_DICTIONARY) {
    DVLOG(1) << "Not valid JSON: " << jwk_set << ", root: " << root.get();
    return false;
  }

  // Locate the set from the dictionary.
  base::DictionaryValue* dictionary =
      static_cast<base::DictionaryValue*>(root.get());
  base::ListValue* list_val = NULL;
  if (!dictionary->GetList(kKeysTag, &list_val)) {
    DVLOG(1) << "Missing '" << kKeysTag
             << "' parameter or not a list in JWK Set";
    return false;
  }

  // Create a local list of keys, so that |jwk_keys| only gets updated on
  // success.
  KeyIdAndKeyPairs local_keys;
  for (size_t i = 0; i < list_val->GetSize(); ++i) {
    base::DictionaryValue* jwk = NULL;
    if (!list_val->GetDictionary(i, &jwk)) {
      DVLOG(1) << "Unable to access '" << kKeysTag << "'[" << i
               << "] in JWK Set";
      return false;
    }
    KeyIdAndKeyPair key_pair;
    if (!ConvertJwkToKeyPair(*jwk, &key_pair)) {
      DVLOG(1) << "Error from '" << kKeysTag << "'[" << i << "]";
      return false;
    }
    local_keys.push_back(key_pair);
  }

  // Successfully processed all JWKs in the set. Now check if "type" is
  // specified.
  base::Value* value = NULL;
  std::string session_type_id;
  if (!dictionary->Get(kTypeTag, &value)) {
    // Not specified, so use the default type.
    *session_type = MediaKeys::TEMPORARY_SESSION;
  } else if (!value->GetAsString(&session_type_id)) {
    DVLOG(1) << "Invalid '" << kTypeTag << "' value";
    return false;
  } else if (session_type_id == kTemporarySession) {
    *session_type = MediaKeys::TEMPORARY_SESSION;
  } else if (session_type_id == kPersistentLicenseSession) {
    *session_type = MediaKeys::PERSISTENT_LICENSE_SESSION;
  } else if (session_type_id == kPersistentReleaseMessageSession) {
    *session_type = MediaKeys::PERSISTENT_RELEASE_MESSAGE_SESSION;
  } else {
    DVLOG(1) << "Invalid '" << kTypeTag << "' value: " << session_type_id;
    return false;
  }

  // All done.
  keys->swap(local_keys);
  return true;
}

void CreateLicenseRequest(const uint8* key_id,
                          int key_id_length,
                          MediaKeys::SessionType session_type,
                          std::vector<uint8>* license) {
  // Create the license request.
  scoped_ptr<base::DictionaryValue> request(new base::DictionaryValue());
  scoped_ptr<base::ListValue> list(new base::ListValue());
  list->AppendString(EncodeBase64(key_id, key_id_length));
  request->Set(kKeyIdsTag, list.release());

  switch (session_type) {
    case MediaKeys::TEMPORARY_SESSION:
      request->SetString(kTypeTag, kTemporarySession);
      break;
    case MediaKeys::PERSISTENT_LICENSE_SESSION:
      request->SetString(kTypeTag, kPersistentLicenseSession);
      break;
    case MediaKeys::PERSISTENT_RELEASE_MESSAGE_SESSION:
      request->SetString(kTypeTag, kPersistentReleaseMessageSession);
      break;
  }

  // Serialize the license request as a string.
  std::string json;
  JSONStringValueSerializer serializer(&json);
  serializer.Serialize(*request);

  // Convert the serialized license request into std::vector and return it.
  std::vector<uint8> result(json.begin(), json.end());
  license->swap(result);
}

bool ExtractFirstKeyIdFromLicenseRequest(const std::vector<uint8>& license,
                                         std::vector<uint8>* first_key) {
  const std::string license_as_str(
      reinterpret_cast<const char*>(!license.empty() ? &license[0] : NULL),
      license.size());
  if (!base::IsStringASCII(license_as_str)) {
    DVLOG(1) << "Non ASCII license: " << license_as_str;
    return false;
  }

  scoped_ptr<base::Value> root(base::JSONReader().ReadToValue(license_as_str));
  if (!root.get() || root->GetType() != base::Value::TYPE_DICTIONARY) {
    DVLOG(1) << "Not valid JSON: " << license_as_str;
    return false;
  }

  // Locate the set from the dictionary.
  base::DictionaryValue* dictionary =
      static_cast<base::DictionaryValue*>(root.get());
  base::ListValue* list_val = NULL;
  if (!dictionary->GetList(kKeyIdsTag, &list_val)) {
    DVLOG(1) << "Missing '" << kKeyIdsTag << "' parameter or not a list";
    return false;
  }

  // Get the first key.
  if (list_val->GetSize() < 1) {
    DVLOG(1) << "Empty '" << kKeyIdsTag << "' list";
    return false;
  }

  std::string encoded_key;
  if (!list_val->GetString(0, &encoded_key)) {
    DVLOG(1) << "First entry in '" << kKeyIdsTag << "' not a string";
    return false;
  }

  std::string decoded_string = DecodeBase64(encoded_key);
  if (decoded_string.empty()) {
    DVLOG(1) << "Invalid '" << kKeyIdsTag << "' value: " << encoded_key;
    return false;
  }

  std::vector<uint8> result(decoded_string.begin(), decoded_string.end());
  first_key->swap(result);
  return true;
}

}  // namespace media
