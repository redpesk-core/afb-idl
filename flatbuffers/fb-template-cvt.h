#pragma once

typedef struct afb_data_x4 * afb_data_t;

{{#types}}

extern
int
{{typename}}_from_afb_data(
	afb_data_t indata,
	afb_data_t *outholder);

extern
int
{{typename}}_to_afb_data_b(
	afb_data_t indata,
	afb_data_t *outdata);

extern
int
{{typename}}_to_afb_data_j(
	afb_data_t indata,
	afb_data_t *outdata);
{{/types}}
