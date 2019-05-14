/* Copyright 2019, JP Norair
  *
  * Licensed under the OpenTag License, Version 1.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  * http://www.indigresso.com/wiki/doku.php?id=opentag:license_1_0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  */

#ifndef otvar_h
#define otvar_h

#include <stdio.h>
#include <stdint.h>

typedef enum {
    VAR_Binary = 0,
    VAR_String,
    VAR_Int,
    VAR_Float,
    VAR_Invalid
} VAR_Type;

typedef void* otvar_handle_t;

typedef void* otvar_item_t;


int otvar_init(otvar_handle_t* handle);

void otvar_deinit(otvar_handle_t handle);

int otvar_del(otvar_handle_t handle, const char* varname);

int otvar_add(otvar_handle_t handle, const char* varname, VAR_Type type, ...);

const otvar_item_t otvar_get_item(otvar_handle_t handle, const char* varname);

size_t otvar_item_size(otvar_item_t item);
const char* otvar_item_name(otvar_item_t item);
VAR_Type otvar_item_type(otvar_item_t item);
double otvar_item_number(otvar_item_t item);
int64_t otvar_item_integer(otvar_item_t item);
const char* otvar_item_string(otvar_item_t item);
const void* otvar_item_binary(otvar_item_t item);


double otvar_get_number(otvar_handle_t handle, const char* varname);
int64_t otvar_get_integer(otvar_handle_t handle, const char* varname);
const char* otvar_get_string(otvar_handle_t handle, const char* varname);
const void* otvar_get_binary(otvar_handle_t handle, const char* varname);

#endif
