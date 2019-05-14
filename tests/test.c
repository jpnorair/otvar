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

#include "otvar.h"

#include <stdio.h>
#include <stdint.h>



int main(void) {
    otvar_handle_t vardict;
    otvar_item_t item;
    int rc;
    VAR_Type val_type;
    size_t val_size;
    double val_number;
    int64_t val_integer;
    const char* val_string;
    
    rc = otvar_init(&vardict);
    printf("otvar_init() returned %i\n", rc);
    
    printf("--> using otvar_add() to add a bunch of variables\n");

    rc = otvar_add(vardict, "verbose", VAR_Int, (int64_t)0);
    printf("otvar_add() returned %i\n", rc);
    
    otvar_add(vardict, "quiet", VAR_Int, (int64_t)0);
    printf("otvar_add() returned %i\n", rc);
    
    otvar_add(vardict, "timeout", VAR_Int, (int64_t)16);
    printf("otvar_add() returned %i\n", rc);
    
    otvar_add(vardict, "mempool", VAR_Int, (int64_t)(512*1024));
    printf("otvar_add() returned %i\n", rc);
    
    otvar_add(vardict, "mbsrc", VAR_Int, (int64_t)1);
    printf("otvar_add() returned %i\n", rc);
    
    otvar_add(vardict, "mbdst", VAR_Int, (int64_t)2);
    printf("otvar_add() returned %i\n", rc);
    
    
    printf("--> using otvar_get_item() to get timeout\n");
    
    item = otvar_get_item(vardict, "timeout");
    printf("otvar_get_item() returned %llx (item address)\n", (uint64_t)item);
    
    val_type = otvar_item_type(item);
    printf("otvar_item_type() returned %u\n", val_type);
    
    val_size = otvar_item_size(item);
    printf("otvar_item_size() returned %zu\n", val_size);
    
    val_number = otvar_item_number(item);
    printf("otvar_item_number() returned %lf\n", val_number);
    
    val_integer = otvar_item_integer(item);
    printf("otvar_item_integer() returned %lli\n", val_integer);
    
    val_string = otvar_item_string(item);
    printf("otvar_item_string() returned %s\n", val_string);

    return 0;
}
