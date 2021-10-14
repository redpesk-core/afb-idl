
#define AFB_BINDING_VERSION 4
#include <afb/afb-binding.h>

#include "{{project-name}}-flatcc.h"
#include "{{project-name}}-fbcli.h"
#include "{{project-name}}-fbcvt.h"

{{#apis}}
{{#verbs}}

typedef void call_{{apiname}}_{{verbname}}_cb_t(
	void *closure,
	int status,
	{{response.typename}}_{{response.typetype}}_t response
);

static void __call_{{apiname}}_{{verbname}}_callback__(
	void *closure,
	int status,
	unsigned nreplies,
	afb_data_t const replies[],
	afb_api_t api
) {
	{{response.typename}}_{{response.typetype}}_t response = 0;
	struct { call_{{apiname}}_{{verbname}}_cb_t *cb; void *clo; } *nclo = closure;
	if (nreplies >= 1)
		response = {{response.typename}}_unwrap(replies[0]);
	nclo->cb(nclo->clo, status, response);
	free(nclo);
}

void call_{{apiname}}_{{verbname}}(
	afb_api_t fromapi,
	flatcc_builder_t *builder,
	call_{{apiname}}_{{verbname}}_cb_t *callback,
	void *closure
) {
	struct { call_{{apiname}}_{{verbname}}_cb_t *cb; void *clo; } *nclo;
	void *ptr;
	size_t sz;
	int rc;
	afb_data_t data;

	nclo = malloc(sizeof *nclo);
	if (nclo) {
		nclo->cb = callback;
		nclo->clo = closure;

		ptr = flatcc_builder_finalize_buffer(builder, &sz);
		if (ptr) {
			rc = afb_create_data_raw(&data, AFB_PREDEFINED_TYPE_BYTEARRAY,
					ptr, sz, free, ptr);
			if (rc >= 0)
				return afb_api_call(
					fromapi,
					"{{apiname}}",
					"{{verbname}}",
					1,
					&data,
					__call_{{apiname}}_{{verbname}}_callback__,
					nclo);
		}
		free(nclo);
	}
	callback(closure, AFB_ERRNO_OUT_OF_MEMORY, 0);
}

void call_{{apiname}}_{{verbname}}_{{request.typetype}}(
	afb_api_t fromapi,
	{{request.typename}}_{{request.typetype}}_t request,
	call_{{apiname}}_{{verbname}}_cb_t *callback,
	void *closure
)
{
	flatbuffers_buffer_ref_t bref;
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	bref = {{request.typename}}_clone_as_typed_root(&builder, request);
	if (bref)
		call_{{apiname}}_{{verbname}}(fromapi, &builder, callback, closure);
	flatcc_builder_clear(&builder);
	if (!bref)
		callback(closure, AFB_ERRNO_OUT_OF_MEMORY, 0);
}

typedef void subcall_{{apiname}}_{{verbname}}_cb_t(
	void *closure,
	int status,
	{{response.typename}}_{{response.typetype}}_t response,
	afb_req_t req
);

static void __subcall_{{apiname}}_{{verbname}}_callback__(
	void *closure,
	int status,
	unsigned nreplies,
	afb_data_t const replies[],
	afb_req_t req
) {
	{{response.typename}}_{{response.typetype}}_t response = 0;
	struct { subcall_{{apiname}}_{{verbname}}_cb_t *cb; void *clo; } *nclo = closure;
	if (nreplies >= 1)
		response = {{response.typename}}_unwrap(replies[0]);
	nclo->cb(nclo->clo, status, response, req);
	free(nclo);
}

void subcall_{{apiname}}_{{verbname}}(
	afb_req_t req,
	flatcc_builder_t *builder,
	int flags,
	subcall_{{apiname}}_{{verbname}}_cb_t *callback,
	void *closure
) {
	struct { subcall_{{apiname}}_{{verbname}}_cb_t *cb; void *clo; } *nclo;
	void *ptr;
	size_t sz;
	int rc;
	afb_data_t data;

	nclo = malloc(sizeof *nclo);
	if (nclo) {
		nclo->cb = callback;
		nclo->clo = closure;

		ptr = flatcc_builder_finalize_buffer(builder, &sz);
		if (ptr) {
			rc = afb_create_data_raw(&data, AFB_PREDEFINED_TYPE_BYTEARRAY,
					ptr, sz, free, ptr);
			if (rc >= 0)
				return afb_req_subcall(
					req,
					"{{apiname}}",
					"{{verbname}}",
					1,
					&data,
					flags,
					__subcall_{{apiname}}_{{verbname}}_callback__,
					nclo);
		}
		free(nclo);
	}
	callback(closure, AFB_ERRNO_OUT_OF_MEMORY, 0, req);
}

void subcall_{{apiname}}_{{verbname}}_{{request.typetype}}(
	afb_req_t req,
	{{request.typename}}_{{request.typetype}}_t request,
	int flags,
	subcall_{{apiname}}_{{verbname}}_cb_t *callback,
	void *closure
)
{
	flatbuffers_buffer_ref_t bref;
	flatcc_builder_t builder;
	flatcc_builder_init(&builder);
	bref = {{request.typename}}_clone_as_typed_root(&builder, request);
	if (bref)
		subcall_{{apiname}}_{{verbname}}(req, &builder, flags, callback, closure);
	flatcc_builder_clear(&builder);
	if (!bref)
		callback(closure, AFB_ERRNO_OUT_OF_MEMORY, 0, req);
}

{{/verbs}}
{{/apis}}

