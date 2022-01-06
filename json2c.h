/*
 * Copyright (C) 2016-2022 IoT.bzh Company
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

#pragma once

struct json_object;

extern char *str2c(const char *str, const char *prefix, int width);
extern char *str2c_std(const char *str);
extern char *str2c_inl(const char *str);

extern char *json2c(struct json_object *object, const char *prefix, int width);
extern char *json2c_std(struct json_object *object);
extern char *json2c_inl(struct json_object *object);
