#pragma once

{{#apis}}
/****************************************************************************************
*** VERBS INTERFACE FOR API {{apiname}} FROM {{project-name}}
****************************************************************************************/
{{#verbs}}

/* FOR VERB {{verbname}} */
extern void {{apiname}}_{{verbname}}_reply(afb_req_t req, int status, afb_data_t data);
extern void {{apiname}}_{{verbname}}_process(afb_req_t req, afb_data_t data);
{{/verbs}}

{{/apis}}