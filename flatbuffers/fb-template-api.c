#include "{{project_name}}_flatcc.h"
#include "{{project_name}}-fbcvt.h"

#define AFB_BINDING_VERSION 4
#include <afb/afb-binding.h>


{{#apis}}
{{#verbs}}

void {{apiname}}_{{verbname}}_process_reply_cb(
		void *closure,
		int status,
		unsigned nreplies,
		afb_data_x4_t const replies[],
		afb_req_x4_t req)
{

}

int {{apiname}}_{{verbname}}_process(afb_data_t holder, afb_req_t req)
{
	return 0;
}

{{/verbs}}

{{#verbs}}

static void {{apiname}}_{{verbname}}_verb_cb(afb_req_t afbreq, unsigned argc, afb_data_t const argv[])
{
	afb_data_t dataholder;
	int rc;

	if (argc != 1)
		AFB_REQ_ERROR(afbreq, "\"{{verbname}}\" expected 1 argument");

	else {
		rc = {{request}}_from_afb_data(argv[0], &dataholder);
		if (rc < 0)
			AFB_REQ_ERROR(afbreq, "\"{{verbname}}\" BAD argument");
		else {
			rc = {{apiname}}_{{verbname}}_process(dataholder, afbreq);
			if (rc >= 0)
				return;
			
			afb_req_reply(afbreq, rc, 0, 0);
			afb_data_unref(dataholder);
			return;
		}
	}
	afb_req_reply(afbreq, AFB_ERRNO_INVALID_REQUEST, 0, 0);
}

{{/verbs}}



static const struct afb_verb_v4 {{apiname}}_verbs[] = {
{{#verbs}}	{ .verb = "{{verbname}}", .callback = {{apiname}}_{{verbname}}_verb_cb },
{{/verbs}}	{ .verb = 0 }
};

const struct afb_binding_v4 afbBindingV4 = {
	.api = "{{apiname}}",
	.verbs = {{apiname}}_verbs
};

{{/apis}}

