#define AFB_BINDING_VERSION 4
#include <afb/afb-binding.h>

#include "{{project-name}}-flatcc.h"
#include "{{project-name}}-fbcvt.h"
#include "{{project-name}}-fbapi.h"
{{! *************************************************************************** }}
{{! **** GENERATE AS MANY API AS ARE RPC SERVICES DESCRIBED IN THE SCHEMA ***** }}
{{! *************************************************************************** }}
{{#apis}}
{{! *************************************************************************** }}
{{! **** FOR EACH PROC OF A SERVICE GENERATES A VERB                      ***** }}
{{! *************************************************************************** }}
{{#verbs}}

void {{apiname}}_{{verbname}}_reply(
		afb_req_t req,
		int status,
		afb_data_t reply)
{
	afb_data_t data = reply;
	unsigned ndata = afb_data_is_valid(reply);
{{#generate-json}}
	if (ndata) {
		afb_data_t const *params;
		afb_req_parameters(req, &params);
		if (!{{request.typename}}_is_in_afb_data(params[0])) {
			 /* reply json when query is not binary */
			int rc = {{response.typename}}_to_afb_data_json(reply, &data);
			afb_data_unref(reply);
			if (rc < 0) {
				AFB_REQ_ERROR(req, "\"{{verbname}}\" can't convert to JSON");
				status = rc;
				ndata = 0;
			}
		}
	}
{{/generate-json}}
	afb_req_reply(req, status, ndata, &data);
}

/**
 * Template verb implementation for RPC of flatbuffer service.
 *
 * Schema:      {{project-name}}
 * RPC service: {{apiname}}
 * Procedure:   {{verbname}}
 */
static void {{apiname}}_{{verbname}}_verb_cb(afb_req_t afbreq, unsigned argc, afb_data_t const argv[])
{
	afb_data_t data;
	int rc;

	if (argc != 1)
		AFB_REQ_ERROR(afbreq, "\"{{verbname}}\" expected 1 argument");

	else {
		rc = {{request.typename}}_from_afb_data(argv[0], &data);
		if (rc >= 0) {
			{{apiname}}_{{verbname}}_process(afbreq, data);
			return;
		}
		AFB_REQ_ERROR(afbreq, "\"{{verbname}}\" BAD argument");
	}
	afb_req_reply(afbreq, AFB_ERRNO_INVALID_REQUEST, 0, 0);
}
{{/verbs}}
{{#has-permissions}}

static const struct afb_auth {{apiname}}_permissions[] = {
{{#permissions}}
	{
		.type = afb_auth_Permission,
		.text = "{{.}}"
	},
{{/permissions}}
};
{{/has-permissions}}

static const struct afb_verb_v4 {{apiname}}_verbs[] = {
{{#verbs}}
	{
		.verb = "{{verbname}}",
	{{#permission}}
		.auth = &{{apiname}}_permissions[{{index}}], /* permission "{{name}}"" */
	{{/permission}}
		.callback = {{apiname}}_{{verbname}}_verb_cb
	},
{{/verbs}}
	{ .verb = 0 }
};

const struct afb_binding_v4 afbBindingV4 = {
	.api = "{{apiname}}",
	.verbs = {{apiname}}_verbs
};
{{/apis}}
