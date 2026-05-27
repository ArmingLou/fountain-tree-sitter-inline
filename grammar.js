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
      $.text,
      $.any_char
    ),

    boneyard: $ => seq(
      $.boneyard_start,
      repeat1(choice(/[^*]+/, /\*[^\/]/)),
      '*/'
    ),

    inline_note: $ => seq(
      $.note_start,
      repeat1(choice(/[^\]]+/, /\][^\]]/)),
      ']]'
    ),

    // text 排除特殊字符（含中文全角左括号），迫使 scanner 被调用
    text: $ => token(prec(-1, /[^\/\[\(\uff08]/)),

    // 兜底：匹配任意单个字符（含 ( ），当 scanner 不匹配时
    any_char: $ => token(prec(-2, /[\s\S]/))
  }
});