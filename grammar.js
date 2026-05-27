module.exports = grammar({
  name: 'fountain_inline',

  extras: $ => [/\n/],

  externals: $ => [
    $.paren_text,
    $.boneyard_start,
    $.note_start,
  ],

  rules: {
    document: $ => repeat1($._element),

    _element: $ => choice(
      // boneyard: /* 后跟内容(含单独的*但不含*/) 到 */
      seq($.boneyard_start, repeat1(choice(/[^*]+/, /\*[^\/]/)), '*/'),
      // inline_note: [[ 后跟内容(含单独的]但不含]]) 到 ]]
      seq($.note_start, repeat1(choice(/[^\]]+/, /\][^\]]/)), ']]'),
      $.paren_text,
      $.text
    ),

    text: $ => token(prec(-1, /[^\/\[\(]/))
  }
});