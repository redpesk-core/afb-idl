#include "{{project_name}}_flatcc.h"
#include "{{project_name}}_fbcli.h"

{{#apis}}
{{#verbs}}



void {{apiname}}_{{verbname}}_subcall(afb_req_t afbreq, {{request}}_table_t request, ...)
{
}

{{/verbs}}

{{/apis}}

