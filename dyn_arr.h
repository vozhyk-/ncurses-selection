/*
 * dyn_arr, part of ncursed-tanks
 * (https://github.com/AwesomePatrol/ncursed-tanks/blob/master/src/dyn_arr.h),
 * writen by vozhyk (Vitaut Bajaryn), licensed under the MIT license. :
 * Copyright (c) 2015, Vitaut Bajaryn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * */

#ifndef DYN_ARR_H
#define DYN_ARR_H

#include <stdlib.h>
#include <string.h>

/* Dynamic array of elements of size el_size
 *
 * To define a dyn_arr of type:
 *   struct dyn_arr name = { sizeof(type) };
 */
struct dyn_arr
{
    size_t el_size;
    size_t count;
    /* use a flexible array member instead of pointer? */
    void *data;
};

/* Dynamic array of pointers
 *
 * To define one:
 *   struct p_dyn_arr name = {0};
 */
struct p_dyn_arr
{
    size_t count;
    void **data;
};

void *dyn_arr_get(const struct dyn_arr *arr, size_t i);
void *dyn_arr_append(struct dyn_arr *arr, void *el);
void dyn_arr_clear(struct dyn_arr *arr);
void dyn_arr_delete(struct dyn_arr *arr, void *el);

void **p_dyn_arr_get_location(const struct p_dyn_arr *arr, size_t i);
void *p_dyn_arr_get(const struct p_dyn_arr *arr, size_t i);
void *p_dyn_arr_append(struct p_dyn_arr *arr, void *el);
void p_dyn_arr_clear(struct p_dyn_arr *arr);
void p_dyn_arr_delete(struct p_dyn_arr *arr, void **location);

#endif /* DYN_ARR_H */
