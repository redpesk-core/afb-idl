
#pragma once

{{#types}}
/****************************************************************************************
*** BASIC OPERATIONS ON FLATBUFFER TYPE {{typename}} FROM {{project-name}}
****************************************************************************************/
extern int {{typename}}_is_in_afb_data(afb_data_t data);
extern int {{typename}}_from_afb_data(afb_data_t indata, afb_data_t *outdata);
extern {{typename}}_{{typetype}}_t {{typename}}_unwrap(afb_data_t data);
extern {{typename}}_{{typetype}}_t {{typename}}_unwrap_unchecked(afb_data_t data);
{{#generate-json}}
extern int {{typename}}_from_afb_data_json(afb_data_t indata, afb_data_t *outdata);
extern int {{typename}}_to_afb_data_json(afb_data_t indata, afb_data_t *outdata);
{{/generate-json}}

{{/types}}