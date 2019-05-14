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

#include <stdio.h>
#include <stdint.h>

#include "otvar.h"

#include <stdlib.h>

#include <stdarg.h>

#include "local_lib/uthash.h"





typedef union {
    void* pointer;
    int64_t integer;
    double number;
} var_union;

struct varstruct {
    char name[16];
    int id;
    
    size_t size;
    VAR_Type type;
    var_union val;
    
    UT_hash_handle hh;
};

typedef struct {
    size_t size;
    int    id_master;
    struct varstruct* base;
} otvar_t;





static struct varstruct* sub_findvar(otvar_t* otvar, const char* name) {
    struct varstruct* item;
    struct varstruct* base;
    
    //base = otvar->base;
    HASH_FIND_STR(otvar->base, name, item);
    //HASH_FIND_STR(base, name, item);
    return item;
}

static void sub_del(struct varstruct* input) {
    struct varstruct* output;
    if (input != NULL) {
        HASH_DEL(input, output);
        if (output->size != 0) {
            free(output->val.pointer);
        }
        free(output);
    }
}

static void* sub_get_pointer(otvar_handle_t handle, const char* varname) {
    struct varstruct* var;
    otvar_t* otvar;
    void* val = NULL;
    
    if (handle != NULL) {
        otvar = handle;
        if (otvar->base != NULL) {
            var = sub_findvar(otvar, varname);
            if (var != NULL) {
                switch (var->type) {
                case VAR_Binary:
                case VAR_String:
                    val = var->val.pointer;
                default:
                    break;
                }
            }
        }
    }

    return val;
}





int otvar_init(otvar_handle_t* handle) {
    otvar_t* otvar;
    
    if (handle == NULL) {
        return -1;
    }
    
    otvar = malloc(sizeof(otvar_t));
    if (otvar == NULL) {
        return -2;
    }
    
    otvar->size         = 0;
    otvar->id_master    = 0;
    otvar->base         = NULL;
    *handle             = otvar;
    return 0;
}


void otvar_deinit(otvar_handle_t handle) {
    struct varstruct* tmp;
    struct varstruct* item;
    
    if (handle != NULL) {
        struct varstruct* vartab = ((otvar_t*)handle)->base;
        
        HASH_ITER(hh, vartab, item, tmp) {
            HASH_DEL(vartab, item);         // delete item (vartab advances to next)
            if (item->size != 0) {
                free(item->val.pointer);
            }
            free(item);
        }
        
        free(handle);
    }
}


int otvar_del(otvar_handle_t handle, const char* varname) {
    struct varstruct* input;
    struct varstruct* output;
    otvar_t* otvar;
    int dels = 0;
    
    if (handle == NULL) {
        return -1;
    }
    
    otvar = handle;
    if (otvar->base == NULL) {
        return -2;
    }
    
    input = sub_findvar(otvar, varname);
    if (input != NULL) {
        HASH_DEL(input, output);
        if (output->size != 0) {
            free(output->val.pointer);
        }
        free(output);
        dels++;
    }
    
    return dels;
}


int otvar_add(otvar_handle_t handle, const char* varname, VAR_Type type, ...) {
    struct varstruct* input;
    struct varstruct* base;
    otvar_t* otvar;
    va_list ap;
    void* datafield;
    
    if (handle == NULL) {
        return -1;
    }
    
    otvar = handle;
//     if (otvar->base == NULL) {
//         return -2;
//     }
    
    input = sub_findvar(otvar, varname);
    if (input != NULL) {
        sub_del(input);
    }
    
    input = malloc(sizeof(struct varstruct));
    if (input == NULL) {
        return -4;
    }
    
    input->type = type;
    va_start(ap, type);
    switch (type) {
        default:
        case VAR_Binary: {
            input->size         = va_arg(ap, size_t);
            datafield           = (void*)va_arg(ap, const uint8_t*);
        } break;
        case VAR_String: {
            datafield           = (void*)va_arg(ap, const char*);
            input->size         = strlen( (char*)datafield ) + 1;
            input->val.pointer  = malloc(input->size);
        } break;
        case VAR_Int: {
            input->size         = 0;
            input->val.integer  = va_arg(ap, int64_t);
        } break;
        case VAR_Float: {
            input->size         = 0;
            input->val.integer  = va_arg(ap, double);
        } break;
    }
    
    va_end(ap);
    switch (type) {
        case VAR_Binary:
        case VAR_String:
            input->val.pointer  = malloc(input->size);
            if (input->val.pointer == NULL) {
                goto otvar_add_TERM;
            }
            memcpy(input->val.pointer, datafield, input->size);
            break;
        
        case VAR_Int:
        case VAR_Float:
        default: break;
    }
    
    input->id = otvar->id_master++;
    strncpy(input->name, varname, 15);

    //base = otvar->base;
    //HASH_ADD_STR( base, name, input);
    HASH_ADD_STR(otvar->base, name, input);
    return 0;

    otvar_add_TERM:
    free(input);
    return -5;
}


double otvar_get_number(otvar_handle_t handle, const char* varname) {
    struct varstruct* var;
    otvar_t* otvar;
    double val = 0.0;
    
    if (handle != NULL) {
        otvar = handle;
        if (otvar->base != NULL) {
            var = sub_findvar(otvar, varname);
            if (var != NULL) {
                switch (var->type) {
                case VAR_Int:
                    val = (double)(var->val.integer);
                    break;
                case VAR_Float:
                    val = var->val.number;
                    break;
                default:
                    val = 0.0;
                    break;
                }
            }
        }
    }

    return val;
}


int64_t otvar_get_integer(otvar_handle_t handle, const char* varname) {
    struct varstruct* var;
    otvar_t* otvar;
    int64_t val = 0;
    
    if (handle != NULL) {
        otvar = handle;
        if (otvar->base != NULL) {
            var = sub_findvar(otvar, varname);
            if (var != NULL) {
                switch (var->type) {
                case VAR_Int:
                    val = var->val.integer;
                    break;
                case VAR_Float:
                    val = (int64_t)(var->val.number);
                    break;
                default:
                    val = 0;
                    break;
                }
            }
        }
    }

    return val;
}


const char* otvar_get_string(otvar_handle_t handle, const char* varname) {
    return (const char*)sub_get_pointer(handle, varname);
}


const void* otvar_get_binary(otvar_handle_t handle, const char* varname) {
    return (const void*)sub_get_pointer(handle, varname);
}


const otvar_item_t otvar_get_item(otvar_handle_t handle, const char* varname) {
    struct varstruct* var = NULL;
    if (handle != NULL) {
        otvar_t* otvar;
        otvar = handle;
        if (otvar->base != NULL) {
            var = sub_findvar(otvar, varname);
        }
    }
    return (otvar_item_t)var;
}

size_t otvar_item_size(otvar_item_t item) {
    struct varstruct* var = (struct varstruct*)item;
    size_t item_size;
    if (var != NULL) {
        item_size = var->size;
    }
    else {
        item_size = 0;
    }
    return item_size;
}

const char* otvar_item_name(otvar_item_t item) {
    struct varstruct* var = (struct varstruct*)item;
    const char* item_name;
    if (var != NULL) {
        item_name = var->name;
    }
    else {
        item_name = NULL;
    }
    return item_name;
}

VAR_Type otvar_item_type(otvar_item_t item) {
    struct varstruct* var = (struct varstruct*)item;
    VAR_Type item_type;
    if (var != NULL) {
        item_type = var->type;
    }
    else {
        item_type = VAR_Invalid;
    }
    return item_type;
}

double otvar_item_number(otvar_item_t item) {
    struct varstruct* var = (struct varstruct*)item;
    double item_number = 0.0;
    if (var != NULL) {
        if (var->type == VAR_Float) {
            item_number = var->val.number;
        }
        else if (var->type == VAR_Int) {
            item_number = (double)(var->val.integer);
        }
    }
    return item_number;
}

int64_t otvar_item_integer(otvar_item_t item) {
    struct varstruct* var = (struct varstruct*)item;
    int64_t item_integer = 0;
    if (var != NULL) {
        if (var->type == VAR_Int) {
            item_integer = var->val.integer;
        }
        else if (var->type == VAR_Float) {
            item_integer = (int64_t)(var->val.number);
        }
    }
    return item_integer;
}

const char* otvar_item_string(otvar_item_t item) {
    struct varstruct* var = (struct varstruct*)item;
    const char* item_string = NULL;
    if (var != NULL) {
        if (var->type == VAR_String) {
            item_string = var->val.pointer;
        }
    }
    return item_string;
}

const void* otvar_item_binary(otvar_item_t item) {
    struct varstruct* var = (struct varstruct*)item;
    const void* item_binary = NULL;
    if (var != NULL) {
        if (var->type == VAR_Binary) {
            item_binary = var->val.pointer;
        }
    }
    return item_binary;
}

