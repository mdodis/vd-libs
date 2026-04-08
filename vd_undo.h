/**
 * vd_undo.h
 * Author: Michael Dodis
 *
 * @todo:
 * 
 */

#ifndef VD_UNDO_H
#define VD_UNDO_H

#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t state_size;
    uint32_t capacity;
    uint8_t  *buf;
    uint32_t first;
    uint32_t curr;
} VdUndo;

VdUndo   vd_undo_make(uint8_t *buf, uint32_t capacity, uint32_t state_size);
int      vd_undo_invalidate(VdUndo *u, void *before, void *after, size_t size);
int      vd_undo_invalidate_mut_before(VdUndo *u, void *before, void *after, size_t size);
int      vd_undo_undo(VdUndo *u, void *output, size_t size);
int      vd_undo_redo(VdUndo *u, void *output, size_t size);
#endif // !VD_UNDO_H

#ifdef VD_UNDO_IMPL

typedef struct {
    uint32_t size;
    uint32_t start;
} VdUndo__Change;

typedef struct {
    uint32_t prev;
    uint32_t next;
    uint32_t size;
} VdUndo__BlockFooter;

static uint32_t vd_undo__buf_r(VdUndo *u, uint32_t from, void *out, uint32_t size)
{
    uint8_t *ptr = (uint8_t*)out;

    if ((from + size) > u->capacity) {
        uint32_t first_piece = u->capacity - from;
        uint32_t last_piece = size - first_piece;

        memcpy(ptr, u->buf + from, first_piece);
        memcpy(ptr + first_piece, u->buf, last_piece);
        from = last_piece;
    } else {
        memcpy(ptr, u->buf + from, size);
        from += size;
    }
    return from;
}

static uint32_t vd_undo__buf_w(VdUndo *u, uint32_t to, void *in, uint32_t size)
{
    uint8_t *ptr = (uint8_t*)in;

    if ((to + size) > u->capacity) {
        uint32_t first_piece = u->capacity - to;
        uint32_t last_piece = size - first_piece;

        memcpy(u->buf + to, ptr, first_piece);
        memcpy(u->buf, ptr + first_piece, last_piece);
        to = last_piece;
    } else {
        memcpy(u->buf + to, ptr, size);
        to += size;
    }

    return to;
}

VdUndo__BlockFooter vd_undo__get(VdUndo *u, uint32_t at)
{
    uint32_t r = (at - sizeof(VdUndo__BlockFooter)) % u->capacity;

    VdUndo__BlockFooter result;
    vd_undo__buf_r(u, r, &result, sizeof(result));
    return result;
}

VdUndo__BlockFooter vd_undo__first(VdUndo *u, uint32_t *handle)
{
    *handle = u->first;
    return vd_undo__get(u, u->first);
}

VdUndo__BlockFooter vd_undo_curr(VdUndo *u, uint32_t *handle)
{
    *handle = u->curr;
    return vd_undo__get(u, u->curr);
}

uint32_t vd_undo_block_off(VdUndo *u, VdUndo__BlockFooter block)
{
    uint32_t my_offset = vd_undo__get(u, block.prev).next;
    return (my_offset + u->capacity - block.size) % u->capacity;
}

uint32_t vd_undo_block_end_before_footer(VdUndo *u, VdUndo__BlockFooter block)
{
    uint32_t my_offset = (vd_undo__get(u, block.prev).next) % u->capacity;
    return (my_offset + u->capacity - sizeof(block)) % u->capacity;
}

uint32_t vd_undo_block_end(VdUndo *u, VdUndo__BlockFooter block)
{
    uint32_t my_offset = (vd_undo__get(u, block.prev).next) % u->capacity;
    return my_offset;
}

VdUndo__BlockFooter vd_undo__next(VdUndo *u, VdUndo__BlockFooter cur)
{
    return vd_undo__get(u, cur.next);
}

static void vd_undo__write_block_handle(VdUndo *u, VdUndo__BlockFooter footer, uint32_t handle)
{
    uint32_t w = (handle - sizeof(VdUndo__BlockFooter)) % u->capacity;
    vd_undo__buf_w(u, w, &footer, sizeof(footer));
}

static int vd_undo__block_eq(VdUndo__BlockFooter a, VdUndo__BlockFooter b)
{
    return (a.size == b.size) && (a.next == b.next) && (a.prev == b.prev);
}

uint32_t vd_undo__append(VdUndo *u, uint32_t size, int *overwrite)
{
    uint32_t overall = size + sizeof(VdUndo__BlockFooter);

    if (overall + (sizeof(VdUndo__Change) + sizeof(VdUndo__BlockFooter)) >= u->capacity) {
        return ~0u;
    }

    *overwrite = 0;

    uint32_t first_handle;
    VdUndo__BlockFooter first = vd_undo__first(u, &first_handle);

    uint32_t last_handle = first.prev;
    VdUndo__BlockFooter last = vd_undo__get(u, last_handle);

    {
        uint32_t curr_handle = u->curr;
        VdUndo__BlockFooter curr = vd_undo__get(u, curr_handle);
        if (!vd_undo__block_eq(curr, last)) {
            // Remove everything between curr and last
            curr.next = first_handle;
            vd_undo__write_block_handle(u, curr, curr_handle);
            first = vd_undo__get(u, first_handle);

            first.prev = curr_handle;
            vd_undo__write_block_handle(u, first, first_handle);

            curr = vd_undo__get(u, curr_handle);
            first = vd_undo__get(u, first_handle);
        }

        last = curr;
        last_handle = curr_handle;
    }

    uint32_t new_start = last_handle;
    uint32_t new_end = (last_handle + overall);
    new_end = new_end % u->capacity;

    // Clear everything between new_start and new_end
    {
        VdUndo__BlockFooter curr = first;
        VdUndo__BlockFooter prev_curr;
        do {
            uint32_t curr_start = vd_undo_block_off(u, curr);
            uint32_t curr_end   = vd_undo_block_end(u, curr);
            int curr_wraps = curr_end < curr_start;
            int new_wraps  = new_end < new_start;

            int hit = 0;
            if (!new_wraps && !curr_wraps) {
                // None wrap, basic hit testing
                hit = (curr_start < new_end) && (new_start < curr_end);
            } else if (new_wraps && !curr_wraps) {
                // Just the range wraps, we just invert the statement below that checks 
                // if curr is exclusievely in the empty segment
                hit = !((curr_start > new_end) && (curr_end < new_start));
            } else if (!new_wraps && curr_wraps) {
                // Just the current block wraps, we do the reverse of the above
                hit = !((new_start > curr_end) && (new_end < curr_start));
            } else {
                // Both wrap, so there's no point in doing hit testing
                hit = 1;
            }

            prev_curr = curr;
            curr = vd_undo__next(u, curr);

            if (hit) {
                *overwrite = 1;
                uint32_t curr_handle = vd_undo_block_end(u, curr);

                // Remove last linkage
                last.next = curr_handle;
                vd_undo__write_block_handle(u, last, last_handle);

                // Remove prev linkage
                curr.prev = last_handle;
                vd_undo__write_block_handle(u, curr, curr_handle);

                u->first = curr_handle;
                first = vd_undo__first(u, &first_handle);
            } else {
                break;
            }

            prev_curr = vd_undo__get(u, curr.prev);
        } while (!vd_undo__block_eq(curr, prev_curr));
    }

    last.next = new_end;
    vd_undo__write_block_handle(u, last, last_handle);

    first = vd_undo__first(u, &first_handle);
    first.prev = new_end;
    vd_undo__write_block_handle(u, first, first_handle);

    VdUndo__BlockFooter new_footer;
    new_footer.prev = last_handle;
    new_footer.next = first_handle;
    new_footer.size = overall;

    u->curr = new_end;

    uint32_t metadata_write = (new_start + size) % u->capacity;
    vd_undo__buf_w(u, metadata_write, &new_footer, sizeof(new_footer));

    return new_start;
}

int vd_undo_push(VdUndo *u, uint32_t *out_start, uint32_t size, int handle_overwrite)
{
    int overwrite;
    uint32_t write_start = vd_undo__append(u, size, &overwrite);
    if (write_start > u->capacity) {
        return 0;
    }

    *out_start = write_start;

    if (overwrite) {
        VdUndo__Change change;
        change.size = 0;
        change.start = 0;
        int unused_overwrite;
        uint32_t prev_curr = u->curr;
        write_start = vd_undo__append(u, sizeof(change), &unused_overwrite);
        vd_undo__buf_w(u, write_start, &change, sizeof(change));
        u->first = u->curr;
        u->curr = prev_curr;
    }

    return 1;
}

int vd_undo_invalidate(VdUndo *u, void *before, void *after, size_t size)
{
    int result = 0;

    if (u->state_size != size) {
        return 0;
    }

    uint8_t *abytes = (uint8_t*)after;
    uint8_t *bbytes = (uint8_t*)before;

    uint32_t overall_change_size = 0;
    for (uint32_t i = 0; i < size; ++i) {
        uint32_t before_i = i;
        while ((i < size) && (abytes[i] != bbytes[i])) {
            i++;
        }

        if (before_i < i) {
            overall_change_size += sizeof(VdUndo__Change) + (i - before_i);
        }
    }

    uint32_t write_start = ~0u;
    if (overall_change_size > 0) {
        result = vd_undo_push(u, &write_start, overall_change_size, 1);
    }

    if (result) {
        uint32_t curr = write_start;
        uint32_t written = 0;
        for (uint32_t i = 0; i < size; ++i) {
            uint32_t before_i = i;
            while ((i < size) && (abytes[i] != bbytes[i])) {
                i++;
            }

            if (before_i < i) {
                written += sizeof(VdUndo__Change) + (i - before_i);

                VdUndo__Change change;
                change.start = before_i;
                change.size = i - before_i;
                curr = vd_undo__buf_w(u, curr, &change, sizeof(change));

                for (uint32_t j = before_i; j < i; ++j) {
                    uint8_t diff = abytes[j] - bbytes[j];
                    curr = vd_undo__buf_w(u, curr, &diff, 1);
                }
            }
        }
    }

    return result;
}

int vd_undo_invalidate_mut_before(VdUndo *u, void *before, void *after, size_t size)
{
    int result = 0;

    if (u->state_size != size) {
        return 0;
    }

    uint8_t *abytes = (uint8_t*)after;
    uint8_t *bbytes = (uint8_t*)before;

    uint32_t overall_change_size = 0;

    for (uint32_t i = 0; i < size; ++i) {

        // Diff the before & after structs
        bbytes[i] = abytes[i] - bbytes[i];
        if (bbytes[i] != 0) {
            overall_change_size++;
        }
    }

    if (overall_change_size == 0) {
        return 0;
    }


    uint32_t write_start = ~0u;
    if (overall_change_size > 0) {
        result = vd_undo_push(u, &write_start, overall_change_size, 1);
    }

    if (result) {
        uint32_t curr = write_start;
        uint32_t written = 0;
        for (uint32_t i = 0; i < size; ++i) {
            uint32_t before_i = i;
            while ((i < size) && (abytes[i] != bbytes[i])) {
                i++;
            }

            if (before_i < i) {
                written += sizeof(VdUndo__Change) + (i - before_i);

                VdUndo__Change change;
                change.start = before_i;
                change.size = i - before_i;
                curr = vd_undo__buf_w(u, curr, &change, sizeof(change));

                for (uint32_t j = before_i; j < i; ++j) {
                    uint8_t diff = abytes[j] - bbytes[j];
                    curr = vd_undo__buf_w(u, curr, &diff, 1);
                }
            }
        }
    }

    return result;
}

int vd_undo_undo(VdUndo *u, void *output, size_t size)
{
    VdUndo__BlockFooter first = vd_undo__get(u, u->first);
    VdUndo__BlockFooter curr = vd_undo__get(u, u->curr);
    if (vd_undo__block_eq(curr, first)) {
        return 0;    
    }

    curr = vd_undo__get(u, u->curr);
    uint32_t r = vd_undo_block_off(u, curr);
    uint32_t e = vd_undo_block_end_before_footer(u, curr);

    uint8_t *out = (uint8_t*)output;

    do {
        VdUndo__Change change;
        r = vd_undo__buf_r(u, r, &change, sizeof(change));

        uint8_t *ptr = out + change.start;
        for (uint32_t i = 0; i < change.size; ++i) {
            uint8_t diff;
            r = vd_undo__buf_r(u, r, &diff, 1);
            ptr[i] -= diff;
        }

    } while (r != e);

    u->curr = curr.prev;

    return 1;
}

int vd_undo_redo(VdUndo *u, void *output, size_t size)
{
    VdUndo__BlockFooter first = vd_undo__get(u, u->first);
    VdUndo__BlockFooter last = vd_undo__get(u, first.prev);
    VdUndo__BlockFooter curr = vd_undo__get(u, u->curr);
    if (vd_undo__block_eq(curr, last)) {
        return 0;
    }

    u->curr = curr.next;

    curr = vd_undo__get(u, u->curr);
    uint32_t r = vd_undo_block_off(u, curr);
    uint32_t e = vd_undo_block_end_before_footer(u, curr);

    uint8_t *out = (uint8_t*)output;

    do {
        VdUndo__Change change;
        r = vd_undo__buf_r(u, r, &change, sizeof(change));

        uint8_t *ptr = out + change.start;
        for (uint32_t i = 0; i < change.size; ++i) {
            uint8_t diff;
            r = vd_undo__buf_r(u, r, &diff, 1);
            ptr[i] += diff;
        }

    } while (r != e);

    return 1;
}

VdUndo vd_undo_make(uint8_t *buf, uint32_t capacity, uint32_t state_size)
{
    VdUndo result;
    VdUndo__Change change;
    VdUndo__BlockFooter footer;

    result.buf = buf;
    result.capacity = capacity;
    result.state_size = state_size;

    uint32_t first_block_end = sizeof(change) + sizeof(footer);
    change.size = 0;
    change.start = 0;
    footer.prev = first_block_end;
    footer.next = first_block_end;
    footer.size = sizeof(change) + sizeof(footer);

    uint32_t cur = 0;
    cur = vd_undo__buf_w(&result, cur, &change, sizeof(change));
    cur = vd_undo__buf_w(&result, cur, &footer, sizeof(footer));
    result.first = first_block_end;
    result.curr = result.first;
    return result;
}

#endif // VD_UNDO_IMPL