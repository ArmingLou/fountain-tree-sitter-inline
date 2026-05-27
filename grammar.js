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
      $.boneyard,
      $.inline_note,
      $.paren_text,
      $.text
    ),

    // 完整的 boneyard 注释 /* ... */
    boneyard: $ => seq(
      $.boneyard_start,
      repeat1(choice(/[^*]+/, /\*[^\/]/)),
      '*/'
    ),

    // 完整的 inline_note [[...]]
    inline_note: $ => seq(
      $.note_start,
      repeat1(choice(/[^\]]+/, /\][^\]]/)),
      ']]'
    ),

    // text - 排除 scanner 处理的特殊字符
    text: $ => token(prec(-1, /[^\/\[\(]/))
  }
});