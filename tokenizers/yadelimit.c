/*
  Copyright(C) 2014 Naoya Murakami <naoya@createfield.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301  USA

  This file includes the Groonga delimited tokenizer code.
  https://github.com/groonga/groonga/blob/master/lib/token.c

  The following is the header of the file:

    Copyright(C) 2009-2012 Brazil

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License version 2.1 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
    USA
*/

#include <groonga/tokenizer.h>

#include <string.h>
#include <ctype.h>

#ifdef __GNUC__
#  define GNUC_UNUSED __attribute__((__unused__))
#else
#  define GNUC_UNUSED
#endif

#define GRN_STRING_ENABLE_NORMALIZER_FILTER (0x01<<5)

typedef struct {
  grn_tokenizer_token token;
  grn_tokenizer_query *query;
  const unsigned char *next;
  const unsigned char *end;
  grn_bool have_tokenized_delimiter;
  const unsigned short *delimiter;
  unsigned int delimiter_len;
  grn_bool delimit_punct;
} grn_yadelimit_tokenizer;


static grn_obj *
yadelimit_init(grn_ctx *ctx, int nargs, grn_obj **args, grn_user_data *user_data,
               const unsigned short *delimiter, unsigned int delimiter_len,
               grn_bool delimit_punct)
{
  grn_tokenizer_query *query;
  unsigned int normalize_flags = GRN_STRING_ENABLE_NORMALIZER_FILTER;
  const char *normalized;
  unsigned int normalized_length_in_bytes;
  grn_yadelimit_tokenizer *tokenizer;

  query = grn_tokenizer_query_open(ctx, nargs, args, normalize_flags);
  if (!query) {
    return NULL;
  }

  if (!(tokenizer = GRN_PLUGIN_MALLOC(ctx,sizeof(grn_yadelimit_tokenizer)))) {
    GRN_PLUGIN_ERROR(ctx,GRN_NO_MEMORY_AVAILABLE,
                     "[tokenizer][yadelimit] "
                     "memory allocation to grn_yadelimit_tokenizer failed");
    grn_tokenizer_query_close(ctx, query);
    return NULL;
  }
  user_data->ptr = tokenizer;
  grn_tokenizer_token_init(ctx, &(tokenizer->token));
  tokenizer->query = query;
  grn_string_get_normalized(ctx, tokenizer->query->normalized_query,
                            &normalized, &normalized_length_in_bytes,
                            NULL);
  tokenizer->next = (const unsigned char *)normalized;
  tokenizer->end = tokenizer->next + normalized_length_in_bytes;

  tokenizer->have_tokenized_delimiter =
    grn_tokenizer_have_tokenized_delimiter(ctx,
                                           tokenizer->query->ptr,
                                           tokenizer->query->length,
                                           tokenizer->query->encoding);
  tokenizer->delimiter = delimiter;
  tokenizer->delimiter_len = delimiter_len;
  tokenizer->delimit_punct = delimit_punct;

  return NULL;
}

static grn_obj *
yadelimit_next(grn_ctx *ctx, GNUC_UNUSED int nargs, GNUC_UNUSED grn_obj **args,
               grn_user_data *user_data)
{
  grn_yadelimit_tokenizer *tokenizer = user_data->ptr;

  if (tokenizer->have_tokenized_delimiter) {
    unsigned int rest_length;
    rest_length = tokenizer->end - tokenizer->next;
    tokenizer->next =
      (unsigned char *)grn_tokenizer_tokenized_delimiter_next(
        ctx,
        &(tokenizer->token),
        (const char *)tokenizer->next,
        rest_length,
        tokenizer->query->encoding);
  } else {
    const unsigned char *token_top = tokenizer->next;
    const unsigned char *token_tail;
    const unsigned char *string_end = tokenizer->end;

    unsigned int char_length;
    unsigned int rest_length = string_end - token_top;

    grn_tokenizer_status status;

    for (token_tail = token_top; token_tail < string_end; token_tail += char_length) {
      if (!(char_length = grn_plugin_charlen(ctx, (char *)token_tail, rest_length,
                                             tokenizer->query->encoding))) {
        tokenizer->next = (unsigned char *)string_end;
        break;
      }
      if (tokenizer->delimit_punct){
        if(ispunct(*token_tail)){
          tokenizer->next = token_tail + char_length;
          break;
        }
      }
      if (tokenizer->delimiter) {
        if (token_tail + tokenizer->delimiter_len <= string_end &&
            !memcmp(token_tail, tokenizer->delimiter, tokenizer->delimiter_len)) {
          tokenizer->next = token_tail + tokenizer->delimiter_len;
          break;
        }
      }
    }
    if (token_tail == string_end) {
      status = GRN_TOKENIZER_LAST;
    } else {
      status = GRN_TOKENIZER_CONTINUE;
    }

    if (token_tail == token_top) {
      status |= GRN_TOKENIZER_TOKEN_SKIP_WITH_POSITION;
    }

    grn_tokenizer_token_push(ctx, &(tokenizer->token),
                             (const char *)token_top, token_tail - token_top, status);
  }

  return NULL;
}

static grn_obj *
yadelimit_fin(grn_ctx *ctx, GNUC_UNUSED int nargs, GNUC_UNUSED grn_obj **args,
              grn_user_data *user_data)
{
  grn_yadelimit_tokenizer *tokenizer = user_data->ptr;
  if (!tokenizer) {
    return NULL;
  }
  grn_tokenizer_query_close(ctx, tokenizer->query);
  grn_tokenizer_token_fin(ctx, &(tokenizer->token));
  GRN_PLUGIN_FREE(ctx,tokenizer);
  return NULL;
}

static grn_obj *
yadelimit_caret_init(grn_ctx *ctx, int nargs, grn_obj **args,
                     grn_user_data *user_data)
{
  static const unsigned short delimiter[1] = {'^'};
  return yadelimit_init(ctx, nargs, args, user_data, delimiter, 1, 0);
}

static grn_obj *
yadelimit_colon_init(grn_ctx *ctx, int nargs, grn_obj **args,
                     grn_user_data *user_data)
{
  static const unsigned short delimiter[1] = {':'};
  return yadelimit_init(ctx, nargs, args, user_data, delimiter, 1, 0);
}

static grn_obj *
yadelimit_semicolon_init(grn_ctx *ctx, int nargs, grn_obj **args,
                         grn_user_data *user_data)
{
  static const unsigned short delimiter[1] = {';'};
  return yadelimit_init(ctx, nargs, args, user_data, delimiter, 1, 0);
}

static grn_obj *
yadelimit_punct_init(grn_ctx *ctx, int nargs, grn_obj **args,
                     grn_user_data *user_data)
{
  return yadelimit_init(ctx, nargs, args, user_data, NULL, 0, GRN_TRUE);
}

static grn_obj *
yadelimit_punct_with_blank_init(grn_ctx *ctx, int nargs, grn_obj **args,
                                grn_user_data *user_data)
{
  static const unsigned short delimiter[1] = {' '};
  return yadelimit_init(ctx, nargs, args, user_data, delimiter, 1, GRN_TRUE);
}

grn_rc
GRN_PLUGIN_INIT(grn_ctx *ctx)
{
  return ctx->rc;
}

grn_rc
GRN_PLUGIN_REGISTER(grn_ctx *ctx)
{
  grn_rc rc;

  rc = grn_tokenizer_register(ctx, "TokenYaDelimitCaret", -1,
                              yadelimit_caret_init, yadelimit_next, yadelimit_fin);
  rc = grn_tokenizer_register(ctx, "TokenYaDelimitColon", -1,
                              yadelimit_colon_init, yadelimit_next, yadelimit_fin);
  rc = grn_tokenizer_register(ctx, "TokenYaDelimitSemicolon", -1,
                              yadelimit_semicolon_init, yadelimit_next, yadelimit_fin);
  rc = grn_tokenizer_register(ctx, "TokenYaDelimitPunct", -1,
                              yadelimit_punct_init, yadelimit_next, yadelimit_fin);
  rc = grn_tokenizer_register(ctx, "TokenYaDelimitPunctWithBlank", -1,
                              yadelimit_punct_with_blank_init, yadelimit_next, yadelimit_fin);

  return rc;
}

grn_rc
GRN_PLUGIN_FIN(GNUC_UNUSED grn_ctx *ctx)
{
  return GRN_SUCCESS;
}
