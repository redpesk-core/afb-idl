#include "{{project_name}}-flatcc.h"
#include "{{project_name}}-fbcli.h"

{{#apis}}
{{#verbs}}



void {{apiname}}_{{verbname}}_subcall(afb_req_t afbreq, {{request}}_table_t request, ...)
{
}

{{/verbs}}

{{/apis}}

