/*
 * Copyright (c) 2018 Yubico AB. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef _EXTERN_H
#define _EXTERN_H

/* aes256 */
int aes256_cbc_dec(const fido_blob_t *, const fido_blob_t *, fido_blob_t *);
int aes256_cbc_enc(const fido_blob_t *, const fido_blob_t *, fido_blob_t *);

/* cbor encoding functions */
cbor_item_t *encode_assert_options(bool, bool);
cbor_item_t *encode_change_pin_auth(const fido_blob_t *, const fido_blob_t *,
    const fido_blob_t *);
cbor_item_t *encode_options(bool, bool);
cbor_item_t *encode_pin_auth(const fido_blob_t *, const fido_blob_t *);
cbor_item_t *encode_pin_enc(const fido_blob_t *, const fido_blob_t *);
cbor_item_t *encode_pin_hash_enc(const fido_blob_t *, const fido_blob_t *);
cbor_item_t *encode_pin_opt(void);
cbor_item_t *encode_pubkey_list(const fido_blob_array_t *);
cbor_item_t *encode_pubkey_param(void);
cbor_item_t *encode_rp_entity(const fido_rp_t *);
cbor_item_t *encode_set_pin_auth(const fido_blob_t *, const fido_blob_t *);
cbor_item_t *encode_user_entity(const fido_user_t *);

/* cbor decoding functions */
int decode_attcred(const unsigned char **, size_t *, fido_attcred_t *);
int decode_attstmt(const cbor_item_t *, fido_attstmt_t *);
int decode_attstmt_entry(const cbor_item_t *, const cbor_item_t *, void *);
int decode_authdata(const cbor_item_t *, fido_blob_t *, fido_authdata_t *,
    fido_attcred_t *);
int decode_cred_id(const cbor_item_t *, fido_blob_t *);
int decode_fmt(const cbor_item_t *, char **);
int decode_uint64(const cbor_item_t *, uint64_t *);
int decode_user(const cbor_item_t *, fido_user_t *);
int decode_x5c(const cbor_item_t *, void *);

/* auxiliary cbor routines */
int cbor_add_bool(cbor_item_t *, const char *, bool);
int cbor_add_bytestring(cbor_item_t *, const char *, const unsigned char *,
    size_t);
int cbor_add_string(cbor_item_t *, const char *, const char *);
int cbor_array_iter(const cbor_item_t *, void *, int(*)(const cbor_item_t *,
    void *));
int cbor_build_frame(uint8_t, cbor_item_t *[], size_t, fido_blob_t *);
int cbor_bytestring_copy(const cbor_item_t *, unsigned char **, size_t *);
int cbor_map_iter(const cbor_item_t *, void *, int(*)(const cbor_item_t *,
    const cbor_item_t *, void *));
int cbor_string_copy(const cbor_item_t *, char **);
int parse_cbor_reply(const unsigned char *, size_t, void *,
    int(*)(const cbor_item_t *, const cbor_item_t *, void *));
int add_cbor_pin_params(fido_dev_t *, const fido_blob_t *, const char *,
    cbor_item_t **, cbor_item_t **);

/* buf */
int buf_read(const unsigned char **, size_t *, void *, size_t);
int buf_write(unsigned char **, size_t *, const void *, size_t);

/* hid i/o wrappers */
void *fido_hid_open_wrapper(const char *);
void  fido_hid_close_wrapper(void *);
int   fido_hid_read_wrapper(void *, unsigned char *, size_t, int);
int   fido_hid_write_wrapper(void *, const unsigned char *, size_t);

/* generic i/o */
int rx(fido_dev_t *, uint8_t, void *, size_t, int);
int tx(fido_dev_t *, uint8_t, const void *, size_t);

/* u2f */
int u2f_register(fido_dev_t *, fido_cred_t *, int);
int u2f_authenticate(fido_dev_t *, fido_assert_t *, int);

/* unexposed fido ops */
int fido_dev_authkey(fido_dev_t *, es256_pk_t *);
int fido_dev_get_pin_token(fido_dev_t *, const char *, const fido_blob_t *,
    const es256_pk_t *, fido_blob_t *);
int fido_do_ecdh(fido_dev_t *, es256_pk_t **, fido_blob_t **);

/* misc */
void fido_assert_reset_rx(fido_assert_t *);
void fido_assert_reset_tx(fido_assert_t *);
void fido_cred_reset_rx(fido_cred_t *);
void fido_cred_reset_tx(fido_cred_t *);

int debug;

#endif /* !_EXTERN_H */
