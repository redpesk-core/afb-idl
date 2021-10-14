#pragma once

{{#apis}}
/****************************************************************************************
*** CALL INTERFACE FOR API {{apiname}} FROM {{project-name}}
****************************************************************************************/
{{#verbs}}

/* CALLING VERB {{verbname}} */
typedef void call_{{apiname}}_{{verbname}}_cb_t(
	void *closure,
	int status,
	{{response.typename}}_{{response.typetype}}_t response
);

void call_{{apiname}}_{{verbname}}(
	afb_api_t fromapi,
	flatcc_builder_t *builder,
	call_{{apiname}}_{{verbname}}_cb_t *callback,
	void *closure
);

void call_{{apiname}}_{{verbname}}_{{request.typetype}}(
	afb_api_t fromapi,
	{{request.typename}}_{{request.typetype}}_t request,
	call_{{apiname}}_{{verbname}}_cb_t *callback,
	void *closure
);

/* SUBCALLING VERB {{verbname}} */
typedef void subcall_{{apiname}}_{{verbname}}_cb_t(
	void *closure,
	int status,
	{{response.typename}}_{{response.typetype}}_t response,
	afb_req_t req
);

void subcall_{{apiname}}_{{verbname}}(
	afb_req_t req,
	flatcc_builder_t *builder,
	int flags,
	subcall_{{apiname}}_{{verbname}}_cb_t *callback,
	void *closure
);

void subcall_{{apiname}}_{{verbname}}_{{request.typetype}}(
	afb_req_t req,
	{{request.typename}}_{{request.typetype}}_t request,
	int flags,
	subcall_{{apiname}}_{{verbname}}_cb_t *callback,
	void *closure
);

{{/verbs}}
{{/apis}}

