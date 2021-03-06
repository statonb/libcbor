/*
 * Copyright (c) 2014-2017 Pavel Kalvoda <me@pavelkalvoda.com>
 *
 * libcbor is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include "cbor.h"
#include "assertions.h"
#include <inttypes.h>

cbor_item_t *tag;
struct cbor_load_result res;


unsigned char embedded_tag_data[] = {0xC0, 0x00};

/* Tag 0 + uint 0 */
static void test_embedded_tag(void **state)
{
	tag = cbor_load(embedded_tag_data, 2, &res);
	assert_true(cbor_typeof(tag) == CBOR_TYPE_TAG);
	assert_true(cbor_tag_value(tag) == 0);
	assert_uint8(cbor_tag_item(tag), 0);
	cbor_decref(&tag);
	assert_null(tag);
}

unsigned char int8_tag_data[] = {0xD8, 0xFF, 0x01};

/* Tag 255 + uint 1 */
static void test_int8_tag(void **state)
{
	tag = cbor_load(int8_tag_data, 3, &res);
	assert_true(cbor_typeof(tag) == CBOR_TYPE_TAG);
	assert_true(cbor_tag_value(tag) == 255);
	assert_uint8(cbor_tag_item(tag), 1);
	cbor_decref(&tag);
	assert_null(tag);
}

unsigned char int16_tag_data[] = {0xD9, 0xFF, 0x00, 0x02};

/* Tag 255 << 8 + uint 2 */
static void test_int16_tag(void **state)
{
	tag = cbor_load(int16_tag_data, 4, &res);
	assert_true(cbor_typeof(tag) == CBOR_TYPE_TAG);
	assert_true(cbor_tag_value(tag) == 255 << 8);
	assert_uint8(cbor_tag_item(tag), 2);
	cbor_decref(&tag);
	assert_null(tag);
}


unsigned char int32_tag_data[] = {0xDA, 0xFF, 0x00, 0x00, 0x00, 0x03};

/* uint 3 */
static void test_int32_tag(void **state)
{
	tag = cbor_load(int32_tag_data, 6, &res);
	assert_true(cbor_typeof(tag) == CBOR_TYPE_TAG);
	assert_true(cbor_tag_value(tag) == 4278190080ULL);
	assert_uint8(cbor_tag_item(tag), 3);
	cbor_decref(&tag);
	assert_null(tag);
}


unsigned char int64_tag_data[] = {0xDB, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04};

/* uint 4 */
static void test_int64_tag(void **state)
{
	tag = cbor_load(int64_tag_data, 10, &res);
	assert_true(cbor_typeof(tag) == CBOR_TYPE_TAG);
	assert_true(cbor_tag_value(tag) == 18374686479671623680ULL);
	assert_uint8(cbor_tag_item(tag), 4);
	cbor_decref(&tag);
	assert_null(tag);
}


unsigned char nested_tag_data[] = {0xC0, 0xC1, 0x18, 0x2A};

/* Tag 0, tag 1 + uint 0 */
static void test_nested_tag(void **state)
{
	tag = cbor_load(nested_tag_data, 4, &res);
	assert_true(cbor_typeof(tag) == CBOR_TYPE_TAG);
	assert_true(cbor_tag_value(tag) == 0);
	assert_true(cbor_typeof(cbor_tag_item(tag)) == CBOR_TYPE_TAG);
	assert_true(cbor_tag_value(cbor_tag_item(tag)) == 1);
	assert_uint8(cbor_tag_item(cbor_tag_item(tag)), 42);
	cbor_decref(&tag);
	assert_null(tag);
}


int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_embedded_tag),
		cmocka_unit_test(test_int8_tag),
		cmocka_unit_test(test_int16_tag),
		cmocka_unit_test(test_int32_tag),
		cmocka_unit_test(test_int64_tag),
		cmocka_unit_test(test_nested_tag)
	};
	return cmocka_run_group_tests(tests, NULL, NULL);
}
