/*
 * Copyright (C) 2016-2021 IoT.bzh Company
 * Author: José Bollo <jose.bollo@iot.bzh>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <json-c/json.h>

enum { UNSET, YES, NO };

char *name = 0;
char *root = 0;
int wjso = UNSET;

/* if value is not zero add to object a field of key and value */
void add(struct json_object *object, const char *key, struct json_object *value)
{
	if (value)
		json_object_object_add(object, key, value);
}

/* search in the array of types the type of the given name */
struct json_object *gettype_str(struct json_object *types, const char *name)
{
	size_t ityp, ntyps;
	struct json_object *type, *tname;

	/* linear cearch in the array */
	ntyps = json_object_array_length(types);
	for (ityp = 0 ; ityp < ntyps ; ityp++) {
		type = json_object_array_get_idx(types, ityp);
		tname = json_object_object_get(type, "typename");
		if (!strcmp(json_object_get_string(tname), name))
			return type;
	}
	return 0;
}

/* search in the array of types the type of the given name */
struct json_object *gettype_obj(struct json_object *types, struct json_object *name)
{
	if (!json_object_is_type(name, json_type_string))
		return 0;
	return gettype_str(types, json_object_get_string(name));
}

/* add the type described by desc in the array of types */
void addtype(struct json_object *types, struct json_object *desc)
{
	size_t ityp, ntyps;
	struct json_object *type, *isstruct, *name;
	enum { type_table, type_struct } typetype;

	/* check if the type is already recorded or not */
	name = json_object_object_get(desc, "name");
	if (!json_object_is_type(name, json_type_string))
		return;
	type = gettype_str(types, json_object_get_string(name));
	if (type)
		return;

	/* not recorded, create it */
	type = json_object_new_object();
	add(type, "typename", json_object_get(name));
	isstruct = json_object_object_get(desc, "is_struct");
	if (json_object_is_type(isstruct, json_type_boolean)
	 && json_object_get_boolean(isstruct))
		typetype = type_struct;
	else
		typetype = type_table;
	add(type, "is-table", json_object_new_boolean(typetype == type_table));
	add(type, "is-struct", json_object_new_boolean(typetype == type_struct));
	add(type, "typetype", json_object_new_string(typetype == type_struct ? "struct" : "table"));

	/* record it */
	json_object_array_add(types, type);
}

/* search in the spec the object of the given name */
void addtypemsg(struct json_object *spec, struct json_object *types, struct json_object *msg)
{
	const char *namestr;
	size_t iobj, nobjs;
	struct json_object *obj, *objects, *name;

	/* get the typename of the message */
	name = json_object_object_get(msg, "name");
	objects = json_object_object_get(spec, "objects");
	if (!json_object_is_type(name, json_type_string)
	 || !json_object_is_type(objects, json_type_array))
		return;
	namestr = json_object_get_string(name);

	/* linear search in the array */
	nobjs = json_object_array_length(objects);
	for (iobj = 0 ; iobj < nobjs ; iobj++) {
		obj = json_object_array_get_idx(objects, iobj);
		name = json_object_object_get(obj, "name");
		if (!strcmp(json_object_get_string(name), namestr))
			return addtype(types, obj);
	}
}

/* record the types required by services */
void record_types_for_services(struct json_object *spec, struct json_object *types)
{
	size_t isrv, nsrvs, icall, ncalls;
	struct json_object *services, *service, *calls, *call, *request, *response;

	/* get the services */
	services = json_object_object_get(spec, "services");
	if (json_object_is_type(services, json_type_array))
		nsrvs = json_object_array_length(services);
	else
		nsrvs = 0;

	/* iterate on services */
	for(isrv = 0 ; isrv < nsrvs ; isrv++) {
		service = json_object_array_get_idx(services, isrv);
		calls = json_object_object_get(service, "calls");
		if (json_object_is_type(calls, json_type_array)) {
			ncalls = json_object_array_length(calls);
			for (icall = 0 ; icall < ncalls ; icall++) {
				call = json_object_array_get_idx(calls, icall);
				request = json_object_object_get(call, "request");
				addtypemsg(spec, types, request);
				response = json_object_object_get(call, "response");
				addtypemsg(spec, types, response);
			}
		}
	}
}

/* record the types tagged with the attribute event */
void record_types_for_events(struct json_object *spec, struct json_object *types)
{
	size_t iobj, nobjs, iattr, nattrs;
	struct json_object *objects, *object, *attrs, *attr, *key;

	/* get the services */
	objects = json_object_object_get(spec, "objects");
	if (json_object_is_type(objects, json_type_array))
		nobjs = json_object_array_length(objects);
	else
		nobjs = 0;

	/* iterate on services */
	for(iobj = 0 ; iobj < nobjs ; iobj++) {

		object = json_object_array_get_idx(objects, iobj);
		attrs = json_object_object_get(object, "attributes");
		if (json_object_is_type(attrs, json_type_array))
			nattrs = json_object_array_length(attrs);
		else
			nattrs = 0;
		for (iattr = 0 ; iattr < nattrs ; iattr++) {
			attr = json_object_array_get_idx(attrs, iattr);
			key = json_object_object_get(attr, "key");
			if (!strcmp(json_object_get_string(key), "event"))
				addtype(types, object);
		}
	}
}

/* record the types potentially used in event or in RPC */
void record_types(struct json_object *spec, struct json_object *prod)
{
	struct json_object *types = json_object_new_array();
	add(prod, "types", types);
	record_types_for_services(spec, types);
	record_types_for_events(spec, types);
}

/* record the permission of given name and return its index */
size_t record_perm(struct json_object *perms, struct json_object *name)
{
	size_t iperm, nperms;
	struct json_object *perm, *pname;

	nperms = json_object_array_length(perms);
	for (iperm = 0 ; iperm < nperms ; iperm++) {
		pname = json_object_array_get_idx(perms, iperm);
		if (!strcmp(json_object_get_string(pname), json_object_get_string(name)))
			return iperm;
	}
	json_object_array_add(perms, json_object_get(name));
	return nperms;
}

/* records the apis, one per services */
void record_apis(struct json_object *spec, struct json_object *prod)
{
	size_t isrv, nsrvs, icall, ncalls, iattr, nattrs, iperm;
	json_object *services, *service, *calls, *call, *request, *response, *attrs, *attr, *key, *value;
	json_object *apis, *api, *verbs, *verb, *permissions, *perm, *types, *type;

	apis = json_object_new_array();
	add(prod, "apis", apis);
	types = json_object_object_get(prod, "types");

	/* get the services */
	services = json_object_object_get(spec, "services");
	if (json_object_is_type(services, json_type_array))
		nsrvs = json_object_array_length(services);
	else
		nsrvs = 0;

	/* iterate on services */
	for(isrv = 0 ; isrv < nsrvs ; isrv++) {

		/* create one api per service */
		service = json_object_array_get_idx(services, isrv);
		permissions = NULL;
		api = json_object_new_object();
		verbs = json_object_new_array();
		add(api, "apiname", json_object_get(json_object_object_get(service, "name")));
		add(api, "verbs", verbs);
		json_object_array_add(apis, api);

		/* create one verb per call */
		calls = json_object_object_get(service, "calls");
		if (json_object_is_type(calls, json_type_array)) {
			ncalls = json_object_array_length(calls);
			for (icall = 0 ; icall < ncalls ; icall++) {
				call = json_object_array_get_idx(calls, icall);

				/* create a verb for the given parameters */
				verb = json_object_new_object();
				json_object_array_add(verbs, verb);
				add(verb, "verbname", json_object_get(json_object_object_get(call, "name")));
				request = json_object_object_get(call, "request");
				type = gettype_obj(types, json_object_object_get(request, "name"));
				add(verb, "request", json_object_get(type));
				response = json_object_object_get(call, "response");
				type = gettype_obj(types, json_object_object_get(response, "name"));
				add(verb, "response", json_object_get(type));

				/* get the permissions */
				perm = NULL;
				attrs = json_object_object_get(call, "attributes");
				if (json_object_is_type(attrs, json_type_array))
					nattrs = json_object_array_length(attrs);
				else
					nattrs = 0;
				for (iattr = 0 ; iattr < nattrs ; iattr++) {
					attr = json_object_array_get_idx(attrs, iattr);
					key = json_object_object_get(attr, "key");
					value = json_object_object_get(attr, "value");
					if (!strcmp(json_object_get_string(key), "permission")
					 && json_object_is_type(value, json_type_string)) {
						if (perm == NULL) {
							if (permissions == NULL) {
								permissions = json_object_new_array();
								add(api, "has-permissions", json_object_new_boolean(1));
								add(api, "permissions", permissions);
							}
							iperm = record_perm(permissions, value);
							perm = json_object_new_object();
							add(perm, "name", json_object_get(value));
							add(perm, "index", json_object_new_int((int)iperm));
							add(verb, "permission", perm);
						}
						else {
							; /* TODO: it is an error to have more than one permission set */
						}
					}
				}
			}
		}
	}
}

int main(int ac, char **av)
{
	char *filename;
	size_t sz;
	int scan = 1;
	int ja, ia = 1;
	struct json_object *obj, *prod;
	int flags = 0;

	while (scan) {
		if (ia >= ac || av[ia] == NULL || av[ia][0] != '-') {
			scan = 0;
		}
		else if (av[ia][1] == 0) {
			scan = 0;
		}
		else if (av[ia][1] == '-') {
			if (!av[ia][2])
				scan = 0;
			else if (!strncmp(&av[ia][2], "root", 4) && (av[ia][6] == '=' || !av[ia][6])) {
				if (av[ia][6] == '=')
					root = &av[ia][7];
				else if (ia + 1 < ac)
					root = av[++ia];
				else {
					fprintf(stderr, "option --root expects a value\n");
					return 1;
				}
			}
			else if (!strncmp(&av[ia][2], "name", 4) && (av[ia][6] == '=' || !av[ia][6])) {
				if (av[ia][6] == '=')
					name = &av[ia][7];
				else if (ia + 1 < ac)
					name = av[++ia];
				else {
					fprintf(stderr, "option --name expects a value\n");
					return 1;
				}
			}
			else if (!strcmp(&av[ia][2], "json")) {
				wjso = YES;
			}
			else if (!strcmp(&av[ia][2], "no-json")) {
				wjso = NO;
			}
			else if (!strcmp(&av[ia][2], "pretty")) {
				flags |= JSON_C_TO_STRING_PRETTY;
			}
			else {
				fprintf(stderr, "error, unknown option %s\n", av[ia]);
				return 1;
			}
			ia++;
		}
		else {
			ja = 1;
			while (ja && av[ia][ja]) {
				switch (av[ia][ja]) {
				case 'p':
					flags |= JSON_C_TO_STRING_PRETTY;
					ja++;
					break;
				case 'j':
					wjso = YES;
					ja++;
					break;
				case 'J':
					wjso = NO;
					ja++;
					break;
				case 'r':
					if (av[ia][ja + 1]) {
						root = &av[ia][ja + 1];
						ja = 0;
					}
					else if (ia + 1 < ac) {
						root = av[++ia];
						ja = 0;
					}
					else {
						fprintf(stderr, "option -r expects a value\n");
						return 1;
					}
					break;
				case 'n':
					if (av[ia][ja + 1]) {
						name = &av[ia][ja + 1];
						ja = 0;
					}
					else if (ia + 1 < ac) {
						name = av[++ia];
						ja = 0;
					}
					else {
						fprintf(stderr, "option -n expects a value\n");
						return 1;
					}
					break;
				default:
					fprintf(stderr, "error, unknown option -%c\n", av[ia][ja]);
					return 1;
				}
			}
			ia++;
		}
	}

	if (ia + 1 < ac) {
		fprintf(stderr, "error, too many parameters\n");
		return 1;
	}

	/* create the product */
	if (!root)
		prod = json_object_new_object();
	else {
		prod = json_tokener_parse(root);
		if (!json_object_is_type(prod, json_type_object)) {
			fprintf(stderr, "error, given root is not an object\n");
			return 1;
		}
	}
	if (!name && json_object_object_get(prod, "project-name") == NULL)
		name = "";
	if (name)
		add(prod, "project-name", json_object_new_string(name));
	if (wjso == UNSET && json_object_object_get(prod, "generate-json") == NULL)
		wjso = YES;
	if (wjso != UNSET)
		add(prod, "generate-json", json_object_new_boolean(wjso == YES));

	/* load the JSON schema description */
	if (ia == ac)
		filename = "/dev/stdin";
	else {
		filename = av[ia];
		if (filename[0] == '-' && !filename[1])
			filename = "/dev/stdin";
	}
	obj = json_object_from_file(filename);
	if (obj == NULL) {
		fprintf(stderr, "%s\n", json_util_get_last_err());
		return 1;
	}

	/* produce the items */
	record_types(obj, prod);
	record_apis(obj, prod);

	json_object_to_fd(1, prod, flags);
	json_object_put(obj);
	json_object_put(prod);

	return 0;
}
