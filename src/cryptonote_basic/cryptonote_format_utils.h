// Copyright (c) 2017, The Graft Project
// Copyright (c) 2014-2017, The Monero Project
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once
#include "cryptonote_protocol/cryptonote_protocol_defs.h"
#include "cryptonote_basic_impl.h"
#include "account.h"
#include "include_base_utils.h"
#include "crypto/crypto.h"
#include "crypto/hash.h"
#include "supernode/grafttxextra.h"

namespace cryptonote
{
  //---------------------------------------------------------------
  void get_transaction_prefix_hash(const transaction_prefix& tx, crypto::hash& h);
  crypto::hash get_transaction_prefix_hash(const transaction_prefix& tx);
  bool parse_and_validate_tx_from_blob(const blobdata& tx_blob, transaction& tx, crypto::hash& tx_hash, crypto::hash& tx_prefix_hash);
  bool parse_and_validate_tx_from_blob(const blobdata& tx_blob, transaction& tx);
  bool parse_and_validate_tx_base_from_blob(const blobdata& tx_blob, transaction& tx);
  bool encrypt_payment_id(crypto::hash8 &payment_id, const crypto::public_key &public_key, const crypto::secret_key &secret_key);
  bool decrypt_payment_id(crypto::hash8 &payment_id, const crypto::public_key &public_key, const crypto::secret_key &secret_key);

  template<typename T>
  bool find_tx_extra_field_by_type(const std::vector<tx_extra_field>& tx_extra_fields, T& field, size_t index = 0)
  {
    auto it = std::find_if(tx_extra_fields.begin(), tx_extra_fields.end(), [&index](const tx_extra_field& f) { return typeid(T) == f.type() && !index--; });
    if(tx_extra_fields.end() == it)
      return false;

    field = boost::get<T>(*it);
    return true;
  }

  bool parse_tx_extra(const std::vector<uint8_t>& tx_extra, std::vector<tx_extra_field>& tx_extra_fields);
  crypto::public_key get_tx_pub_key_from_extra(const std::vector<uint8_t>& tx_extra, size_t pk_index = 0);
  crypto::public_key get_tx_pub_key_from_extra(const transaction_prefix& tx, size_t pk_index = 0);
  crypto::public_key get_tx_pub_key_from_extra(const transaction& tx, size_t pk_index = 0);
  bool add_tx_pub_key_to_extra(transaction& tx, const crypto::public_key& tx_pub_key);

  /*!
   * \brief add_graft_tx_extra_to_extra - adds graft extra fields to tx
   * \param tx            - transaction fields to be added to
   * \param graft_extra   - graft fields to add
   * \return              - true on success
   */
  // TODO:  probably we will remove "..graft_tx_extra.." methods
  bool add_graft_tx_extra_to_extra(transaction &tx, const supernode::GraftTxExtra &graft_extra);

  /*!
   * @brief add_graft_tx_extra_to_extra - overloaded function to add graft extra directly to tx extra
   * @param extra                 - extra to add fields to
   * @param graft_extra           - graft fields to add
   * @return                      - true on success
   */
  bool add_graft_tx_extra_to_extra(std::vector<uint8_t>& extra, const supernode::GraftTxExtra &graft_extra);

  /*!
   * \brief get_graft_tx_extra_from_extra - read graft extra fields from tx extra
   * \param tx                    - source transaction
   * \param graft_tx_extra        - destination graft extra fields
   * \return                      - true on success
   */
  bool get_graft_tx_extra_from_extra(const transaction &tx, supernode::GraftTxExtra &graft_tx_extra);

  /*!
   * \brief add_graft_stake_tx_extra_to_extra - adds graft stake transaction fields to extra
   * \param extra                - extra to add fields to
   * \param supernode_public_id  - public identifier of supernode
   * \param supernode_wallet_id  - supernode public wallet address
   * \return                     - true on success
   */
  bool add_graft_stake_tx_extra_to_extra(std::vector<uint8_t> &extra, const std::string &supernode_public_id,
    const cryptonote::account_public_address &supernode_public_address, const crypto::signature& supernode_signature);

  /*!
   * \brief add_graft_tx_secret_key_to_extra - adds graft transaction secret key to extra
   * \param extra       - extra to add fields to
   * \param secret_key  - secret key
   * \return            - true on success
   */
  bool add_graft_tx_secret_key_to_extra(std::vector<uint8_t> &extra, const crypto::secret_key& secret_key);

  /*!
   * \brief get_graft_stake_tx_extra_from_extra - gets graft stake transaction fields from extra
   * \param tx                   - source transaction
   * \param supernode_public_id  - public identifier of supernode
   * \param supernode_wallet_id  - supernode public wallet address
   * \param tx_secret_key        - transaction secret key
   * \return                     - true on success
   */
  bool get_graft_stake_tx_extra_from_extra
    (const transaction &tx,
     std::string &supernode_public_id,
     cryptonote::account_public_address &supernode_public_address,
     crypto::signature &supernode_signature,
     crypto::secret_key &tx_secret_key);

  /*!
   * \brief add_graft_rta_header_to_extra - add rta_header to the extra
   * \param extra                         - extra to add fields to
   * \param rta_header                    - source rta_header
   * \return                              - true on success
   */
  bool add_graft_rta_header_to_extra(std::vector<uint8_t> &extra, const rta_header &rta_header);

  /*!
   * \brief get_graft_rta_header_from_extra - read rta_header from tx extra
   * \param tx                              - input transaction
   * \param rta_header                      - output rta_header
   * \return                                - true on success
   */
  bool get_graft_rta_header_from_extra(const transaction& tx, rta_header &rta_header);

  /*!
   * \brief add_graft_rta_signatures_to_extra - add rta_signatures to the 'extra' container
   * \param extra                             - 'extra' container
   * \param rta_signatures                    - rta signatures to add
   * \return                                  - true on success
   */
  bool add_graft_rta_signatures_to_extra2(std::vector<uint8_t> &extra, const std::vector<rta_signature> &rta_signatures);

  /*!
   * \brief get_graft_rta_signatures_from_extra2 - read rta signatures from tx extra2 field
   * \param tx                                   - input transaction
   * \param rta_signatures                       - output rta signatures
   * \return                                     - true on success
   */
  bool get_graft_rta_signatures_from_extra2(const transaction& tx, std::vector<rta_signature> &rta_signatures);

  bool add_extra_nonce_to_tx_extra(std::vector<uint8_t>& tx_extra, const blobdata& extra_nonce);
  bool remove_field_from_tx_extra(std::vector<uint8_t>& tx_extra, const std::type_info &type);
  void set_payment_id_to_tx_extra_nonce(blobdata& extra_nonce, const crypto::hash& payment_id);
  void set_encrypted_payment_id_to_tx_extra_nonce(blobdata& extra_nonce, const crypto::hash8& payment_id);
  bool get_payment_id_from_tx_extra_nonce(const blobdata& extra_nonce, crypto::hash& payment_id);
  bool get_encrypted_payment_id_from_tx_extra_nonce(const blobdata& extra_nonce, crypto::hash8& payment_id);
  bool is_out_to_acc(const account_keys& acc, const txout_to_key& out_key, const crypto::public_key& tx_pub_key, size_t output_index);
  bool is_out_to_acc_precomp(const crypto::public_key& spend_public_key, const txout_to_key& out_key, const crypto::key_derivation& derivation, size_t output_index);
  bool lookup_acc_outs(const account_keys& acc, const transaction& tx, const crypto::public_key& tx_pub_key, std::vector<size_t>& outs, uint64_t& money_transfered);
  bool lookup_acc_outs(const account_keys& acc, const transaction& tx, std::vector<size_t>& outs, uint64_t& money_transfered);
  bool get_tx_fee(const transaction& tx, uint64_t & fee);
  uint64_t get_tx_fee(const transaction& tx);
  bool generate_key_image_helper(const account_keys& ack, const crypto::public_key& tx_public_key, size_t real_output_index, keypair& in_ephemeral, crypto::key_image& ki);
  void get_blob_hash(const blobdata& blob, crypto::hash& res);
  crypto::hash get_blob_hash(const blobdata& blob);
  std::string short_hash_str(const crypto::hash& h);

  crypto::hash get_transaction_hash(const transaction& t);
  bool get_transaction_hash(const transaction& t, crypto::hash& res);
  bool get_transaction_hash(const transaction& t, crypto::hash& res, size_t& blob_size);
  bool get_transaction_hash(const transaction& t, crypto::hash& res, size_t* blob_size);
  bool calculate_transaction_hash(const transaction& t, crypto::hash& res, size_t* blob_size);
  blobdata get_block_hashing_blob(const block& b);
  bool calculate_block_hash(const block& b, crypto::hash& res);
  bool get_block_hash(const block& b, crypto::hash& res);
  crypto::hash get_block_hash(const block& b);
  bool get_block_longhash(const block& b, crypto::hash& res, uint64_t height);
  crypto::hash get_block_longhash(const block& b, uint64_t height);
  bool parse_and_validate_block_from_blob(const blobdata& b_blob, block& b);
  bool get_inputs_money_amount(const transaction& tx, uint64_t& money);
  uint64_t get_outs_money_amount(const transaction& tx);
  bool check_inputs_types_supported(const transaction& tx);
  bool check_outs_valid(const transaction& tx);
  bool parse_amount(uint64_t& amount, const std::string& str_amount);

  bool check_money_overflow(const transaction& tx);
  bool check_outs_overflow(const transaction& tx);
  bool check_inputs_overflow(const transaction& tx);
  uint64_t get_block_height(const block& b);
  std::vector<uint64_t> relative_output_offsets_to_absolute(const std::vector<uint64_t>& off);
  std::vector<uint64_t> absolute_output_offsets_to_relative(const std::vector<uint64_t>& off);
  void set_default_decimal_point(unsigned int decimal_point = CRYPTONOTE_DISPLAY_DECIMAL_POINT);
  unsigned int get_default_decimal_point();
  std::string get_unit(unsigned int decimal_point = -1);
  std::string print_money(uint64_t amount, unsigned int decimal_point = -1);
  //---------------------------------------------------------------
  template<class t_object>
  bool t_serializable_object_to_blob(const t_object& to, blobdata& b_blob)
  {
    std::stringstream ss;
    binary_archive<true> ba(ss);
    bool r = ::serialization::serialize(ba, const_cast<t_object&>(to));
    b_blob = ss.str();
    return r;
  }
  //---------------------------------------------------------------
  template<class t_object>
  blobdata t_serializable_object_to_blob(const t_object& to)
  {
    blobdata b;
    t_serializable_object_to_blob(to, b);
    return b;
  }
  //---------------------------------------------------------------
  template<class t_object>
  bool get_object_hash(const t_object& o, crypto::hash& res)
  {
    get_blob_hash(t_serializable_object_to_blob(o), res);
    return true;
  }
  //---------------------------------------------------------------
  template<class t_object>
  size_t get_object_blobsize(const t_object& o)
  {
    blobdata b = t_serializable_object_to_blob(o);
    return b.size();
  }
  //---------------------------------------------------------------
  template<class t_object>
  bool get_object_hash(const t_object& o, crypto::hash& res, size_t& blob_size)
  {
    blobdata bl = t_serializable_object_to_blob(o);
    blob_size = bl.size();
    get_blob_hash(bl, res);
    return true;
  }
  //---------------------------------------------------------------
  template <typename T>
  std::string obj_to_json_str(T& obj)
  {
    std::stringstream ss;
    json_archive<true> ar(ss, true);
    bool r = ::serialization::serialize(ar, obj);
    CHECK_AND_ASSERT_MES(r, "", "obj_to_json_str failed: serialization::serialize returned false");
    return ss.str();
  }
  //---------------------------------------------------------------
  // 62387455827 -> 455827 + 7000000 + 80000000 + 300000000 + 2000000000 + 60000000000, where 455827 <= dust_threshold
  template<typename chunk_handler_t, typename dust_handler_t>
  void decompose_amount_into_digits(uint64_t amount, uint64_t dust_threshold, const chunk_handler_t& chunk_handler, const dust_handler_t& dust_handler)
  {
    if (0 == amount)
    {
      return;
    }

    bool is_dust_handled = false;
    uint64_t dust = 0;
    uint64_t order = 1;
    while (0 != amount)
    {
      uint64_t chunk = (amount % 10) * order;
      amount /= 10;
      order *= 10;

      if (dust + chunk <= dust_threshold)
      {
        dust += chunk;
      }
      else
      {
        if (!is_dust_handled && 0 != dust)
        {
          dust_handler(dust);
          is_dust_handled = true;
        }
        if (0 != chunk)
        {
          chunk_handler(chunk);
        }
      }
    }

    if (!is_dust_handled && 0 != dust)
    {
      dust_handler(dust);
    }
  }
  //---------------------------------------------------------------
  blobdata block_to_blob(const block& b);
  bool block_to_blob(const block& b, blobdata& b_blob);
  blobdata tx_to_blob(const transaction& b);
  bool tx_to_blob(const transaction& b, blobdata& b_blob);
  void get_tx_tree_hash(const std::vector<crypto::hash>& tx_hashes, crypto::hash& h);
  crypto::hash get_tx_tree_hash(const std::vector<crypto::hash>& tx_hashes);
  crypto::hash get_tx_tree_hash(const block& b);
  bool is_valid_decomposed_amount(uint64_t amount);
  void get_hash_stats(uint64_t &tx_hashes_calculated, uint64_t &tx_hashes_cached, uint64_t &block_hashes_calculated, uint64_t & block_hashes_cached);

#define CHECKED_GET_SPECIFIC_VARIANT(variant_var, specific_type, variable_name, fail_return_val) \
  CHECK_AND_ASSERT_MES(variant_var.type() == typeid(specific_type), fail_return_val, "wrong variant type: " << variant_var.type().name() << ", expected " << typeid(specific_type).name()); \
  specific_type& variable_name = boost::get<specific_type>(variant_var);

}
