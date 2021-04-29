#include "{{project_name}}_flatcc.h"
#include "{{project_name}}-fbcvt.h"

#define AFB_BINDING_VERSION 4
#include <afb/afb-binding.h>
#include <errno.h>

#ifdef DEBUG_PRINT
#include "flatcc/support/hexdump.h"
#include <stdio.h>
#endif

{{#types}}

/****************************************************************************************
*** SERIALISATION OF {{typename}}
****************************************************************************************/

int {{typename}}_from_afb_data(afb_data_t indata, afb_data_t *outholder)
{
	// {{typename}}_table_t tbl;
	void *ptr;
	size_t sz;
	int rc;
#if !NOJSONPARSE
	flatcc_builder_t builder;
	flatcc_json_parser_t parser;
	flatcc_builder_ref_t fbref;
	afb_data_t datastr;
	const char *parseout;
#endif

	/* is the input data a bytearray ? */
	if (afb_data_type(indata) == AFB_PREDEFINED_TYPE_BYTEARRAY) {
		/* yes, it is a byte array, retrieve address and size */
		rc = afb_data_get_constant(indata, &ptr, &sz);
		if (rc < 0)
			goto error; /* failed */
#if !NOFBVERIF
		/* verify the validity of the bytearray according to type {{typename}} */
		if ({{typename}}_verify_as_typed_root(ptr, sz)) {
			rc = -EINVAL;
			goto error; /* failed */
		}
#endif
		/* holder of bytearray is the input data */
		*outholder = afb_data_addref(indata);
		rc = 0;
		goto end;
	}

#if !NOJSONPARSE
	/* trying JSON. First, can match a string? */
	rc = afb_data_convert(indata, AFB_PREDEFINED_TYPE_STRINGZ, &datastr);
	if (rc < 0)
		goto error;

	/* get address and length of the string */
	rc = afb_data_get_constant(datastr, &ptr, &sz);
	if (rc < 0)
		goto error2;

	/* build a memory representation of type {{typename}} for the parsed JSON */
	flatcc_builder_init(&builder);
	flatcc_json_parser_init(&parser, &builder, ptr, &((char*)ptr)[sz - 1], 0);
	if (flatcc_builder_start_buffer(&builder, {{typename}}_type_identifier, 0, 0))
		goto error3;
	{{typename}}_parse_json_table(&parser, ptr, (char*)ptr+(sz-1), &fbref);
	if (parser.error != flatcc_json_parser_error_ok) {
		/* failed to parse the JSON for type {{typename}} */
		AFB_DEBUG("Fail to parse {{typename}} json : %s", flatcc_json_parser_error_string(parser.error));
		rc = -EINVAL;
		goto error3;
	}
	if (!flatcc_builder_end_buffer(&builder, fbref))
		goto error3;

	/* retrieve the created buffer */
	ptr = flatcc_builder_finalize_buffer(&builder, &sz);
	if (ptr == NULL) {
		rc = -ENOMEM;
		goto error3;
	}

#ifdef DEBUG_PRINT
	hexdump("{{typename}} buffer", ptr, sz, stderr);
#endif
	/* create the data holding that memory buffer */
	rc = afb_create_data_raw(outholder, AFB_PREDEFINED_TYPE_BYTEARRAY,
			ptr, sz, free, ptr);
	if (rc < 0)
		goto error3;

	/* cleanup */
	flatcc_builder_clear(&builder);
	afb_data_unref(datastr);
	goto end;

error3:
	flatcc_builder_clear(&builder);
error2:
	afb_data_unref(datastr);
#endif
error:
	*outholder = NULL;
end:
	return rc;
}

int {{typename}}_to_afb_data_b(afb_data_t indata, afb_data_t *outdata)
{
	/* Probably not necessary here */
#if !NOFBVERIF
	size_t sz;
	void * fbuf;
	
	int rc = afb_data_get_constant(indata, &fbuf, &sz);
	
	if (rc){
		*outdata = 0;
		return rc;
	}

	/* verify the validity of the bytearray according to type {{typename}} */
	if ({{typename}}_verify_as_typed_root(fbuf, sz)) {
		*outdata = 0;
		return -EINVAL;
	}
#endif

	*outdata = afb_data_addref(indata);

	return 0;
}

int {{typename}}_to_afb_data_j(afb_data_t indata, afb_data_t *outdata)
{
	size_t sz;
	void * fbuf;
	void * jbuf;
	int rc;

	rc = afb_data_get_constant(indata, &fbuf, &sz);
	if (rc)
		goto error;

	/* Probably not necessary here */
#if !NOFBVERIF
	/* verify the validity of the bytearray according to type {{typename}} */
	if ({{typename}}_verify_as_typed_root(fbuf, sz)) {
		rc = -EINVAL;
		goto error; /* failed */
	}
#endif
	/* convert flatbuffer to json */
	flatcc_json_printer_t cvt;
	flatcc_json_printer_init_dynamic_buffer(&cvt, sz*5);
	{{typename}}_print_json_as_root(&cvt, fbuf, sz, {{typename}}_type_identifier);
	jbuf = flatcc_json_printer_finalize_dynamic_buffer(&cvt, &sz);
	
	if (jbuf == NULL){
		rc = -EPROTO; // probably a diferent error code would be preferable
		goto error;
	}

	/* create the data holding that memory buffer */
	rc = afb_create_data_raw(outdata, AFB_PREDEFINED_TYPE_JSON,
			jbuf, sz, free, jbuf);
	if (rc < 0)
		goto error;

	return 0;
error:
	*outdata = 0;
	return rc;
}

{{/types}}
